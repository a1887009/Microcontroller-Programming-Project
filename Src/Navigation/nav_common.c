/* <student numbers here - required on first line of every .c/.h> */
/**
 * nav_common.c - angle maths, range windows and median.
 * Pure functions: no motor commands, no hardware access.
 */
#include "nav_common.h"

int32_t Angle_Delta_cdeg(int16_t now, int16_t before)
{
    int32_t d = (int32_t)now - (int32_t)before;
    if (d > 18000)       d -= 36000;
    else if (d < -18000) d += 36000;
    return d;
}

int16_t Wrap_cdeg(int32_t a)
{
    a %= 36000;
    if (a > 18000)       a -= 36000;
    else if (a < -18000) a += 36000;
    return (int16_t)a;
}

int16_t Wrap_Deg(int32_t deg)
{
    deg %= 360;
    if (deg > 180)       deg -= 360;
    else if (deg < -180) deg += 360;
    return (int16_t)deg;
}

int16_t Turn_To_Heading_Deg(int16_t current_cdeg, int16_t target_cdeg)
{
    int32_t d = Angle_Delta_cdeg(target_cdeg, current_cdeg);
    // round to the nearest degree, don't truncate
    return (int16_t)((d >= 0) ? (d + 50) / 100 : (d - 50) / 100);
}

uint16_t Window_Min(int a, int b, int tol)
{
    int lo = ((a < b) ? a : b) - tol;
    return (uint16_t)((lo < 0) ? 0 : lo);
}

uint16_t Window_Max(int a, int b, int tol)
{
    return (uint16_t)(((a > b) ? a : b) + tol);
}

uint16_t Median_mm(uint16_t *v, uint8_t n)
{
    for (uint8_t i = 1; i < n; i++) { //insersion sort
        uint16_t x = v[i];
        int8_t   j = (int8_t)(i - 1);
        while ((j >= 0) && (v[j] > x)) { v[j + 1] = v[j]; j--; }
        v[j + 1] = x;
    }
    return v[n / 2U];
}
