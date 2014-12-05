/* 
 * File:   newfile.h
 * Author: nenad
 *
 * Created on 04 December 2014, 00:21
 */

#ifndef PERIPHERAL_H_
#define	PERIPHERAL_H_

#include <stdint.h>

#include "device/peripheral.h"

#ifdef	__cplusplus
extern "C" {
#endif

#define periph_base_address(periph)     (periph)->base_address

#define periph_id(periph)               (periph)->id

#define periph_flags(periph)            (periph)->flags

struct nisr;
struct npower;
struct nclock;

struct nperipheral
{
    uint32_t                    id;
    uint32_t                    flags;
    const struct nperipheral *  host;
#if (ARCH_IO_ADDRESS == 1)
    volatile unsigned int *     base_address;
#endif
#if (ARCH_ATTR_MULTI_ISR == 1)
    const struct nisr *         isr;
#endif
#if (ARCH_ATTR_POWER_AVARE == 1)
    const struct npower *       power;
#endif
#if (ARCH_ATTR_CLOCK == 1)
    const struct nclock *       clock;
#endif
    const void *                variations;
};



#ifdef	__cplusplus
}
#endif

#endif	/* PERIPHERAL_H_ */

