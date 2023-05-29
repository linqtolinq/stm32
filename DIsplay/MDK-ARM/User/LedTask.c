#include "LedTask.h"
#include "Task.h"
#include "stdio.h"
#define scale 35.0

u8 O_x = 16; //??
u8 O_y = 220;
// 0-255 ->ch1   256->511 ch2
float Data[CH_LEN * 2] = {0};
float Data_old[CH_LEN * 2] = {0};

float Chh1_youxiao = 0;
float Chh1_T = 0;
float Chh1_Max = 0;
float Chh1_W = 0;

float Chh2_youxiao = 0;
float Chh2_T = 0;
float Chh2_Max = 0;
float Chh2_W = 0;


#define DATA_LENGTH 1000  
#define THRESHOLD 0.95    



void calculate_period(void)
{
  u16 cycle_start1 = 0;  
  u16 cycle_end1 = 0;    
  u16 cycle_count1 = 0;  
  u16 sample_count1 = 0; 


  u16 cycle_start2 = 0;  
  u16 cycle_end2 = 0;    
  u16 cycle_count2 = 0;  
  u16 sample_count2 = 0; 


  
  for (int i = 1; i < CH_LEN; i++)
  {
    if (Data[i*2] >= THRESHOLD && Data[(i - 1) * 2] < THRESHOLD)
    {
      cycle_start1 = i;
      break;
    }

	if (Data[i*2 + 1] >= THRESHOLD && Data[(i - 1) * 2 + 1] < THRESHOLD)
    {
      cycle_start2 = i;
      break;
    }
  }

  for (int i = cycle_start1 + 1; i < CH_LEN; i++)
  {
    if (Data[i*2] >= THRESHOLD && Data[(i - 1) * 2] < THRESHOLD)
    {
      cycle_end1 = i;
      cycle_count1++;
      sample_count1 += cycle_end1 - cycle_start1;
      cycle_start1 = cycle_end1;
    }
  }



  for (int i = cycle_start2 + 1; i < CH_LEN; i++)
  {
    if (Data[i*2 +1] >= THRESHOLD && Data[(i - 1) * 2 +1] < THRESHOLD)
    {
      cycle_end2 = i;
      cycle_count2++;
      sample_count2 += cycle_end2 - cycle_start2;
      cycle_start2 = cycle_end2;
    }
  }



  float avg_sample_count = (float)sample_count1 / cycle_count1;


  Chh1_T = avg_sample_count / 1000 / 25.0;
 avg_sample_count = (float)sample_count2 / cycle_count2;
Chh1_T = avg_sample_count / 1000 / 25.0;
}



void Data_Progress(void)
{
	Chh1_youxiao = 0;
	Chh2_youxiao = 0;
	Chh1_Max = 0;
	Chh2_Max = 0;
	calculate_period();
	for (u16 i = 0; i < CH_LEN; i++)
	{
		// Data[i] = (CH_new[i] / 2048.0 - 1) * scale;
		// Data[i] = (int)(Data[i] + 0.5);

		Data[i] = (CH_new[i * 2] / 4096.0) * 3.3 * scale;

		Chh1_Max = Chh1_Max < Data[i] ? Data[i] : Chh1_Max;
		Chh1_youxiao += Data[i];
		// Data[i] = (int)(Data[i] + 0.5);

		Data[CH_LEN + i] = (CH_new[i * 2 + 1] / 4096.0) * 3.3 * scale;

		Chh2_Max = Chh2_Max < Data[CH_LEN + i] ? Data[CH_LEN + i] : Chh2_Max;
		Chh2_youxiao += Data[CH_LEN + i];

		// Data[CH_LEN + i] = (int)(Data[CH_LEN + i] + 0.5);

		// printf("%d,", CH_new[i * 2]);
		// printf("%d,", CH_new[i * 2 + 1]);
	}
	Chh1_youxiao = Chh1_youxiao / CH_LEN;
	Chh2_youxiao = Chh2_youxiao / CH_LEN;
}

uint8_t Model = 2;
uint8_t Model_change = 0;

void Model_Transmit(void) // 模式切换
{
	if (Model < 2)
		Model++;
	else
		Model = 1;
	Model_change = 1;
}

void Drew_sin_wave_Tran(void) // 绘制正弦波形图
{

	u16 X1;
	u16 Y1;

	u16 X2;
	u16 Y2;

	u16 Y3;
	u16 X3;
	if (Model_change == 1)
	{
		Model_change = 0;
		LCD_SetColors(WHITE, WHITE);
		ILI9341_DrawRectangle(7, 68, 306, 166, 1);

		// LCD_SetColors(BLUE, WHITE);					   //??? ???(16,149)
		// ILI9341_DrawLine(16, 71, 16, 224);			   // Y?
		// ILI9341_DrawLine(16, 224 - 75, 309, 224 - 75); // X?
		// ILI9341_DrawLine(16, 71, 11, 81);			   // Y???
		// ILI9341_DrawLine(16, 71, 21, 81);
		// ILI9341_DispString_EN(25, 71, "Y");
		// ILI9341_DrawLine(309, 224 - 75, 299, 219 - 75); // X???
		// ILI9341_DrawLine(309, 224 - 75, 299, 229 - 75);
		// ILI9341_DispString_EN(301, 203 - 75, "X");
		// ILI9341_DrawRectangle(24, 84, 270, 140, 1);
	}
	if (Model == 0) // 模式0，两个通道同时显示
	{
		for (u16 i = 1; i < CH_LEN; i++)
		{
			X1 = O_x + i;
			X2 = O_x + i;

			Y1 = O_y - Data_old[i];
			Y2 = O_y - Data_old[CH_LEN + i];
			LCD_SetColors(WHITE, WHITE);
			ILI9341_SetPointPixel(X1, Y1);

			LCD_SetColors(WHITE, WHITE);
			ILI9341_SetPointPixel(X2, Y2);

			Data_old[i] = Data[i];
			Data_old[CH_LEN + i] = Data[CH_LEN + i];

			Y1 = O_y - Data[i];
			Y2 = O_y - Data[CH_LEN + i];
			LCD_SetColors(BLUE, WHITE);
			ILI9341_SetPointPixel(X1, Y1);
			LCD_SetColors(RED, WHITE);
			ILI9341_SetPointPixel(X2, Y2);
		}
	}
	else if (Model == 1) // 模式1，只显示通道CH1
	{
		for (u16 i = 1; i < CH_LEN; i++)
		{
			X1 = O_x + i - 1;
			Y1 = O_y - Data_old[i - 1];

			X3 = O_x + i;
			Y3 = O_y - Data_old[CH_LEN + i];
			// Y2 = O_y - Data_old[CH_LEN + i];
			LCD_SetColors(WHITE, WHITE);
			ILI9341_DrawLine(X1, Y1,X3,Y3);
			// LCD_SetColors(WHITE, WHITE);
			// ILI9341_SetPointPixel(X2, Y2);

			Data_old[i - 1] = Data[i - 1];
			Data_old[CH_LEN + i - 1] = Data[CH_LEN + i - 1];


			Y3 = O_y - Data[i];
			Y1 = O_y - Data[i - 1];
			LCD_SetColors(BLUE, WHITE);
			ILI9341_DrawLine(X1, Y1,X3,Y3);
		}
	}
	else // 模式2，只显示通道CH2
	{
		for (u16 i = 1; i < CH_LEN; i++)
		{
			X2 = O_x + i -1;
			Y2 = O_y - Data_old[CH_LEN + i - 1];

			X3 = O_x + i;
			Y3 = O_y - Data_old[CH_LEN + i];

			LCD_SetColors(WHITE, WHITE);
			ILI9341_DrawLine(X2, Y2,X3,Y3);




			Data_old[i-1] = Data[i-1];
			Data_old[CH_LEN + i - 1] = Data[CH_LEN + i - 1];

			Y2 = O_y - Data[CH_LEN + i - 1];
			Y3 = O_y - Data[CH_LEN + i];
			LCD_SetColors(RED, WHITE);
			ILI9341_DrawLine(X2, Y2,X3,Y3);
		}
	}
}

u8 str[10] = {'0'};

void data_show(void)
{

	// ILI9341_DrawRectangle(32, 11, 40, 16, 1);  // U
	// ILI9341_DrawRectangle(40, 41, 40, 16, 1);  // Um
	// ILI9341_DrawRectangle(134, 41, 40, 16, 1); // T
	// ILI9341_DrawRectangle(237, 41, 40, 16, 1); // f

	if (Model == 1)
	{
		LCD_SetColors(GREEN, WHITE);
		ILI9341_DrawRectangle(110, 8, 100, 27, 0);

		// LCD_SetColors(BLUE, WHITE);
		// ILI9341_DispString_EN(113, 11, "CH1:");
		// ILI9341_DrawLine(150, 20, 200, 20);

		LCD_SetColors(WHITE, WHITE);
		ILI9341_DrawRectangle(212, 8, 100, 27, 0);

		// LCD_SetColors(RED, WHITE);
		// ILI9341_DispString_EN(216, 11, "CH2:");
		// ILI9341_DrawLine(253, 20, 303, 20);

		LCD_SetFont(&Font8x16);
		LCD_SetColors(BLUE, WHITE); //??

		sprintf((char *)str, "%.4f", Chh1_youxiao / scale);
		ILI9341_DispString_EN(40, 11, (char *)str);

		sprintf((char *)str, "%.4f", Chh1_Max / scale);
		ILI9341_DispString_EN(40, 41, (char *)str);

		sprintf((char *)str, "%.3f", Chh1_T);
		ILI9341_DispString_EN(140, 41, (char *)str);

		sprintf((char *)str, "%.2f",  1.0 / Chh1_T);
		ILI9341_DispString_EN(240, 41,(char *)str);
	}
	else if (Model == 2)
	{
		LCD_SetColors(WHITE, WHITE);
		ILI9341_DrawRectangle(110, 8, 100, 27, 0);

		// LCD_SetColors(BLUE, WHITE);
		// ILI9341_DispString_EN(113, 11, "CH1:");
		// ILI9341_DrawLine(150, 20, 200, 20);

		LCD_SetColors(GREEN, WHITE);
		ILI9341_DrawRectangle(212, 8, 100, 27, 0);

		// LCD_SetColors(RED, WHITE);
		// ILI9341_DispString_EN(216, 11, "CH2:");
		// ILI9341_DrawLine(253, 20, 303, 20);

		LCD_SetFont(&Font8x16);
		LCD_SetColors(BLUE, WHITE); //??

		sprintf((char *)str, "%.4f", Chh2_youxiao / scale);
		ILI9341_DispString_EN(40, 11, (char *)str);

		sprintf((char *)str, "%.4f", Chh2_Max / scale);
		ILI9341_DispString_EN(40, 41, (char *)str);

		sprintf((char *)str, "%.3f",  Chh1_T);
		ILI9341_DispString_EN(140, 41, (char *)str);

		sprintf((char *)str, "%.2f", 1.0 / Chh1_T);
		ILI9341_DispString_EN(240, 41,(char *)str);
	}
}


