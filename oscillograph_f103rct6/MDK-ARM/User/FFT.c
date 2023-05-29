#include "FFT.h"
#include "math.h"
#include "app.h"
#define START_INDEX 10
#define FFT_N 256
void FFT(compx *xin)
{
    static int16_t f, m, nv2, nm1, i, k, l, j = 0;
    static compx u, w, t;
    f = m = nv2 = nm1 = i = k =  l = j = 0;
    nv2 = FFT_N / 2;
    nm1 = FFT_N - 1;
    for (i = 0; i < nm1; i++)
    {
        if (i < j)
        {
            t = xin[j];
            xin[j] = xin[i];
            xin[i] = t;
        }
        k = nv2;
        while (k <= j)
        {
            j = j - k;
            k = k / 2;
        }
        j = j + k;
    }
    static int16_t le, lei, ip;
    le = 0, lei = 0, ip = 0;
    f = FFT_N;
    for (l = 1; (f = f / 2) != 1; l++)
        ;
    for (m = 1; m <= l; m++)
    {
        le = 2 << (m - 1);
        lei = le / 2;
        u.real = 1.0;
        u.imag = 0.0;
        w.real = cos(PI / lei);
        w.imag = -sin(PI / lei);
        for (j = 0; j <= lei - 1; j++)
        {
            for (i = j; i <= FFT_N - 1; i = i + le)
            {
                ip = i + lei;
                t = EE(xin[ip], u);
                xin[ip].real = xin[i].real - t.real;
                xin[ip].imag = xin[i].imag - t.imag;
                xin[i].real = xin[i].real + t.real;
                xin[i].imag = xin[i].imag + t.imag;
            }
            u = EE(u, w);
        }
    }
}
compx EE(compx a, compx b)
{
    static compx c = {0.0, 0.0};
    c.real = a.real * b.real - a.imag * b.imag;
    c.imag = a.real * b.imag + a.imag * b.real;
    return (c);
}
int16_t find_max_num_index(compx *data, int16_t count)
{
    static int16_t i = START_INDEX;
    static int16_t max_num_index = 0;
    max_num_index = i;
    static float temp = 0;
    temp = data[i].real;
    for (i = START_INDEX; i < count; i++)
    {
        if (temp < data[i].real)
        {
            temp = data[i].real;
            max_num_index = i;
        }
    }
    return max_num_index;
}
int16_t dc_filter(int16_t input, DC_FilterData *df)
{
    static float new_w;
    new_w = input + df->w * df->a;
    static int16_t result;
    result = 5 * (new_w - df->w);
    df->w = new_w;
    return result;
}
int16_t bw_filter(int16_t input, BW_FilterData *bw)
{
    bw->v0 = bw->v1;
    bw->v1 = (1.241106190967544882e-2 * input) + (0.97517787618064910582 * bw->v0);
    return bw->v0 + bw->v1;
}
