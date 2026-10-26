#include "nav_common.h"

int32_t Angle_Delta_cdeg(int16_t now, int16_t before)
{
    int32_t d = now - before;
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

int16_t Turn_To_Heading_Deg(int16_t current_cdeg, int16_t target_cdeg)
{
    int32_t d = Angle_Delta_cdeg(target_cdeg, current_cdeg);
    // round to the nearest degree, don't truncate
    return (int16_t)((d >= 0) ? (d + 50) / 100 : (d - 50) / 100);
}
