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
 * @brief       Peripheral definitions for STM32Fxxx port
 *********************************************************************//** @{ */

#ifndef FAMILY_P_DEV_H_
#define FAMILY_P_DEV_H_

/*=========================================================  INCLUDE FILES  ==*/
/*===============================================================  MACRO's  ==*/

/* NOTE:
 * This port supports:
 *  - register access by common structure address
 *  - peripheral dynamic power enable/disable
 *  - dynamic ISR enable/disable
 *  - GPIO mux configuration
 *  - peripheral reset
 */
#define NP_ATTR_ADDRESS                 1
#define NP_ATTR_CLK                     1
#define NP_ATTR_ISR                     1
#define NP_ATTR_MUX                     1
#define NP_ATTR_RST                     0
#define NP_ATTR_PWR						1

/* NOTE:
 * This port defines some common ARCH_* macros
 */
#define ARCH_STM32_APB2                 0
#define ARCH_STM32_APB1                 1
#define ARCH_STM32_AHB1                 10
#define ARCH_STM32_AHB2					11

/*-------------------------------------------------------  C++ extern base  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

struct np_dev_clk
{
    unsigned int                clk_bus;
};

struct np_dev_isr
{
    unsigned int                irqn;
    unsigned int                flags;
};

struct np_dev_mux
{
    unsigned int                af;
    unsigned int                mode;
    unsigned int                pull;
};

struct np_dev_rst
{
    volatile void *             reg;
    unsigned int                mask;
};

struct np_dev_pwr
{
	volatile void *             reg;
	unsigned int                mask;
};

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/
/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of p_dev.h
 ******************************************************************************/
#endif /* FAMILY_P_DEV_H_ */
