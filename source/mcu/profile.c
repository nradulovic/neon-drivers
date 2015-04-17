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
 * @brief       Profile data
 *********************************************************************//** @{ */

/*=========================================================  INCLUDE FILES  ==*/

#include "mcu/profile.h"
#include "mcu/peripheral.h"
#include "base/bitop.h"
#include <stddef.h>

/*=========================================================  LOCAL MACRO's  ==*/
/*======================================================  LOCAL DATA TYPES  ==*/
/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/
/*=======================================================  LOCAL VARIABLES  ==*/
/*======================================================  GLOBAL VARIABLES  ==*/

static const struct npdev *     g_devices[64] =
{
#if (NPROFILE_EN_GPIO & NPROFILE_EN(1))
    &g_gpioa,
#else
	NULL,
#endif
#if (NPROFILE_EN_GPIO & NPROFILE_EN(2))
    &g_gpiob,
#else
	NULL,
#endif
#if (NPROFILE_EN_GPIO & NPROFILE_EN(3))
    &g_gpioc,
#else
	NULL,
#endif
#if (NPROFILE_EN_GPIO & NPROFILE_EN(4))
    &g_gpiod,
#else
	NULL,
#endif
#if (NPROFILE_EN_GPIO & NPROFILE_EN(5))
    &g_gpioe,
#else
	NULL,
#endif
#if (NPROFILE_EN_GPIO & NPROFILE_EN(6))
    &g_gpiof,
#else
	NULL,
#endif
#if (NPROFILE_EN_GPIO & NPROFILE_EN(7))
    &g_gpiog,
#else
	NULL,
#endif
#if (NPROFILE_EN_GPIO & NPROFILE_EN(8))
    &g_gpioh,
#else
	NULL,
#endif
#if (NPROFILE_EN_GPIO & NPROFILE_EN(9))
    &g_gpioi,
#else
	NULL,
#endif
#if (NPROFILE_EN_GPIO & NPROFILE_EN(10))
    &g_gpioj,
#else
	NULL,
#endif
#if (NPROFILE_EN_GPIO & NPROFILE_EN(11))
    &g_gpiok,
#else
	NULL,
#endif
#if (NPROFILE_EN_GPIO & NPROFILE_EN(12))
    &g_gpiol,
#else
	NULL,
#endif
#if (NPROFILE_EN_GPIO & NPROFILE_EN(13))
    &g_gpiom,
#else
	NULL,
#endif
#if (NPROFILE_EN_GPIO & NPROFILE_EN(14))
    &g_gpion,
#else
	NULL,
#endif
#if (NPROFILE_EN_GPIO & NPROFILE_EN(15))
    &g_gpioo,
#else
	NULL,
#endif
#if (NPROFILE_EN_GPIO & NPROFILE_EN(16))
    &g_gpiop,
#else
	NULL,
#endif
#if (NPROFILE_EN_UART & NPROFILE_EN(1))
    &g_uart1,
#else
	NULL,
#endif
#if (NPROFILE_EN_UART & NPROFILE_EN(2))
    &g_uart2,
#else
	NULL,
#endif
#if (NPROFILE_EN_UART & NPROFILE_EN(3))
    &g_uart3,
#else
	NULL,
#endif
#if (NPROFILE_EN_UART & NPROFILE_EN(4))
    &g_uart4,
#else
	NULL,
#endif
#if (NPROFILE_EN_UART & NPROFILE_EN(5))
    &g_uart5,
#else
	NULL,
#endif
#if (NPROFILE_EN_UART & NPROFILE_EN(6))
    &g_uart6,
#else
	NULL,
#endif
#if (NPROFILE_EN_UART & NPROFILE_EN(7))
    &g_uart7,
#else
	NULL,
#endif
#if (NPROFILE_EN_UART & NPROFILE_EN(8))
    &g_uart8,
#else
	NULL,
#endif
#if (NPROFILE_EN_UART & NPROFILE_EN(9))
    &g_uart9,
#else
	NULL,
#endif
#if (NPROFILE_EN_UART & NPROFILE_EN(10))
    &g_uart10,
#else
	NULL,
#endif
#if (NPROFILE_EN_UART & NPROFILE_EN(11))
    &g_uart11,
#else
	NULL,
#endif
#if (NPROFILE_EN_UART & NPROFILE_EN(12))
    &g_uart12,
#else
	NULL,
#endif
#if (NPROFILE_EN_UART & NPROFILE_EN(13))
    &g_uart13,
#else
	NULL,
#endif
#if (NPROFILE_EN_UART & NPROFILE_EN(14))
    &g_uart14,
#else
	NULL,
#endif
#if (NPROFILE_EN_UART & NPROFILE_EN(15))
    &g_uart15,
#else
	NULL,
#endif
#if (NPROFILE_EN_UART & NPROFILE_EN(16))
    &g_uart16,
#else
	NULL,
#endif
#if (NPROFILE_EN_SPI & NPROFILE_EN(1))
    &g_spi1,
#else
	NULL,
#endif
#if (NPROFILE_EN_SPI & NPROFILE_EN(2))
    &g_spi2,
#else
	NULL,
#endif
#if (NPROFILE_EN_SPI & NPROFILE_EN(3))
    &g_spi3,
#else
	NULL,
#endif
#if (NPROFILE_EN_SPI & NPROFILE_EN(4))
    &g_spi4,
#else
	NULL,
#endif
#if (NPROFILE_EN_SPI & NPROFILE_EN(5))
    &g_spi5,
#else
	NULL,
#endif
#if (NPROFILE_EN_SPI & NPROFILE_EN(6))
    &g_spi6,
#else
	NULL,
#endif
#if (NPROFILE_EN_SPI & NPROFILE_EN(7))
    &g_spi7,
#else
	NULL,
#endif
#if (NPROFILE_EN_SPI & NPROFILE_EN(8))
    &g_spi8,
#else
	NULL,
#endif
#if (NPROFILE_EN_SPI & NPROFILE_EN(9))
    &g_spi9,
#else
	NULL,
#endif
#if (NPROFILE_EN_SPI & NPROFILE_EN(10))
    &g_spi10,
#else
	NULL,
#endif
#if (NPROFILE_EN_SPI & NPROFILE_EN(11))
    &g_spi11,
#else
	NULL,
#endif
#if (NPROFILE_EN_SPI & NPROFILE_EN(12))
    &g_spi12,
#else
	NULL,
#endif
#if (NPROFILE_EN_SPI & NPROFILE_EN(13))
    &g_spi13,
#else
	NULL,
#endif
#if (NPROFILE_EN_SPI & NPROFILE_EN(14))
    &g_spi14,
#else
	NULL,
#endif
#if (NPROFILE_EN_SPI & NPROFILE_EN(15))
    &g_spi15,
#else
	NULL,
#endif
#if (NPROFILE_EN_SPI & NPROFILE_EN(16))
    &g_spi16,
#else
	NULL,
#endif
#if (NPROFILE_EN_I2C & NPROFILE_EN(1))
    &g_i2c1,
#else
	NULL,
#endif
#if (NPROFILE_EN_I2C & NPROFILE_EN(2))
    &g_i2c2,
#else
	NULL,
#endif
#if (NPROFILE_EN_I2C & NPROFILE_EN(3))
    &g_i2c3,
#else
	NULL,
#endif
#if (NPROFILE_EN_I2C & NPROFILE_EN(4))
    &g_i2c4,
#else
	NULL,
#endif
#if (NPROFILE_EN_I2C & NPROFILE_EN(5))
    &g_i2c5,
#else
	NULL,
#endif
#if (NPROFILE_EN_I2C & NPROFILE_EN(6))
    &g_i2c6,
#else
	NULL,
#endif
#if (NPROFILE_EN_I2C & NPROFILE_EN(7))
    &g_i2c7,
#else
	NULL,
#endif
#if (NPROFILE_EN_I2C & NPROFILE_EN(8))
    &g_i2c8,
#else
	NULL,
#endif
#if (NPROFILE_EN_I2C & NPROFILE_EN(9))
    &g_i2c9,
#else
	NULL,
#endif
#if (NPROFILE_EN_I2C & NPROFILE_EN(10))
    &g_i2c10,
#else
	NULL,
#endif
#if (NPROFILE_EN_I2C & NPROFILE_EN(11))
    &g_i2c11,
#else
	NULL,
#endif
#if (NPROFILE_EN_I2C & NPROFILE_EN(12))
    &g_i2c12,
#else
	NULL,
#endif
#if (NPROFILE_EN_I2C & NPROFILE_EN(13))
    &g_i2c13,
#else
	NULL,
#endif
#if (NPROFILE_EN_I2C & NPROFILE_EN(14))
    &g_i2c14,
#else
	NULL,
#endif
#if (NPROFILE_EN_I2C & NPROFILE_EN(15))
    &g_i2c15,
#else
	NULL,
#endif
#if (NPROFILE_EN_I2C & NPROFILE_EN(16))
    &g_i2c16
#else
	NULL
#endif
};

/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/
/*===========================================  GLOBAL FUNCTION DEFINITIONS  ==*/

const struct npdev * nprofile_pdev(uint32_t dev_id)
{
	const struct npdev *		pdev;
	uint32_t 					dev_index;

	dev_index  = (NP_DEV_ID_TO_CLASS(dev_id) & 0xffu) * 16u;
	dev_index += NP_DEV_ID_TO_MAJOR(dev_id);

	pdev = g_devices[dev_index];

	return (pdev);
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of profile.c
 ******************************************************************************/
