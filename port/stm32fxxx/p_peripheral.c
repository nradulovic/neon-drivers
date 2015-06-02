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

/*=========================================================  INCLUDE FILES  ==*/

#include "mcu/peripheral.h"
#include "mcu/profile.h"

/*=========================================================  LOCAL MACRO's  ==*/
/*======================================================  LOCAL DATA TYPES  ==*/
/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/
/*=======================================================  LOCAL VARIABLES  ==*/
/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/
/*===========================================  GLOBAL FUNCTION DEFINITIONS  ==*/

void np_pwr_enable(const struct npdev_pwr * pwr)
{
    *(volatile uint32_t *)pwr->reg |= pwr->mask;
}

void np_pwr_disable(const struct npdev_pwr * pwr)
{
    *(volatile uint32_t *)pwr->reg &= ~pwr->mask;
}

void np_isr_enable(const struct npdev_isr * isr)
{
    NVIC_EnableIRQ(isr->irqn);
}

void np_isr_disable(const struct npdev_isr * isr)
{
    NVIC_DisableIRQ(isr->irqn);
}

void np_isr_clear_flag(const struct npdev_isr * isr)
{
    NVIC_ClearPendingIRQ(isr->irqn);
}

void np_isr_set_flag(const struct npdev_isr * isr)
{
    NVIC_SetPendingIRQ(isr->irqn);
}

void np_isr_set_prio(const struct npdev_isr * isr, uint32_t prio)
{
	prio = NCORE_LOCK_TO_CODE(prio);

    NVIC_SetPriority(isr->irqn, prio);
}

uint32_t np_isr_get_prio(const struct npdev_isr * isr)
{
    return (NVIC_GetPriority(isr->irqn));
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of p_dev.c
 ******************************************************************************/
