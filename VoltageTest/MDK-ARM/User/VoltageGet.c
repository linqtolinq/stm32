#include "VoltageGet.h"
#include "usart.h"

uint16_t sysTick = 0;

uint16_t NB_Uart_Cnt = 0;
uint16_t Debug_Uart_Cnt = 0;

uint16_t NB_End_Flag = 0;
uint16_t Debug_End_Flag = 0;
uint16_t Sensor_task_Flag = 0;

SystemTypeDef StrSystem;

// 1ms
void inc_sysTick(void)
{
    sysTick++;
    sysTick %= 5000;

    if (NB_Uart_Cnt > 0)
        NB_Uart_Cnt--;
    if (NB_Uart_Cnt == 1)
    {
        NB_Uart_Cnt = 0;
        NB_End_Flag = 1;
    }

    if (Debug_Uart_Cnt > 0)
        Debug_Uart_Cnt--;
    if (Debug_Uart_Cnt == 1)
    {
        Debug_Uart_Cnt = 0;
        Debug_End_Flag = 1;
    }

    // sensor task 500ms
    if(sysTick%500 == 0)
    {
        Sensor_task_Flag = 1;
    }
}

void processUartInfo(void)
{
  // uart1 information (NB Uart)
  if (NB_End_Flag && NB_Uart_Cnt == 0)
  {
    if (Modbus_Read_03_Rx() != 0)
    {
        StrSystem.UART_TX_FLAG = SET;
			printf("read sensor error\r\n");
    }
		StrSystem.UART_TX_FLAG = SET;
    //    printf("read UART\r\n");
	  //HAL_UART_Transmit(&huart2,(uint8_t*)nb_uart_tx_buf,nb_rx_count,0xff);
    nb_rx_count = 0;
    memset(&nb_uart_tx_buf, '\0', max_uart_len);
    NB_End_Flag = 0;
  }

  // uart2 information(Debug Uart)
  if (Debug_End_Flag && Debug_Uart_Cnt == 0)
  {
    //printf("debug UART\r\n");
    HAL_UART_Transmit(&huart2,(uint8_t*)debug_uart_tx_buf,debug_rx_count,0xff);
    debug_rx_count = 0;
    memset(&debug_uart_tx_buf, '\0', max_uart_len);
    Debug_End_Flag = 0;
  }
}


void UART_SEND_PACKET(uint8_t* data,uint16_t len)
{
    HAL_UART_Transmit(&huart1, data, len, 0xff);
//    HAL_UART_Transmit(&huart2, data, len, 0xff);
}

u16 GetModBusCRC16(u8 *aucData, u8 iBytesCount)
{
    u8 wHi = 0;
    u8 wLo = 0;
    u16 wCRC = 0xFFFF;
    u8 i, j;
    u8 wCheck = 0;
    for (i = 0; i < iBytesCount; i++)
    {
        wCRC ^= aucData[i];
        for (j = 0; j < 8; j++)
        {
            wCheck = wCRC & 1;
            wCRC = wCRC >> 1;
            wCRC = wCRC & 0x7fff;
            if (wCheck == 1)
                wCRC = wCRC ^ 0xa001;
            wCRC = wCRC & 0xffff;
        }
    }
    wHi = wCRC / 256;
    wLo = wCRC % 256;
    wCRC = (wHi << 8) | wLo;
    return wCRC;
}
/**
 * @name: Modbus 读取寄存器数据03
 * @msg:
 * @param {u8} Addr
 * @param {u16} Reg
 * @param {u8} len
 * @return {*}
 */
void Modbus_Read_03_Tx(u8 Addr, u16 Reg, u8 len)
{
    u16 CRC_16;
    if (StrSystem.UART_TX_FLAG != SET)
    {
        return;
    }
    StrSystem.UART_TX_FLAG = RESET;
    memset(StrSystem.TxData, 0, sizeof(StrSystem.TxData));
    StrSystem.TxData[0] = Addr;
    StrSystem.TxData[1] = 0x03;
    StrSystem.TxData[2] = (Reg >> 8) & 0xFF;
    StrSystem.TxData[3] = Reg & 0xFF;
    StrSystem.TxData[4] = (len >> 8) & 0xFF;
    StrSystem.TxData[5] = len & 0xFF;
    CRC_16 = GetModBusCRC16(StrSystem.TxData, 6);
    StrSystem.TxData[6] = CRC_16 & 0xFF;
    StrSystem.TxData[7] = (CRC_16 >> 8) & 0xFF;
    UART_SEND_PACKET(StrSystem.TxData, 8);
    StrSystem.UART_TimeOut = GetSysTick();
    StrSystem.UART_RX_FLAG = RESET;
}
/**
定时器1ms定时
*/
u8 CompareSysTick(u32 Time, u16 TimeOut)
{
    if (Time > GetSysTick())
    {
        if (0xFFFFFFFF - Time + GetSysTick() > TimeOut)
        {
            return 1;
        }
    }
    else if (abs((int)(GetSysTick() - Time)) > TimeOut)
    {
        return 1;
    }
    return 0;
}
/**
数据校验
*/
u8 UART_Packet_Check(u8 *m_pData, u8 m_nlen)
{
    static u16 usCRC_16=0;
    if (m_nlen < 2)
        return 0;
    if (*m_pData != StrSystem.DeviceAddr && *m_pData != 0)
        return 0;
    usCRC_16 = GetModBusCRC16(m_pData, m_nlen - 2);
    if (usCRC_16 == ((*(m_pData + m_nlen - 1) << 8) + *(m_pData + m_nlen - 2)))
        return 1;
    return 0;
}
void ArrayToU32(u8 *m_pData, u32 *m_Value)
{
    *m_Value = 0;
    *m_Value |= *(m_pData);
    *m_Value <<= 8;
    *m_Value |= *(m_pData + 1);
    *m_Value <<= 8;
    *m_Value |= *(m_pData + 2);
    *m_Value <<= 8;
    *m_Value |= *(m_pData + 3);
}
void ArrayToU16(u8 *m_pData, u16 *m_Value)
{
    *m_Value = (*(m_pData) << 8) + *(m_pData + 1);
}


u8 Modbus_Read_03_Rx(void)
{
    // u16 CRC_16;
    // u8 Rxlen;
    // if (CompareSysTick(StrSystem.UART_TimeOut, 1000))
    // {
    //     return 1; // 超时
    // }
    // if (StrSystem.UART_RX_FLAG != SET)
    // {
    //     return 2; // 暂未完成数据接收
    // }


    StrSystem.UART_RX_FLAG = RESET;
    // Rxlen = GET_UART_RECEIVCE_PACKET(StrSystem.RxData);
    if (UART_Packet_Check(StrSystem.RxData, nb_rx_count) == 0)
        return 3;
    if (StrSystem.RxData[1] != 0x03)
        return 4;
    // StrSystem.RxData[0]是地址
    // StrSystem.RxData[1]是03功能码
    // StrSystem.RxData[2]是字节数
    ArrayToU16(&StrSystem.RxData[3], &StrSystem.usValue);
    StrSystem.I = StrSystem.usValue / 100.0;
    ArrayToU32(&StrSystem.RxData[5], &StrSystem.ulValue);
    StrSystem.I = StrSystem.ulValue / 10000.0;
    ArrayToU32(&StrSystem.RxData[9], &StrSystem.ulValue);
    StrSystem.P = StrSystem.ulValue / 10000.0;
    ArrayToU32(&StrSystem.RxData[13], &StrSystem.ulValue);
    StrSystem.E = StrSystem.ulValue / 100.0;
    ArrayToU16(&StrSystem.RxData[17], &StrSystem.usValue);
    StrSystem.Pf = StrSystem.usValue / 1000.0;
    ArrayToU16(&StrSystem.RxData[19], &StrSystem.usValue);
    StrSystem.UFreq = StrSystem.usValue / 100.0;
    // 21~32预留
    ArrayToU32(&StrSystem.RxData[33], &StrSystem.ulValue);
    StrSystem.S = StrSystem.ulValue / 10000.0;
    ArrayToU32(&StrSystem.RxData[37], &StrSystem.ulValue);
    StrSystem.Q = StrSystem.ulValue / 10000.0;
    ArrayToU16(&StrSystem.RxData[41], &StrSystem.usValue);
    StrSystem.Phase = StrSystem.usValue / 100.0;

    printf("I:%.5f\r\n",StrSystem.I);
    printf("U:%.5f\r\n",StrSystem.P / StrSystem.I);
    printf("P:%.5f\r\n",StrSystem.P);
    printf("E:%.5f\r\n",StrSystem.E);
    printf("Pf:%.5f\r\n",StrSystem.Pf);
		return 0;
}
// int main()
// {
//     /*
//      */
//     StrSystem.UART_TX_FLAG = SET;
//     while (1)
//     {
//         StrSystem.DeviceAddr = 1;
//         Modbus_Read_03_Tx(StrSystem.DeviceAddr, 0x0048, 0x15);
//         if (Modbus_Read_03_Rx() != 2)
//         {
//             StrSystem.UART_TX_FLAG = SET;
//             delay1ms(500); // 自定义延时一段时间，建议不要直接使用延时方式
//         }
//     }
// }

void test()
{
    StrSystem.UART_TX_FLAG = SET;
    while (1)
    {
        if(Sensor_task_Flag == 1)
        {
            Sensor_task_Flag = 0;
            StrSystem.DeviceAddr = 1;
            Modbus_Read_03_Tx(StrSystem.DeviceAddr, 0x0048, 0x15);
        }
       processUartInfo();
    }
}