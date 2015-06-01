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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Neon. If not, see <http://www.gnu.org/licenses/>.
 *
 * web site: http://github.com/nradulovic
 * e-mail : nenad.b.radulovic@gmail.com
 *//***********************************************************************//**
 * @file
 * @author dejan
 * @brief 
 *********************************************************************//** @{ */

/*=========================================================  INCLUDE FILES  ==*/

#include "base/debug.h"
#include "base/component.h"
#include "family/p_mux.h"
#include "mcu/peripheral.h"
#include "mcu/profile.h"
#include "mcu/mux.h"
#include "mcu/gpio.h"

/*=========================================================  LOCAL MACRO's  ==*/
/*======================================================  LOCAL DATA TYPES  ==*/
/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/
/*=======================================================  LOCAL VARIABLES  ==*/

static const NCOMPONENT_DEFINE("STM32Fxxx GPIO driver");

/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/
/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/

void nmux_setup(const struct nmux * mux_array, size_t size)
{
	uint32_t 			mux_index;
	GPIO_InitTypeDef  	GPIO_InitStruct;
	struct npdrv *		pdrv;

	size /= sizeof(mux_array[0]);

	for (mux_index = 0; mux_index < size; mux_index++) {
	    pdrv = npdrv_request(mux_array[mux_index].gpio_id);

		NREQUIRE(NAPI_USAGE "Invalid gpio_id.", pdrv != NULL);

		npdrv_pwr_enable(pdrv, 0);

		GPIO_InitStruct.Pin       = (uint32_t)(1u << NGPIO_ID_TO_PIN(mux_array[mux_index].gpio_id));
		GPIO_InitStruct.Mode      = mux_array[mux_index].mode;
		GPIO_InitStruct.Pull      = mux_array[mux_index].pull;
		GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
		GPIO_InitStruct.Alternate = mux_array[mux_index].alternate;

	    HAL_GPIO_Init((GPIO_TypeDef *)npdrv_address(pdrv), &GPIO_InitStruct);
	}
}



void nmux_reset(uint32_t gpio_id)
{
	uint32_t 			mux_index;
	GPIO_InitTypeDef  	GPIO_InitStruct;
	struct npdrv *		pdrv;

	pdrv = npdrv_request(gpio_id);

	NREQUIRE(NAPI_USAGE "Invalid gpio_id.", pdrv != NULL);

	GPIO_InitStruct.Pin       = NGPIO_ID_TO_PIN(gpio_id);
	GPIO_InitStruct.Mode      = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull      = GPIO_NOPULL;
	GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;

	HAL_GPIO_Init((GPIO_TypeDef *)npdrv_address(pdrv), &GPIO_InitStruct);
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of p_mux.c
 ******************************************************************************/


