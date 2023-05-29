#ifndef __FONT_H
#define __FONT_H 	   

extern const unsigned char asc2_1206[95][12];
extern const unsigned char asc2_1608[95][16];
extern const unsigned char gImage_qq[3200];

typedef struct 
{
       unsigned char Index[2];	
       char Msk[32];
}typFNT_GB16; 
typedef struct 
{
       unsigned char Index[2];	
       char Msk[72];
}typFNT_GB24; 
typedef struct 
{
       unsigned char Index[2];	
       char Msk[128];
}typFNT_GB32; 



#endif

