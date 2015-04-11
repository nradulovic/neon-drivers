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
 * @brief       Profile data for STM32Fxxx port
 *********************************************************************//** @{ */

/*=========================================================  INCLUDE FILES  ==*/

#include "mcu/profile.h"

/*=========================================================  LOCAL MACRO's  ==*/
/*======================================================  LOCAL DATA TYPES  ==*/
/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/
/*=======================================================  LOCAL VARIABLES  ==*/
/*======================================================  GLOBAL VARIABLES  ==*/

#if (NPROFILE_EN_GPIO)
const struct np_dev *                   g_gpios[] =
{
#if (NPROFILE_EN_GPIO & NP_EN_MAJOR(1))
    &g_gpioa,
#endif
#if (NPROFILE_EN_GPIO & NP_EN_MAJOR(2))
    &g_gpiob,
#endif
#if (NPROFILE_EN_GPIO & NP_EN_MAJOR(3))
    &g_gpioc,
#endif
    NULL
};
#endif /* NPROFILE_EN_GPIO */

#if (NPROFILE_EN_UART)
const struct np_dev *                   g_uarts[] =
{
#if (NPROFILE_EN_UART & NP_EN_MAJOR(1))
    &g_uart1,
#endif
#if (NPROFILE_EN_UART & NP_EN_MAJOR(2))
    &g_uart2,
#endif
#if (NPROFILE_EN_UART & NP_EN_MAJOR(6))
    &g_uart6,
#endif
    NULL
};
#endif /* NPROFILE_EN_UART */

/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/
/*===========================================  GLOBAL FUNCTION DEFINITIONS  ==*/
/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of profile.c
 ******************************************************************************/
