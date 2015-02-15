/*
 * This file is part of neon-test.
 *
 * Copyright (C) 2010 - 2015 nenad
 *
 * neon-test is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * neon-test is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with neon-test.  If not, see <http://www.gnu.org/licenses/>.
 *
 * web site:    
 * e-mail  :    
 *//***********************************************************************//**
 * @file
 * @author      nenad
 * @brief       Brief description
 * @defgroup    def_group Name
 * @brief       Brief description
 *********************************************************************//** @{ */

#ifndef DEVICE_PERIPHERAL_H_
#define DEVICE_PERIPHERAL_H_

/*=========================================================  INCLUDE FILES  ==*/

#include <stdint.h>

#include "stm32f4xx.h"

/*===============================================================  MACRO's  ==*/

#define ARCH_IO_ADDRESS                 1
#define ARCH_ATTR_MULTI_ISR             1
#define ARCH_ATTR_CLOCK                 1
#define ARCH_ATTR_MUX					1

/*-------------------------------------------------------  C++ extern base  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

struct nisr
{
	IRQn_Type					irqn;
};

struct nmux
{
	unsigned int 				af;
	unsigned int 				mode;
	unsigned int 				pull;
};

struct nclock
{
	struct nclock *				host;
	volatile uint32_t *			reg;
	unsigned int				mask;
};

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/
/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of peripheral.h
 ******************************************************************************/
#endif /* DEVICE_PERIPHERAL_H_ */
