/****************************************************************************************************   
//       SDAIN         PB15     
//       LED_BACK      PA8         
//       SCK           PB13         
//       RS/DC         PB5         
//       RST/SDAOUT    PB14         
//       CS            PB12        
*/

#ifndef __TFT_LED_H
#define __TFT_LED_H		
#include "main.h"	 
#include "stdlib.h"
#include "font.h"

typedef uint16_t u16;
typedef uint8_t u8;
typedef uint32_t u32;

typedef struct  
{										    
	u16 width;			
	u16 height;			
	u16 id;				  
	u8  dir;			  
	u16	 wramcmd;		
	u16  setxcmd;		
	u16  setycmd;		
}_lcd_dev; 	


extern _lcd_dev lcddev;	
/*
	  Pin header
	--********--
	|			|
	|           |
	|           |
	|           |
	|           |
	|-----------|
        logo
*/
/*
	* ------------>x
	|
	|
	y
*/
#define USE_HORIZONTAL_0  	 0 
/*
	y <------------ *
					        |
					        |
					        x
*/
#define USE_HORIZONTAL_1  	 1 
/*
					        y
					        |
	 				        |
	x <------------ *
*/
#define USE_HORIZONTAL_2 	 2 
/*
	x 
	|
	|
	* ------------->y
*/
#define USE_HORIZONTAL_3 	 3 
#define LCD_W 130
#define LCD_H 160
	   
extern u16  POINT_COLOR;   
extern u16  BACK_COLOR;

#define GPIO_TYPE  GPIOB  //GPIO×éÀàÐÍ
#define GPIO_BACK_PORT  GPIOA  //GPIO×éÀàÐÍ
#define GPIO_CLK_EN  do{__HAL_RCC_GPIOB_CLK_ENABLE() ; __HAL_RCC_GPIOC_CLK_ENABLE();__HAL_RCC_GPIOA_CLK_ENABLE();} while(0u)

// #define LED_BACK_PIN      GPIO_PIN_9        //±³¹â¿ØÖÆÒý½Å        PB9
// #define LCD_CS   GPIO_PIN_11       //Æ¬Ñ¡Òý½Å            PB11
// #define LCD_RS   GPIO_PIN_10       //¼Ä´æÆ÷/Êý¾ÝÑ¡ÔñÒý½Å PB10 
// #define LCD_RST  GPIO_PIN_12       //¸´Î»Òý½Å            PB12
// #define SPI_SCLK GPIO_PIN_13	//PB13--->>TFT --SCL/SCK
// #define SPI_MISO GPIO_PIN_14	
// #define SPI_MOSI GPIO_PIN_15	//PB15 MOSI--->>TFT --SDA/DIN


#define LED_BACK_PIN  GPIO_PIN_8
#define LCD_CS   GPIO_PIN_12      
#define LCD_RS   GPIO_PIN_6      
#define LCD_RST  GPIO_PIN_14       
#define SPI_SCLK GPIO_PIN_13	
#define SPI_MISO GPIO_PIN_14	
#define SPI_MOSI GPIO_PIN_15	


#define	SPI_MOSI_SET  	GPIO_TYPE->BSRR=SPI_MOSI    
#define	SPI_SCLK_SET  	GPIO_TYPE->BSRR=SPI_SCLK    




#define	SPI_MOSI_CLR  	GPIO_TYPE->BRR=SPI_MOSI    
#define	SPI_SCLK_CLR  	GPIO_TYPE->BRR=SPI_SCLK    

void  SPIv_WriteData(u8 Data);

#define delay_ms(x) HAL_Delay(x)


#define	LCD_LED_BACK_ON()  HAL_GPIO_WritePin(GPIO_BACK_PORT ,LED_BACK_PIN,GPIO_PIN_SET) 
#define	LCD_LED_BACK_OFF()  HAL_GPIO_WritePin(GPIO_BACK_PORT ,LED_BACK_PIN,GPIO_PIN_RESET) 


#define	LCD_CS_SET  GPIO_TYPE->BSRR=LCD_CS    
#define	LCD_RS_SET	GPIOC->BSRR=LCD_RS    
#define	LCD_RST_SET	GPIO_TYPE->BSRR=LCD_RST  

				    
#define	LCD_CS_CLR  GPIO_TYPE->BRR=LCD_CS
#define	LCD_RS_CLR	GPIOC->BRR=LCD_RS 
#define	LCD_RST_CLR	GPIO_TYPE->BRR=LCD_RST


#define WHITE       0xFFFF
#define BLACK      	0x0000	  
#define BLUE       	0x001F  
#define BRED        0XF81F
#define GRED 	    0XFFE0
#define GBLUE		0X07FF
#define RED         0xF800
#define MAGENTA     0xF81F
#define GREEN       0x07E0
#define CYAN        0x7FFF
#define YELLOW      0xFFE0
#define BROWN 		0XBC40 
#define BRRED 		0XFC07 
#define GRAY  		0X8430 


#define DARKBLUE     	0X01CF
#define LIGHTBLUE     	0X7D7C  
#define GRAYBLUE      	0X5458

 
#define LIGHTGREEN     	0X841F 
#define LIGHTGRAY       0XEF5B 
#define LGRAY 			0XC618 

#define LGRAYBLUE      	0XA651 
#define LBBLUE          0X2B12 


	    															  
void LCD_Init(void);
void LCD_Clear(u16 Color);	 
void LCD_SetCursor(u16 Xpos, u16 Ypos);
void LCD_DrawPoint(u16 x,u16 y);//»­µã  
void LCD_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd);								    
void LCD_WriteReg(u8 LCD_Reg, u16 LCD_RegValue);
void LCD_WR_DATA(u8 data);
void LCD_WriteRAM_Prepare(void);
void Lcd_WriteData_16Bit(u16 Data);
void LCD_direction(u8 direction );
void GUI_DrawPoint(u16 x,u16 y,u16 color);
void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color);
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);
void gui_circle(int xc, int yc,u16 c,int r, int fill);
void Draw_Triangel(u16 x0,u16 y0,u16 x1,u16 y1,u16 x2,u16 y2);
void Fill_Triangel(u16 x0,u16 y0,u16 x1,u16 y1,u16 x2,u16 y2);
void LCD_ShowChar(u16 x,u16 y,u16 fc, u16 bc, u8 num,u8 size,u8 mode);
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size);
void LCD_ShowString(u16 x,u16 y,u8 size,u8 *p,u8 mode);
void Show_Str(u16 x, u16 y, u16 fc, u16 bc, u8 *str,u8 size,u8 mode);
void Gui_Drawbmp16(u16 x,u16 y,const unsigned char *p); 
void gui_circle(int xc, int yc,u16 c,int r, int fill);
void Gui_StrCenter(u16 x, u16 y, u16 fc, u16 bc, u8 *str,u8 size,u8 mode);
void LCD_DrawFillRectangle(u16 x1, u16 y1, u16 x2, u16 y2);
void Update_Force_Color(u16 color);
void Update_Back_Color(u16 color);		  		 
#endif  
	 
	 



