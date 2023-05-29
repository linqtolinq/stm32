#ifndef _DECODE_H
#define _DECODE_H
#include "main.h"
#include "tftled.h"
#define DecodeC_Prevent_Shake do{} while(HAL_GPIO_ReadPin(DecodeC_GPIO_Port,DecodeC_Pin)==GPIO_PIN_RESET)
typedef struct _Decoder_Handle{
    void (*DecodeA_Callback)(void);
    void (*DecodeC_Callback)(void);
}Decoder_Handle;

typedef struct _Update_Window{
    uint32_t* UpdataValue;
    uint32_t update_value_temp;
    uint8_t Cursor_Position;
    uint8_t MaxCursor_Offset;
    uint8_t Visibility;
     void (*SetOk_Callback)(void);
     void (* Refresh_Update_Windows) (void);
}Update_Window;
extern Decoder_Handle Decodee_handle;
extern Update_Window Update_Windows;
void DrawUpdateDataWindows(uint32_t * out_value,uint8_t * title,uint8_t title_len,void (*CB)(void));
void DecodeA_Callback(void);
void DecodeC_Callback(void);
void Decode_Init(void);
void Refresh_Update_Windows(void);
void DeFault_Methode_Reset_CB(void);
#endif

