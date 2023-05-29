#include "Key.h"
#include "app.h"
#include "stdio.h"

Key_Handler Key_handler;
void Key_Init(void)
{
    APP.Key_handler = &Key_handler;
    Key_handler.BUtton_Cancel_Pressed_Handler = BUtton_Cancel_Pressed_Handler;
    Key_handler.BUtton_OK_Pressed_Handler = BUtton_OK_Pressed_Handler;
}

void BUtton_Cancel_Pressed_Handler(void)
{
    printf("cancel button pressed\n");
}
void BUtton_OK_Pressed_Handler(void)
{
    printf("ok button pressed\n");
}
void Key_Reset_Ok_Handler(void)
{
    Key_handler.BUtton_OK_Pressed_Handler = BUtton_OK_Pressed_Handler;
}
void Key_Reset_Cancel_Handler(void)
{
    Key_handler.BUtton_Cancel_Pressed_Handler = BUtton_Cancel_Pressed_Handler;
}
