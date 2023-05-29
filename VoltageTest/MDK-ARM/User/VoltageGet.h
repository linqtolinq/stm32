#ifndef _VOLTAGE_GET_H
#define _VOLTAGE_GET_H

#include <stdlib.h>
#include <stdio.h>
#include "main.h"
#include<string.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;

#define  GetSysTick() HAL_GetTick()


typedef struct
{
    u8 DeviceAddr; // 设备地址
    u8 UART_RX_FLAG : 1;
    u8 UART_TX_FLAG : 1;
    u8 : 6;
    u16 usValue;
    u32 ulValue;
    u32 UART_TimeOut; // 串口接收超时
    u8 TxData[20];    // 串口发送数据
    u8 RxData[80];    // 串口接收数据
    float U;          // 电压
    float I;          // 电流
    float P;          // 有功功率
    float E;          // 电能
    float Pf;         // 功率因数
    float UFreq;      // 电压频率
    float S;          // 视在功率
    float Q;          // 无功功率
    float Phase;      // 相位角
} SystemTypeDef;

extern  SystemTypeDef StrSystem;


extern uint16_t NB_Uart_Cnt;
extern uint16_t Debug_Uart_Cnt;

extern uint16_t NB_End_Flag;
extern uint16_t Debug_End_Flag;


void inc_sysTick(void);
void processUartInfo(void);
void Modbus_Read_03_Tx(u8 Addr, u16 Reg, u8 len);
u8 Modbus_Read_03_Rx(void);
void test();
#endif