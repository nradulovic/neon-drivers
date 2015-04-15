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
 * @brief       Brief description
 *********************************************************************//** @{ */
/**@defgroup    def_impl Implementation
 * @brief       Default Implementation
 * @{ *//*--------------------------------------------------------------------*/

/*=========================================================  INCLUDE FILES  ==*/

#include "mcu/peripheral.h"
#include "mcu/profile.h"

/* NOTE:
 * The condition will turn off this module if not enabled or not available in
 * the current port. This kind of if/endif protection is necessary because this
 * source file may define unwanted ISRs.
 */
#if (NPROFILE_EN_GPIO)
#include "base/debug.h"
#include "base/component.h"
#include "mcu/gpio.h"

/*=========================================================  LOCAL MACRO's  ==*/
/*======================================================  LOCAL DATA TYPES  ==*/
/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/
/*=======================================================  LOCAL VARIABLES  ==*/

static const NCOMPONENT_DEFINE("STM32Fxxx GPIO driver", "Nenad Radulovic");

/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/
/*===========================================  GLOBAL FUNCTION DEFINITIONS  ==*/

void ngpio_init(uint32_t gpio_id, uint32_t config)
{
	GPIO_InitTypeDef			gpio_init;
	const struct npdev *		pdev;
	struct npdrv *				pdrv;
	uint32_t					port;
	uint32_t					pin;

	pdrv = npdrv_request(gpio_id);

	NREQUIRE(NAPI_USAGE "Invalid gpio_id.", pdev != NULL);

	port = NP_DEV_ID_TO_MAJOR(gpio_id);
	pin  = NP_DEV_ID_TO_MINOR(gpio_id);

	gpio_init.Pin = (0x1u << pin);
	gpio_init.Speed = GPIO_SPEED_FAST;

	switch (config & NGPIO_MODE) {
		case NGPIO_INPUT: {
			gpio_init.Mode = GPIO_MODE_INPUT;
			break;
		}
		case NGPIO_OUTPUT_HIGH:
		case NGPIO_OUTPUT_LOW: {
			gpio_init.Mode = GPIO_MODE_OUTPUT_PP;
			break;
		}
		case NGPIO_OUTPUT_OPEN_DRAIN_FLOAT:
		case NGPIO_OUTPUT_OPEN_DRAIN_LOW: {
			gpio_init.Mode = GPIO_MODE_OUTPUT_OD;
			break;
		}
		default: {
			NASSERT_ALWAYS(NAPI_USAGE "Invalid mode argument.");

			return;
		}
	}

	switch (config & NGPIO_PULL) {
		case NGPIO_PULL_NONE : {
			gpio_init.Pull = GPIO_NOPULL;
			break;
		}
		case NGPIO_PULL_DOWN : {
			gpio_init.Pull = GPIO_PULLDOWN;
			break;
		}
		case NGPIO_PULL_UP: {
			gpio_init.Pull = GPIO_PULLUP;
			break;
		}
		default: {
			NASSERT_ALWAYS(NAPI_USAGE "Invalid pull argument.");

			return;
		}
	}

}



void ngpio_term(uint32_t gpio_id)
{
	const struct npdev * 		pdev;
	uint32_t					pin;

	pdev = nprofile_pdev(gpio_id);

	NREQUIRE(NAPI_USAGE "Invalid gpio_id.", pdev != NULL);

	pin  = NP_DEV_ID_TO_MINOR(gpio_id);

	HAL_GPIO_DeInit((GPIO_TypeDef *)npdev_address(pdev), 0x1 << pin);
	npdrv_release(npdev_to_pdrv(pdev));
}



bool ngpio_is_set(uint32_t gpio_id)
{
	const struct npdev * 		pdev;
	uint32_t					pin;

	pdev = nprofile_pdev(gpio_id);

	NREQUIRE(NAPI_USAGE "Invalid gpio_id.", pdev != NULL);

	pin  = NP_DEV_ID_TO_MINOR(gpio_id);

	if (HAL_GPIO_ReadPin((GPIO_TypeDef *)npdev_address(pdev), 0x1 << pin) == GPIO_PIN_SET) {

		return (true);
	}

	return (false);
}



void ngpio_set(uint32_t gpio_id)
{
	const struct npdev * 		pdev;
	uint32_t					pin;

	pdev = nprofile_pdev(gpio_id);

	NREQUIRE(NAPI_USAGE "Invalid gpio_id.", pdev != NULL);

	pin  = NP_DEV_ID_TO_MINOR(gpio_id);

	HAL_GPIO_WritePin((GPIO_TypeDef *)npdev_address(pdev), 0x1 << pin, GPIO_PIN_SET);
}



void ngpio_clear(uint32_t gpio_id)
{
	const struct npdev * 		pdev;
	uint32_t					pin;

	pdev = nprofile_pdev(gpio_id);

	NREQUIRE(NAPI_USAGE "Invalid gpio_id.", pdev != NULL);

	pin  = NP_DEV_ID_TO_MINOR(gpio_id);

	HAL_GPIO_WritePin((GPIO_TypeDef *)npdev_address(pdev), 0x1 << pin, GPIO_PIN_RESET);
}



void ngpio_toggle(uint32_t gpio_id)
{
	const struct npdev * 		pdev;
	uint32_t					pin;

	pdev = nprofile_pdev(gpio_id);

	NREQUIRE(NAPI_USAGE "Invalid gpio_id.", pdev != NULL);

	pin  = NP_DEV_ID_TO_MINOR(gpio_id);

	HAL_GPIO_TogglePin((GPIO_TypeDef *)npdev_address(pdev), 0x1 << pin);
}



bool ngpio_request(uint32_t gpio_id);
void ngpio_release(uint32_t gpio_id);
bool ngpio_change_notice_request(uint32_t gpio_id, enum ngpio_trigger trigger, ngpio_change_handler * change_handler)
{

}



bool ngpio_change_notice_release(uint32_t gpio_id)
{

}



bool ngpio_is_id_valid(uint32_t gpio_id)
{
	if ((NP_DEV_ID_TO_CLASS(gpio_id) == NPROFILE_CLASS_GPIO) &&
	    (nprofile_pdev(gpio_id) != NULL)) {
		return (true);
	} else {
		return (false);
	}
}

#endif /* NPROFILE_EN_GPIO */
/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of p_gpio.c
 ******************************************************************************/
