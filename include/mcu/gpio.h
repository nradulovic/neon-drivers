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
 * @brief       GPIO operations
 * @defgroup    gpio_intf GPIO operations
 * @brief       GPIO operations
 *********************************************************************//** @{ */

#ifndef NEON_MCU_GPIO_H_
#define NEON_MCU_GPIO_H_

/*=========================================================  INCLUDE FILES  ==*/

#include <stdbool.h>
#include <stdint.h>

#include "mcu/peripheral.h"

#include "family/p_gpio.h"

/*===============================================================  MACRO's  ==*/

/**@brief       Convenience wrapper macro around @ref NP_DEV_CREATE_ID().
 * @param       port
 *              Specify port number in range 1 - 32, where GPIOA is 1 etc.
 * @param       pin
 *              Specify pin number in range 1 - 32
 * @return      Returns valid GPIO pin ID number which can be used with other
 *              functions and macros.
 */
#define NGPIO_ID(port, pin)             NP_DEV_CREATE_ID(NPROFILE_CLASS_GPIO, port, pin)

/**@brief       Convenience wrapper macro around @ref NP_DEV_ID_TO_MAJOR().
 * @param       id
 *              Valid GPIO pin ID
 * @return      Returns GPIO Port number
 */
#define NGPIO_ID_TO_PORT(id)            NP_DEV_ID_TO_MAJOR(id)

/**@brief       Convenience wrapper macro around @ref NP_DEV_ID_TO_MINOR().
 * @param       id
 *              Valid GPIO pin ID
 * @return      Returns GPIO Pin number
 */
#define NGPIO_ID_TO_PIN(id)             NP_DEV_ID_TO_MINOR(id)

#define NGPIOA                          0
#define NGPIOB                          1
#define NGPIOC                          2
#define NGPIOD                          3
#define NGPIOE                          4
#define NGPIOF                          5
#define NGPIOG                          6
#define NGPIOH                          7
#define NGPIOI                          8
#define NGPIOJ                          9
#define NGPIOK                          10

#define NGPIO_INPUT                     (0x1ul << 0)
#define NGPIO_OUTPUT_LOW                (0x1ul << 1)
#define NGPIO_OUTPUT_HIGH               (0x1ul << 2)
#define NGPIO_OUTPUT_OPEN_DRAIN_LOW     (0x1ul << 3)
#define NGPIO_OUTPUT_OPEN_DRAIN_FLOAT 	(0x1ul << 4)
#define NGPIO_PULL_NONE					(0)
#define NGPIO_PULL_UP                   (0x1ul << 5)
#define NGPIO_PULL_DOWN                 (0x1ul << 6)

#define NGPIO_TRIGGER_FALLING			(0x1ul << 7)
#define NGPIO_TRIGGER_RISING			(0x1ul << 8)
#define NGPIO_TRIGGER_TOGGLE			(0x1ul << 9)

#define NGPIO_MODE                                                              \
    (NGPIO_INPUT | NGPIO_OUTPUT_LOW | NGPIO_OUTPUT_HIGH | 						\
	 NGPIO_OUTPUT_OPEN_DRAIN_LOW    | NGPIO_OUTPUT_OPEN_DRAIN_FLOAT)

#define NGPIO_PULL																\
	(NGPIO_PULL_UP | NGPIO_PULL_DOWN)

#define NGPIO_TRIGGER															\
	(NGPIO_TRIGGER_FALLING | NGPIO_TRIGGER_RISING | NGPIO_TRIGGER_TOGGLE)

/*-------------------------------------------------------  C++ extern base  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

/**@brief       GPIO driver structure
 */
struct ngpio_driver
{
    struct npdrv               pdrv;
};

typedef void (ngpio_change_handler)(uint32_t gpio_id);

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/

void ngpio_init(uint32_t gpio_id, uint32_t config);
void ngpio_term(uint32_t gpio_id);
bool ngpio_is_set(uint32_t gpio_id);
void ngpio_set(uint32_t gpio_id);
void ngpio_clear(uint32_t gpio_id);
void ngpio_toggle(uint32_t gpio_id);
void ngpio_request(uint32_t gpio_id);
void ngpio_release(uint32_t gpio_id);
void ngpio_change_notice_request(uint32_t gpio_id, uint32_t config, ngpio_change_handler * change_handler);
void ngpio_change_notice_release(uint32_t gpio_id);
bool ngpio_is_id_valid(uint32_t gpio_id);
void ngpio_isr(void);


/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of gpio.h
 ******************************************************************************/
#endif /* NEON_MCU_GPIO_H_ */
