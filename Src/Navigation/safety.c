/*
 * safety.c - hardware safety checks.
 *
 */
#include "safety.h"

Safety_Fault Safety_Check(const Kobuki_Typedef *k)
{
    // Trigger for all non-zero
    if (k->wheeldrop != 0U) return SAFETY_WHEEL_DROP;
    if (k->bumper    != 0U) return SAFETY_BUMPER;
    if (k->cliff     != 0U) return SAFETY_CLIFF;
    return SAFETY_OK; // Default ok
}
