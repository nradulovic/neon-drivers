/*
 * isr.h
 *
 *  Created on: Dec 15, 2014
 *      Author: nenad
 */

#ifndef DRIVERS_INCLUDE_MCU_ISR_H_
#define DRIVERS_INCLUDE_MCU_ISR_H_

#include "port/cpu.h"
#include "periph/isr.h"

struct nisr;

void nisr_enable(struct nisr * isr);
void nisr_disable(struct nisr * isr);
void nisr_set(struct nisr * isr);
void nisr_clear(struct nisr * isr);
void nisr_set_prio(struct nisr * isr, ncpu_reg prio);
ncpu_reg nisr_get_prio(struct nisr * isr);

#endif /* DRIVERS_INCLUDE_MCU_ISR_H_ */
