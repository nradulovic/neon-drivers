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
#include "mcu/gpio.h"
#include "mcu/uart.h"

/*=========================================================  LOCAL MACRO's  ==*/
/*======================================================  LOCAL DATA TYPES  ==*/
/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/
/*=======================================================  LOCAL VARIABLES  ==*/

/*-- GPIOA  ------------------------------------------------------------------*/
static const struct nclock 		g_gpioa_clock =
{
	.reg				= &RCC->AHB1ENR,
	.mask				= RCC_AHB1ENR_GPIOAEN
};

/*-- GPIOB  ------------------------------------------------------------------*/
static const struct nclock 		g_gpiob_clock =
{
	.reg				= &RCC->AHB1ENR,
	.mask				= RCC_AHB1ENR_GPIOBEN
};

/*-- GPIOC  ------------------------------------------------------------------*/
static const struct nclock 		g_gpioc_clock =
{
	.reg				= &RCC->AHB1ENR,
	.mask				= RCC_AHB1ENR_GPIOCEN
};

/*-- UART1  ------------------------------------------------------------------*/
static struct nuart_driver 		g_uart1_driver;
static const struct nmux 		g_uart1_mux =
{
	.af					= GPIO_AF7_USART1,
	.mode				= GPIO_MODE_AF_PP,
	.pull				= GPIO_NOPULL
};
static const struct nclock 		g_uart1_clock =
{
	.reg				= &RCC->APB2ENR,
	.mask				= RCC_APB2ENR_USART1EN
};
static const struct nisr 		g_uart1_isr[] =
{
	{
		.irqn			= USART1_IRQn
	}
};

/*-- UART2  ------------------------------------------------------------------*/
static struct nuart_driver 		g_uart2_driver;
static const struct nmux 		g_uart2_mux =
{
	.af					= GPIO_AF7_USART2,
	.mode				= GPIO_MODE_AF_PP,
	.pull				= GPIO_NOPULL
};
static const struct nclock 		g_uart2_clock =
{
	.reg				= &RCC->APB1ENR,
	.mask				= RCC_APB1ENR_USART2EN
};
static const struct nisr 		g_uart2_isr[] =
{
	{
		.irqn			= USART2_IRQn
	}
};

/*======================================================  GLOBAL VARIABLES  ==*/

const struct nperiph   			g_gpios[] =
{
	{
		.id 				= 1,
		.class_id			= NGPIO_DEVICE_CLASS_ID,
		.address			= (volatile unsigned int *)GPIOA_BASE,
		.clock				= &g_gpioa_clock
	}, {
		.id 				= 2,
		.class_id			= NGPIO_DEVICE_CLASS_ID,
		.address			= (volatile unsigned int *)GPIOB_BASE,
		.clock				= &g_gpiob_clock
	}, {
		.id 				= 3,
		.class_id			= NGPIO_DEVICE_CLASS_ID,
		.address			= (volatile unsigned int *)GPIOC_BASE,
		.clock				= &g_gpioc_clock
	}
};

const struct nperiph 			g_uart1 =
{
	.id 				= 1,
	.class_id			= NUART_DEVICE_CLASS_ID,
	.driver 			= &g_uart1_driver,
	.address			= (volatile unsigned int *)USART1_BASE,
	.isr				= &g_uart1_isr[0],
	.clock				= &g_uart1_clock,
	.mux 				= &g_uart1_mux
};

const struct nperiph 			g_uart2 =
{
	.id					= 2,
	.class_id			= NUART_DEVICE_CLASS_ID,
	.driver 			= &g_uart2_driver,
	.address			= (volatile unsigned int *)USART2_BASE,
	.isr				= &g_uart2_isr[0],
	.clock				= &g_uart2_clock,
	.mux 				= &g_uart2_mux
};

/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/
/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/
/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of profile.c
 ******************************************************************************/
