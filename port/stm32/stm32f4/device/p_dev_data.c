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
static struct ngpio_driver              g_gpioa_driver;
static const struct np_dev_clock        g_gpioa_clock =
{
    .reg                = &RCC->AHB1ENR,
    .mask               = RCC_AHB1ENR_GPIOAEN
};

/*-- GPIOB  ------------------------------------------------------------------*/
static struct ngpio_driver              g_gpiob_driver;
static const struct np_dev_clock        g_gpiob_clock =
{
    .reg                = &RCC->AHB1ENR,
    .mask               = RCC_AHB1ENR_GPIOBEN
};

/*-- GPIOC  ------------------------------------------------------------------*/
static struct ngpio_driver              g_gpioc_driver;
static const struct np_dev_clock        g_gpioc_clock =
{
    .reg                = &RCC->AHB1ENR,
    .mask               = RCC_AHB1ENR_GPIOCEN
};

/*-- UART1  ------------------------------------------------------------------*/
static struct nuart_drv      g_uart1_driver;
static const struct np_dev_mux          g_uart1_mux =
{
    .af                 = GPIO_AF7_USART1,
    .mode               = GPIO_MODE_AF_PP,
    .pull               = GPIO_NOPULL
};
static const struct np_dev_clock        g_uart1_clock =
{
    .reg                = &RCC->APB2ENR,
    .mask               = RCC_APB2ENR_USART1EN
};
static const struct np_dev_isr          g_uart1_isr =
{
    .irqn               = USART1_IRQn
};

/*-- UART2  ------------------------------------------------------------------*/
static struct nuart_drv      g_uart2_driver;
static const struct np_dev_mux          g_uart2_mux =
{
    .af                 = GPIO_AF7_USART2,
    .mode               = GPIO_MODE_AF_PP,
    .pull               = GPIO_NOPULL
};
static const struct np_dev_clock        g_uart2_clock =
{
    .reg                = &RCC->APB1ENR,
    .mask               = RCC_APB1ENR_USART2EN
};
static const struct np_dev_isr          g_uart2_isr =
{
    .irqn               = USART2_IRQn
};

/*======================================================  GLOBAL VARIABLES  ==*/



const struct np_dev             g_gpioa =
{
    .recognition        = NP_DEV_RECOGNITION(NGPIO_DEVICE_CLASS_ID, 1),
    .p_drv              = &g_gpioa_driver.p_drv,
    .address            = (volatile unsigned int *)GPIOA_BASE,
    .clock              = &g_gpioa_clock
};

const struct np_dev             g_gpiob =
{
    .recognition        = NP_DEV_RECOGNITION(NGPIO_DEVICE_CLASS_ID, 2),
    .p_drv              = &g_gpiob_driver.p_drv,
    .address            = (volatile unsigned int *)GPIOB_BASE,
    .clock              = &g_gpiob_clock
};

const struct np_dev             g_gpioc =
{
    .recognition        = NP_DEV_RECOGNITION(NGPIO_DEVICE_CLASS_ID, 3),
    .p_drv              = &g_gpioc_driver.p_drv,
    .address            = (volatile unsigned int *)GPIOC_BASE,
    .clock              = &g_gpioc_clock
};

const struct np_dev             g_uart1 =
{
    .recognition        = NP_DEV_RECOGNITION(NUART_DEVICE_CLASS_ID, 1),
    .flags              = NUART_MODE_RX | NUART_MODE_TX | NUART_PARITY_EVEN |
                          NUART_PARITY_ODD | NUART_PARITY_NONE |NUART_STOPBITS_1 |
                          NUART_STOPBITS_2 | NUART_WORDLENGTH_8 |
                          NUART_WORDLENGTH_9,
    .p_drv              = &g_uart1_driver.p_drv,
    .address            = (volatile unsigned int *)USART1_BASE,
    .isr                = &g_uart1_isr,
    .clock              = &g_uart1_clock,
    .mux                = &g_uart1_mux
};

const struct np_dev            g_uart2 =
{
    .recognition        = NP_DEV_RECOGNITION(NUART_DEVICE_CLASS_ID, 2),
    .flags              = NUART_MODE_RX | NUART_MODE_TX | NUART_PARITY_EVEN |
                          NUART_PARITY_ODD | NUART_PARITY_NONE |NUART_STOPBITS_1 |
                          NUART_STOPBITS_2 | NUART_WORDLENGTH_8 |
                          NUART_WORDLENGTH_9,
    .p_drv              = &g_uart2_driver.p_drv,
    .address            = (volatile unsigned int *)USART2_BASE,
    .isr                = &g_uart2_isr,
    .clock              = &g_uart2_clock,
    .mux                = &g_uart2_mux
};


/*-- Device arrays  ----------------------------------------------------------*/

const struct np_dev *           g_gpios[] =
{
    &g_gpioa,
    &g_gpiob,
    &g_gpioc,
    NULL
};

const struct np_dev *           g_uarts[] =
{
    &g_uart1,
    &g_uart2,
    NULL
};

/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/
/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/
/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of profile.c
 ******************************************************************************/
