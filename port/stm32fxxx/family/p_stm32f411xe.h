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
 * @brief       Profile data for STM32F411XE
 *********************************************************************//** @{ */

#ifndef FAMILY_STM32F411XE_H_
#define FAMILY_STM32F411XE_H_

/*=========================================================  INCLUDE FILES  ==*/

#include "stm32f4xx.h"

/*===============================================================  MACRO's  ==*/

#define NPROFILE_MAX_CPU_CLOCK          (100ul * 1000000ul)
#define NPROFILE_RAM_SIZE               (128ul * 1024ul)

#define NPROFILE_AVAILABLE_GPIO         NPROFILE_EN_1 | NPROFILE_EN_2 | NPROFILE_EN_3
#define NPROFILE_AVAILABLE_UART         0
#define NPROFILE_AVAILABLE_SPI			0
#define NPROFILE_AVAILABLE_I2C			0

/*-------------------------------------------------------  C++ extern base  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/
/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/
/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of p_stm32f411xe.h
 ******************************************************************************/
#endif /* FAMILY_STM32F411XE_H_ */
