#ifndef _FFT_H
#define _FFT_H
#include "main.h"
#define PI 3.14159265//35897932384626433832795028841971
typedef struct _compx
{
    float real;
    float imag;
} compx;

compx EE( compx a,  compx b);

typedef struct
{
    float w;
    int16_t init;
    float a;

} DC_FilterData;

typedef struct
{
    float v0;
    float v1;
} BW_FilterData;
void FFT( compx *xin);
#endif
