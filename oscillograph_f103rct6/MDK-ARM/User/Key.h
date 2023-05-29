#ifndef _KEY_H
#define _KEY_H

#include "main.h"

typedef struct _KEY_HANDLER
{
    void (*BUtton_OK_Pressed_Handler)(void);
    void (*BUtton_Cancel_Pressed_Handler)(void);
}Key_Handler;
#define Key_Ok_Prevent_Shake do{} while(HAL_GPIO_ReadPin(Button_OK_GPIO_Port,Button_OK_Pin)==GPIO_PIN_RESET)
#define Key_Cancel_Prevent_Shake do{} while(HAL_GPIO_ReadPin(Button_Cancel_GPIO_Port,Button_Cancel_Pin)==GPIO_PIN_RESET)
extern Key_Handler Key_handler;
void Key_Init(void);
void BUtton_OK_Pressed_Handler(void);
void BUtton_Cancel_Pressed_Handler(void);
void SetOk_Callback(void);
void Key_Reset_Ok_Handler(void);
void Key_Reset_Cancel_Handler(void);
#endif
