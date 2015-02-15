/* 
 * File:   clock.h
 * Author: nenad
 *
 * Created on 05 December 2014, 01:20
 */

#ifndef CLOCK_H
#define	CLOCK_H

#include <stdint.h>

#include "device/clock.h"

#ifdef	__cplusplus
extern "C" {
#endif

void clock_enable(const struct nclock * clock);

uint32_t clock_get_frequency(const struct nclock * clock);


#ifdef	__cplusplus
}
#endif

#endif	/* CLOCK_H */

