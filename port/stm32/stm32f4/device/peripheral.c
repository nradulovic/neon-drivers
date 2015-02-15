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
 *********************************************************************//** @{ */
/**@defgroup    def_impl Implementation
 * @brief       Default Implementation
 * @{ *//*--------------------------------------------------------------------*/

/*=========================================================  INCLUDE FILES  ==*/

#include "mcu/peripheral.h"

/*=========================================================  LOCAL MACRO's  ==*/
/*======================================================  LOCAL DATA TYPES  ==*/
/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/
/*=======================================================  LOCAL VARIABLES  ==*/
/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/
/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/

void nperiph_clock_enable(const struct nperiph * periph)
{
	const struct nclock * 		clock = periph->clock;

	*clock->reg |= clock->mask;
}

void nperiph_clock_disable(const struct nperiph * periph)
{
	const struct nclock * 		clock = periph->clock;

	*clock->reg &= ~clock->mask;
}

void nperiph_isr_enable(const struct nperiph * periph, uint32_t irq_enum)
{
	const struct nisr * 		isr_array = periph->isr;

	NVIC_EnableIRQ(isr_array[irq_enum].irqn);
}

void nperiph_isr_disable(const struct nperiph * periph, uint32_t irq_enum)
{
	const struct nisr * 		isr_array = periph->isr;

	NVIC_DisableIRQ(isr_array[irq_enum].irqn);
}

void nperiph_isr_clear_flag(const struct nperiph * periph, uint32_t irq_enum)
{
	const struct nisr *			isr_array = periph->isr;

	NVIC_ClearPendingIRQ(isr_array[irq_enum].irqn);
}

void nperiph_isr_set_flag(const struct nperiph * periph, uint32_t irq_enum)
{
	const struct nisr *			isr_array = periph->isr;

	NVIC_SetPendingIRQ(isr_array[irq_enum].irqn);
}

void nperiph_isr_set_prio(const struct nperiph * periph, uint32_t irq_num, uint32_t prio)
{
	const struct nisr * 		isr_array = periph->isr;

	NVIC_SetPriority(isr_array[irq_num].irqn, prio);
}

uint32_t nperiph_isr_get_prio(const struct nperiph * periph, uint32_t irq_num)
{
	const struct nisr * 		isr_array = periph->isr;

	return (NVIC_GetPriority(isr_array[irq_num].irqn));
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of peripheral.c
 ******************************************************************************/
