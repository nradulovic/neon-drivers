/*
 * This file is part of Neon.
 *
 * Copyright (C) 2010 - 2015 Nenad Radulovic
 *
 * Neon is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Neon is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Neon.  If not, see <http://www.gnu.org/licenses/>.
 *
 * web site:    http://github.com/nradulovic
 * e-mail  :    nenad.b.radulovic@gmail.com
 *//***********************************************************************//**
 * @file
 * @author      Nenad Radulovic
 * @brief       UART driver
 * @defgroup    uart UART driver
 * @brief       UART driver
 *********************************************************************//** @{ */

#ifndef NEON_DRIVERS_PERIPHERAL_H_
#define NEON_DRIVERS_PERIPHERAL_H_

/*=========================================================  INCLUDE FILES  ==*/

#include <stdint.h>

#include "device/peripheral.h"

/*===============================================================  MACRO's  ==*/

#define periph_id(periph)               (periph)->id
#define periph_class_id(periph)         (periph)->class_id
#define periph_flags(periph)            (periph)->flags
#define periph_driver(periph)           (periph)->driver
#define periph_address(periph)          (periph)->address
#define periph_mux(periph)              (periph)->mux

/*-------------------------------------------------------  C++ extern base  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

struct nisr;
struct npower;
struct nclock;
struct nmux;

struct nperiph
{
    uint32_t                    id;
    uint32_t                    class_id;
    uint32_t                    flags;
    void *                      driver;
    const struct nperiph *      host;
#if (ARCH_IO_ADDRESS == 1)
    volatile unsigned int *     address;
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
#if (ARCH_ATTR_MUX == 1)
    const struct nmux *         mux;
#endif
    const void *                variations;
};

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/

void nperiph_clock_enable(const struct nperiph * periph);
void nperiph_clock_disable(const struct nperiph * periph);

void nperiph_isr_enable(const struct nperiph * periph, uint32_t irq_enum);
void nperiph_isr_disable(const struct nperiph * periph, uint32_t irq_enum);
void nperiph_isr_clear_flag(const struct nperiph * periph, uint32_t irq_enum);
void nperiph_isr_set_flag(const struct nperiph * periph, uint32_t irq_enum);
void nperiph_isr_set_prio(const struct nperiph * periph, uint32_t irq_num, uint32_t prio);
uint32_t nperiph_isr_get_prio(const struct nperiph * periph, uint32_t irq_num);

/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of peripheral.h
 ******************************************************************************/
#endif  /* NEON_DRIVERS_PERIPHERAL_H_ */

