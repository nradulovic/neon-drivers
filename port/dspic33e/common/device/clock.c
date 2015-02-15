
#include "mcu/clock.h"


uint32_t clock_get_frequency(const struct nclock * clock)
{
    return (clock->frequency);
}