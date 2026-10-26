#include "safety.h"

Safety_Fault Safety_Check(const Kobuki_Typedef *k)
{
    if (k->wheeldrop != 0U) return SAFETY_WHEEL_DROP;
    return SAFETY_OK; // Default ok
}
