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
 * @brief       MCU profile data
 * @defgroup    profile_intf MCU profile data
 * @brief       MCU profile data
 *********************************************************************//** @{ */

#ifndef NEON_PORT_PROFILE_H_
#define NEON_PORT_PROFILE_H_

/*=========================================================  INCLUDE FILES  ==*/

#include "mcu/config.h"

#include "family/p_profile_data.h"

/*===============================================================  MACRO's  ==*/

#define NPROFILE_CLASS_MAGIC            ((uint16_t)0xde00u)
#define NPROFILE_CLASS_GPIO             (NPROFILE_CLASS_MAGIC | 0)
#define NPROFILE_CLASS_UART             (NPROFILE_CLASS_MAGIC | 1)
#define NPROFILE_CLASS_SPI              (NPROFILE_CLASS_MAGIC | 2)
#define NPROFILE_CLASS_I2C              (NPROFILE_CLASS_MAGIC | 3)

#define N_IS_PROFILE_CLASS(class)		(((class) & 0xff00u) == NPROFILE_CLASS_MAGIC)

#define NPROFILE_EN_1             		NPROFILE_EN( 0)
#define NPROFILE_EN_2             		NPROFILE_EN( 1)
#define NPROFILE_EN_3             		NPROFILE_EN( 2)
#define NPROFILE_EN_4             		NPROFILE_EN( 3)
#define NPROFILE_EN_5             		NPROFILE_EN( 4)
#define NPROFILE_EN_6             		NPROFILE_EN( 5)
#define NPROFILE_EN_7             		NPROFILE_EN( 6)
#define NPROFILE_EN_8             		NPROFILE_EN( 7)
#define NPROFILE_EN_9             		NPROFILE_EN( 8)
#define NPROFILE_EN_10            		NPROFILE_EN(9)
#define NPROFILE_EN_11            		NPROFILE_EN(10)
#define NPROFILE_EN_12            		NPROFILE_EN(11)
#define NPROFILE_EN_13            		NPROFILE_EN(12)
#define NPROFILE_EN_14            		NPROFILE_EN(13)
#define NPROFILE_EN_15            		NPROFILE_EN(14)
#define NPROFILE_EN_16            		NPROFILE_EN(15)

#define NPROFILE_EN(major)        		(0x1u << major)

/**@brief       This macro checks if a GPIO is enabled.
 * @details     In order to check if particular GPIO port is enabled then do a
 *              bitwise AND operation between this macro and the peripheral
 *              macro @ref NP_DEV_MAJOR().
 *
 *              For example, in order to check if GPIO port A is enabled then
 *              you would write 'if (NPROFILE_EN_GPIO & NP_DEV_MAJOR(1))'.
 *
 *              Likewise, you can write 'if (NPROFILE_EN_GPIO)' which will
 *              evaluate to TRUE if any GPIO port is enabled.
 */
#define NPROFILE_EN_GPIO                (CONFIG_GPIO & (NPROFILE_AVAILABLE_GPIO))

/**@brief       This macro checks if an UART is enabled.
 * @details     In order to check if particular UART port is enabled then do a
 *              bitwise AND operation between this macro and the peripheral
 *              macro @ref NP_DEV_MAJOR().
 *
 *              For example, in order to check if UART number 1 is enabled then
 *              you would write 'if (NPROFILE_EN_UART & NP_DEV_MAJOR(1))'.
 *
 *              Likewise, you can write 'if (NPROFILE_EN_UART)' which will
 *              evaluate to TRUE if any UART peripheral is enabled.
 */
#define NPROFILE_EN_UART                (CONFIG_UART & (NPROFILE_AVAILABLE_UART))

/**@brief       This macro checks if an I2C peripheral is enabled.
 * @details     In order to check if particular I2C port is enabled then do a
 *              bitwise AND operation between this macro and the peripheral
 *              macro @ref NP_DEV_MAJOR().
 *
 *              For example, in order to check if I2C number 1 is enabled then
 *              you would write 'if (NPROFILE_EN_I2C & NP_DEV_MAJOR(1))'.
 *
 *              Likewise, you can write 'if (NPROFILE_EN_I2C)' which will
 *              evaluate to TRUE if any I2C peripheral is enabled.
 */
#define NPROFILE_EN_I2C                 (CONFIG_I2C &  (NPROFILE_AVAILABLE_I2C))

/**@brief       This macro checks if an SPI peripheral is enabled.
 * @details     In order to check if particular SPI port is enabled then do a
 *              bitwise AND operation between this macro and the peripheral
 *              macro @ref NP_DEV_MAJOR().
 *
 *              For example, in order to check if SPI number 1 is enabled then
 *              you would write 'if (NPROFILE_EN_SPI & NP_DEV_MAJOR(1))'.
 *
 *              Likewise, you can write 'if (NPROFILE_EN_SPI)' which will
 *              evaluate to TRUE if any SPI peripheral is enabled.
 */
#define NPROFILE_EN_SPI                 (CONFIG_SPI &  (NPROFILE_AVAILABLE_SPI))

/*-------------------------------------------------------  C++ extern base  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/
/*======================================================  GLOBAL VARIABLES  ==*/

/* NOTE:
 * Individual peripheral data is defined in `p_profile_data.c` port source file.
 */
#if (NPROFILE_EN_GPIO & NPROFILE_EN(1))
extern const struct npdev      g_gpioa;
#endif

#if (NPROFILE_EN_GPIO & NPROFILE_EN(2))
extern const struct npdev      g_gpiob;
#endif

#if (NPROFILE_EN_GPIO & NPROFILE_EN(3))
extern const struct npdev      g_gpioc;
#endif

#if (NPROFILE_EN_GPIO & NPROFILE_EN(4))
extern const struct npdev      g_gpiod;
#endif

#if (NPROFILE_EN_GPIO & NPROFILE_EN(5))
extern const struct npdev      g_gpioe;
#endif

#if (NPROFILE_EN_GPIO & NPROFILE_EN(6))
extern const struct npdev      g_gpiof;
#endif

#if (NPROFILE_EN_GPIO & NPROFILE_EN(7))
extern const struct npdev      g_gpiog;
#endif

#if (NPROFILE_EN_GPIO & NPROFILE_EN(8))
extern const struct npdev      g_gpioh;
#endif

#if (NPROFILE_EN_GPIO & NPROFILE_EN(9))
extern const struct npdev      g_gpioi;
#endif

#if (NPROFILE_EN_GPIO & NPROFILE_EN(10))
extern const struct npdev      g_gpioj;
#endif

#if (NPROFILE_EN_GPIO & NPROFILE_EN(11))
extern const struct npdev      g_gpiok;
#endif

#if (NPROFILE_EN_UART & NPROFILE_EN(1))
extern const struct npdev      g_uart1;
#endif

#if (NPROFILE_EN_UART & NPROFILE_EN(2))
extern const struct npdev      g_uart2;
#endif

#if (NPROFILE_EN_UART & NPROFILE_EN(3))
extern const struct npdev      g_uart3;
#endif

#if (NPROFILE_EN_UART & NPROFILE_EN(4))
extern const struct npdev      g_uart4;
#endif

#if (NPROFILE_EN_UART & NPROFILE_EN(5))
extern const struct npdev      g_uart5;
#endif

#if (NPROFILE_EN_UART & NPROFILE_EN(6))
extern const struct npdev      g_uart6;
#endif

/* NOTE:
 * Group peripheral data is defined in `profile.c` source file.
 */
extern const struct npdev *    g_gpios[];

extern const struct npdev *    g_uarts[];

extern const struct npdev *    g_spis[];

extern const struct npdev *    g_i2cs[];

/*===================================================  FUNCTION PROTOTYPES  ==*/

const struct npdev * nprofile_pdev(uint32_t dev_id);

/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/

#if !defined(NPROFILE_MAX_CPU_CLOCK)
# error "NEON::drivers::profile: Internal error: NPROFILE_MAX_CPU_CLOCK is not defined in p_peripheral_data.h file."
#endif

#if !defined(NPROFILE_RAM_SIZE)
# error "NEON::drivers::profile: Internal error: NPROFILE_RAM_SIZE is not defined in p_peripheral_data.h file."
#endif

#if !defined(NPROFILE_AVAILABLE_GPIO)
# error "NEON::drivers::profile: Internal error: NPROFILE_AVAILABLE_GPIO is not defined in p_peripheral_data.h file."
#endif

#if !defined(NPROFILE_AVAILABLE_UART)
# error "NEON::drivers::profile: Internal error: NPROFILE_AVAILABLE_UART is not defined in p_peripheral_data.h file."
#endif

#if !defined(NPROFILE_AVAILABLE_SPI)
# error "NEON::drivers::profile: Internal error: NPROFILE_AVAILABLE_SPI is not defined in p_peripheral_data.h file."
#endif

#if !defined(NPROFILE_AVAILABLE_I2C)
# error "NEON::drivers::profile: Internal error: NPROFILE_AVAILABLE_I2C is not defined in p_peripheral_data.h file."
#endif


/** @endcond *//** @} *//******************************************************
 * END of profile.h
 ******************************************************************************/
#endif /* NEON_PORT_PROFILE_H_ */
