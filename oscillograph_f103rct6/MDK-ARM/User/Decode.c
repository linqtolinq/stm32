#include "Decode.h"
#include "stdio.h"
#include "app.h"

Decoder_Handle Decodee_handle;
Update_Window Update_Windows;
void Decode_Init(void)
{
    Decodee_handle.DecodeA_Callback = DeFault_Methode_Reset_CB;
    Decodee_handle.DecodeC_Callback = DeFault_Methode_Reset_CB;
}
uint32_t Pow_Int(uint8_t base, uint8_t exp)
{
    uint32_t value = 1;
    for (uint8_t i = 0; i < exp; i++)
        value *= base;
    return value;
}
void DrawUpdateDataWindows(uint32_t * out_value,uint8_t * title,uint8_t title_len,void (*CB)(void))
{
    LCD_Fill(0, 0, lcddev.width, lcddev.height, WHITE);
    Decodee_handle.DecodeA_Callback = DecodeA_Callback;
    Decodee_handle.DecodeC_Callback = DecodeC_Callback;
    Update_Windows.SetOk_Callback = CB;
    Update_Windows.Refresh_Update_Windows = Refresh_Update_Windows;
    Update_Force_Color(GRAY);
    Update_Windows.MaxCursor_Offset = 6;
    // 000.000
    Update_Windows.Cursor_Position = 3;
    LCD_DrawRectangle(20, 20, lcddev.width - 20, lcddev.height - 60);
    Show_Str(20 + (lcddev.width - 8 * (title_len - 1) - 40) / 2, 25, BLUE, WHITE, title, 16, 0);
    Update_Windows.UpdataValue = out_value;

    // TODO: update
    // *(Update_Windows.UpdataValue) = 0;

    Update_Windows.Visibility = 1;
    APP.Key_handler->BUtton_OK_Pressed_Handler = Update_Windows.SetOk_Callback;
    APP.Window_Resresh_Handler = Update_Windows.Refresh_Update_Windows;
    Key_Reset_Cancel_Handler();
}
void Refresh_Update_Windows(void)
{
    static uint8_t cursor_po = 0;
    static uint8_t data1 = 0;
    static uint8_t data2 = 0;
    static uint8_t data3 = 0;
    static uint8_t data4 = 0;
    static uint8_t data5 = 0;
    static uint8_t data6 = 0;
    if (Update_Windows.Visibility == 1)
    {
        data1 = (*(Update_Windows.UpdataValue)) % 1000000 / 100000;
        data2 = (*(Update_Windows.UpdataValue)) % 100000 / 10000;
        data3 = (*(Update_Windows.UpdataValue)) % 10000 / 1000;

        data4 = (*(Update_Windows.UpdataValue)) % 1000 / 100;
        data5 = (*(Update_Windows.UpdataValue)) % 100 / 10;
        data6 = (*(Update_Windows.UpdataValue)) % 10;

        LCD_ShowChar(30, 45, BLUE, WHITE, data1 + '0', 16, 0);
        LCD_ShowChar(45, 45, BLUE, WHITE, data2 + '0', 16, 0);
        LCD_ShowChar(60, 45, BLUE, WHITE, data3 + '0', 16, 0);
        LCD_ShowChar(75, 45, BLUE, WHITE, '.', 16, 0);
        LCD_ShowChar(90, 45, BLUE, WHITE, data4 + '0', 16, 0);
        LCD_ShowChar(105, 45, BLUE, WHITE, data5 + '0', 16, 0);
        LCD_ShowChar(120, 45, BLUE, WHITE, data6 + '0', 16, 0);

        cursor_po = Update_Windows.Cursor_Position > 3 ? Update_Windows.Cursor_Position + 1 : Update_Windows.Cursor_Position;

        Update_Force_Color(WHITE);

        LCD_DrawLine(30 - 2, 61, 30 + 7 * 15 - 7, 61);
        LCD_DrawLine(30 - 2, 62, 30 + 7 * 15 - 7, 62);

        Update_Force_Color(RED);
        LCD_DrawLine(30 + (cursor_po - 1) * 15 - 2, 61, 30 + cursor_po * 15 - 7, 61);
        LCD_DrawLine(30 + (cursor_po - 1) * 15 - 2, 62, 30 + cursor_po * 15 - 7, 62);
    }
}

void DecodeA_Callback(void)
{
    static uint32_t temp = 0;
    temp = Pow_Int(10, 6 - Update_Windows.Cursor_Position);
    printf("add temp %d\n", temp);
    if (HAL_GPIO_ReadPin(DecodeB_GPIO_Port, DecodeB_Pin) == GPIO_PIN_SET)
    {
        *(Update_Windows.UpdataValue) = *(Update_Windows.UpdataValue) +
                                                    temp <
                                                1000000
                                            ? *(Update_Windows.UpdataValue) +
                                                  temp
                                            : *(Update_Windows.UpdataValue);
    }
    else if (HAL_GPIO_ReadPin(DecodeB_GPIO_Port, DecodeB_Pin) == GPIO_PIN_RESET)
    {
        *(Update_Windows.UpdataValue) = *(Update_Windows.UpdataValue) > temp
                                            ? *(Update_Windows.UpdataValue) - temp
                                            : *(Update_Windows.UpdataValue);
    }
    printf("add value %d\n", *(Update_Windows.UpdataValue));
}
void DecodeC_Callback(void)
{
    while (HAL_GPIO_ReadPin(DecodeC_GPIO_Port, DecodeC_Pin) == GPIO_PIN_RESET)
        ;
    Update_Windows.Cursor_Position += 1;
    Update_Windows.Cursor_Position %= 7;
    if (Update_Windows.Cursor_Position == 0)
        Update_Windows.Cursor_Position = 1;
}
void SetOk_Callback(void)
{
    Update_Windows.Visibility = 0;
    Update_Windows.UpdataValue = &(Update_Windows.update_value_temp);
    DAC_Generate_Sin(APP.Sin_Wave_Frequency);
    Reset_APP_Window_Resresh_Handler();
    Key_Reset_Ok_Handler();

    Main_Windows_Init();
}

void DeFault_Methode_Reset_CB(void)
{
    // use to reset callback function
}

