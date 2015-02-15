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
#include "mcu/profile.h"
#include "mcu/gpio.h"
#include "stm32f4xx.h"

/*=========================================================  LOCAL MACRO's  ==*/
/*======================================================  LOCAL DATA TYPES  ==*/
/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/
/*=======================================================  LOCAL VARIABLES  ==*/
/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/

static const struct nperiph * port_no_periph(uint32_t port_no)
{
	return (&g_gpios[port_no]);
}

/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/

void ngpio_mux_init(
	uint32_t 					pin_id,
	const struct nmux * 		mux)
{
	const struct nperiph * 		periph;
	GPIO_InitTypeDef			gpio_config;

	periph = port_no_periph(NGPIO_PIN_ID_TO_PORT(pin_id));
	nperiph_clock_enable(periph);
	gpio_config.Alternate = mux->af;
	gpio_config.Mode      = mux->mode;
	gpio_config.Pin		  = NGPIO_PIN_ID_TO_PIN(pin_id);
	gpio_config.Pull	  = mux->pull;
	gpio_config.Speed	  = GPIO_SPEED_FAST;

	HAL_GPIO_Init((GPIO_TypeDef *)periph_address(periph), &gpio_config);
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of gpio_device.c
 ******************************************************************************/
