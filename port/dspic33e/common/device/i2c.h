/* 
 * File:   i2c.h
 * Author: nenad
 *
 * Created on November 30, 2014, 12:30 PM
 */

#ifndef COMMON_I2C_H
#define	COMMON_I2C_H

#include <stdint.h>

#include "port/isr.h"

#ifdef	__cplusplus
extern "C" {
#endif

#define I2C_ISR_SLAVE_EVENT     0
#define I2C_ISR_MASTER_EVENT    1

/* FIXME: name properly this errata */
#define I2C_ERRATA_01           (0x1u << 0)

#ifdef	__cplusplus
}
#endif

#endif	/* COMMON_I2C_H */

