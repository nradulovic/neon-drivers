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
 * @brief       Profile data for STM32Fxxx port
 *********************************************************************//** @{ */

/* NOTE 1:
 * Because of specific nature of this source file it does not follow the
 * standard source code file template.
 *
 * NOTE 2:
 * Each microcontroller family has its own if/endif block of static structures
 * which contains individual peripheral data to describe the peripherals.
 * Each peripheral's data is then exported by a global structure.
 */

/*=========================================================  INCLUDE FILES  ==*/

#include "mcu/peripheral.h"
#include "mcu/profile.h"
#include "mcu/uart.h"
#include "mcu/gpio.h"
#include "mcu/i2c_bus.h"

/*================================================  PERIPHERAL DESCIPTIONS  ==*/

/* -------------------------------------------------------------------------- *
 * NOTE:
 * Static structures for STM32F411xE
 * -------------------------------------------------------------------------- */
#if defined(STM32F411xE)
#if (NPROFILE_EN_GPIO)
struct npdev_isr				g_exti_isr[16] =
{
	{
		.irqn = EXTI0_IRQn,
	},
	{
		.irqn = EXTI1_IRQn,
	},
	{
		.irqn = EXTI2_IRQn,
	},
	{
		.irqn = EXTI3_IRQn,
	},
	{
		.irqn = EXTI4_IRQn,
	},
	{
		.irqn = EXTI9_5_IRQn,
	},
	{
		.irqn = EXTI9_5_IRQn,
	},
	{
		.irqn = EXTI9_5_IRQn,
	},
	{
		.irqn = EXTI9_5_IRQn,
	},
	{
		.irqn = EXTI9_5_IRQn,
	},
	{
		.irqn = EXTI15_10_IRQn,
	},
	{
		.irqn = EXTI15_10_IRQn,
	},
	{
		.irqn = EXTI15_10_IRQn,
	},
	{
		.irqn = EXTI15_10_IRQn,
	},
	{
		.irqn = EXTI15_10_IRQn,
	},
	{
		.irqn = EXTI15_10_IRQn,
	},
};
#endif /* NPROFILE_EN_GPIO */
/*-- GPIOA  ------------------------------------------------------------------*/
#if (NPROFILE_EN_GPIO & NPROFILE_EN(0))
static struct ngpio_driver      g_gpioa_driver;

static const struct npdev_pwr   g_gpioa_pwr =
{
    .reg                = &RCC->AHB1ENR,
    .mask               = RCC_AHB1ENR_GPIOAEN
};

const struct npdev              g_gpioa =
{
    .recognition        = NP_DEV_RECOGNITION(NPROFILE_CLASS_GPIO, 0),
	.max_ref			= 16,
    .pdrv               = &g_gpioa_driver.pdrv,
    .address            = (volatile void *)GPIOA,
	.isr				= g_exti_isr,
    .pwr                = &g_gpioa_pwr
};
#endif /* (NPROFILE_EN_GPIO & NP_MASK_MAJOR_1) */

/*-- GPIOB  ------------------------------------------------------------------*/
#if (NPROFILE_EN_GPIO & NPROFILE_EN(1))
static struct ngpio_driver      g_gpiob_driver;
static const struct npdev_pwr   g_gpiob_pwr =
{
    .reg                = &RCC->AHB1ENR,
    .mask               = RCC_AHB1ENR_GPIOBEN
};

const struct npdev              g_gpiob =
{
    .recognition        = NP_DEV_RECOGNITION(NPROFILE_CLASS_GPIO, 1),
	.max_ref			= 16,
    .pdrv               = &g_gpiob_driver.pdrv,
    .address            = (volatile void *)GPIOB,
	.isr				= g_exti_isr,
    .pwr                = &g_gpiob_pwr
};
#endif /* (NPROFILE_EN_GPIO & NP_MASK_MAJOR_2) */

/*-- GPIOC  ------------------------------------------------------------------*/
#if (NPROFILE_EN_GPIO & NPROFILE_EN(2))
static struct ngpio_driver      g_gpioc_driver;

static const struct npdev_pwr   g_gpioc_pwr =
{
    .reg                = &RCC->AHB1ENR,
    .mask               = RCC_AHB1ENR_GPIOCEN
};

const struct npdev              g_gpioc =
{
    .recognition        = NP_DEV_RECOGNITION(NPROFILE_CLASS_GPIO, 2),
	.max_ref			= 16,
    .pdrv               = &g_gpioc_driver.pdrv,
    .address            = (volatile void *)GPIOC,
	.isr				= g_exti_isr,
    .pwr                = &g_gpioc_pwr
};
#endif /* (NPROFILE_EN_GPIO & NP_MASK_MAJOR_3) */

/*-- UART1  ------------------------------------------------------------------*/
#if (NPROFILE_EN_UART & NPROFILE_EN(0))
static struct nuart_drv         g_uart1_driver;

static const struct npdev_isr   g_uart1_isr =
{
    .irqn               = USART1_IRQn
};

static const struct npdev_pwr   g_uart1_pwr =
{
    .reg                = &RCC->APB2ENR,
    .mask               = RCC_APB2ENR_USART1EN
};

static const struct npdev_clk	g_uart1_clk =
{
	.clk_bus			= ARCH_STM32_APB2
};

static const struct npdev_mux   g_uart1_mux =
{
    .af                 = GPIO_AF7_USART1,
    .mode               = GPIO_MODE_AF_PP,
    .pull               = GPIO_NOPULL
};

const struct npdev              g_uart1 =
{
    .recognition        = NP_DEV_RECOGNITION(NPROFILE_CLASS_UART, 0),
    .flags              = NUART_MODE_RX    | NUART_MODE_TX      | NUART_PARITY_EVEN |
                          NUART_PARITY_ODD | NUART_PARITY_NONE  | NUART_STOPBITS_1  |
                          NUART_STOPBITS_2 | NUART_WORDLENGTH_8 | NUART_WORDLENGTH_9,
	.max_ref			= 1,
    .pdrv               = &g_uart1_driver.pdrv,
    .address            = (volatile void *)USART1_BASE,
    .isr                = &g_uart1_isr,
    .pwr                = &g_uart1_pwr,
	.clk				= &g_uart1_clk,
    .mux                = &g_uart1_mux
};

#endif /* (NPROFILE_EN_UART & NPROFILE_EN(1)) */

/*-- UART2  ------------------------------------------------------------------*/
#if (NPROFILE_EN_UART & NPROFILE_EN(1))
static struct nuart_drv         g_uart2_driver;

static const struct npdev_isr   g_uart2_isr =
{
    .irqn               = USART2_IRQn
};

static const struct npdev_pwr   g_uart2_pwr =
{
    .reg                = &RCC->APB1ENR,
    .mask               = RCC_APB1ENR_USART2EN
};

static const struct npdev_clk 	g_uart2_clk =
{
	.clk_bus			= ARCH_STM32_APB1
};

static const struct npdev_mux   g_uart2_mux =
{
    .af                 = GPIO_AF7_USART2,
    .mode               = GPIO_MODE_AF_PP,
    .pull               = GPIO_NOPULL
};

const struct npdev              g_uart2 =
{
    .recognition        = NP_DEV_RECOGNITION(NPROFILE_CLASS_UART, 1),
    .flags              = NUART_MODE_RX    | NUART_MODE_TX      | NUART_PARITY_EVEN |
                          NUART_PARITY_ODD | NUART_PARITY_NONE  | NUART_STOPBITS_1  |
                          NUART_STOPBITS_2 | NUART_WORDLENGTH_8 | NUART_WORDLENGTH_9,
    .max_ref			= 1,
    .pdrv               = &g_uart2_driver.pdrv,
    .address            = (volatile void *)USART2_BASE,
    .isr                = &g_uart2_isr,
    .pwr                = &g_uart2_pwr,
	.clk				= &g_uart2_clk,
    .mux                = &g_uart2_mux
};
#endif /* (NPROFILE_EN_UART & NPROFILE_EN(2)) */

/*-- UART6  ------------------------------------------------------------------*/
#if (NPROFILE_EN_UART & NPROFILE_EN(5))
static struct nuart_drv         g_uart6_driver;

static const struct npdev_isr   g_uart6_isr =
{
    .irqn               = USART6_IRQn
};

static const struct npdev_pwr   g_uart6_pwr =
{
	.reg                = &RCC->APB2ENR,
    .mask               = RCC_APB2ENR_USART6EN
};

static const struct npdev_clk	g_uart6_clk =
{
	.clk_bus			= ARCH_STM32_APB2
};

static const struct npdev_mux   g_uart6_mux =
{
    .af                 = GPIO_AF8_USART6,
    .mode               = GPIO_MODE_AF_PP,
    .pull               = GPIO_NOPULL
};

const struct npdev              g_uart6 =
{
    .recognition        = NP_DEV_RECOGNITION(NPROFILE_CLASS_UART, 5),
    .flags              = NUART_MODE_RX    | NUART_MODE_TX      | NUART_PARITY_EVEN |
                          NUART_PARITY_ODD | NUART_PARITY_NONE  | NUART_STOPBITS_1  |
                          NUART_STOPBITS_2 | NUART_WORDLENGTH_8 | NUART_WORDLENGTH_9,
	.max_ref			= 1,
    .pdrv               = &g_uart6_driver.pdrv,
    .address            = (volatile void *)USART6_BASE,
    .isr                = &g_uart6_isr,
    .pwr                = &g_uart6_pwr,
	.clk				= &g_uart6_clk,
    .mux                = &g_uart6_mux
};
#endif /* (NPROFILE_EN_UART & NPROFILE_EN(6)) */

#endif /* defined(STM32F411xE) */

#if defined(STM32F429xx)
#if (NPROFILE_EN_GPIO)
struct npdev_isr				g_exti_isr[16] =
{
	{
		.irqn = EXTI0_IRQn,
	},
	{
		.irqn = EXTI1_IRQn,
	},
	{
		.irqn = EXTI2_IRQn,
	},
	{
		.irqn = EXTI3_IRQn,
	},
	{
		.irqn = EXTI4_IRQn,
	},
	{
		.irqn = EXTI9_5_IRQn,
	},
	{
		.irqn = EXTI9_5_IRQn,
	},
	{
		.irqn = EXTI9_5_IRQn,
	},
	{
		.irqn = EXTI9_5_IRQn,
	},
	{
		.irqn = EXTI9_5_IRQn,
	},
	{
		.irqn = EXTI15_10_IRQn,
	},
	{
		.irqn = EXTI15_10_IRQn,
	},
	{
		.irqn = EXTI15_10_IRQn,
	},
	{
		.irqn = EXTI15_10_IRQn,
	},
	{
		.irqn = EXTI15_10_IRQn,
	},
	{
		.irqn = EXTI15_10_IRQn,
	},
};
#endif /* NPROFILE_EN_GPIO */

/*-- GPIOA  ------------------------------------------------------------------*/
#if (NPROFILE_EN_GPIO & NPROFILE_EN(0))
static struct ngpio_driver      g_gpioa_driver;
static const struct npdev_pwr   g_gpioa_pwr =
{
    .reg                = &RCC->AHB1ENR,
    .mask               = RCC_AHB1ENR_GPIOAEN
};

const struct npdev              g_gpioa =
{
    .recognition        = NP_DEV_RECOGNITION(NPROFILE_CLASS_GPIO, 0),
	.max_ref			= 16,
    .pdrv               = &g_gpioa_driver.pdrv,
    .address            = (volatile void *)GPIOA,
	.isr				= g_exti_isr,
    .pwr                = &g_gpioa_pwr
};
#endif /* (NPROFILE_EN_GPIO & NP_MASK_MAJOR_0) */

/*-- GPIOB  ------------------------------------------------------------------*/
#if (NPROFILE_EN_GPIO & NPROFILE_EN(1))
static struct ngpio_driver      g_gpiob_driver;
static const struct npdev_pwr   g_gpiob_pwr =
{
    .reg                = &RCC->AHB1ENR,
    .mask               = RCC_AHB1ENR_GPIOBEN
};

const struct npdev              g_gpiob =
{
    .recognition        = NP_DEV_RECOGNITION(NPROFILE_CLASS_GPIO, 1),
	.max_ref			= 16,
    .pdrv               = &g_gpiob_driver.pdrv,
    .address            = (volatile void *)GPIOB,
	.isr				= g_exti_isr,
    .pwr                = &g_gpiob_pwr
};
#endif /* (NPROFILE_EN_GPIO & NP_MASK_MAJOR_1) */

/*-- GPIOC  ------------------------------------------------------------------*/
#if (NPROFILE_EN_GPIO & NPROFILE_EN(2))
static struct ngpio_driver      g_gpioc_driver;

static const struct npdev_pwr   g_gpioc_pwr =
{
    .reg                = &RCC->AHB1ENR,
    .mask               = RCC_AHB1ENR_GPIOCEN
};

const struct npdev              g_gpioc =
{
    .recognition        = NP_DEV_RECOGNITION(NPROFILE_CLASS_GPIO, 2),
	.max_ref			= 16,
    .pdrv               = &g_gpioc_driver.pdrv,
    .address            = (volatile void *)GPIOC,
	.isr				= g_exti_isr,
    .pwr                = &g_gpioc_pwr
};
#endif /* (NPROFILE_EN_GPIO & NP_MASK_MAJOR_2) */

/*-- GPIOD  ------------------------------------------------------------------*/
#if (NPROFILE_EN_GPIO & NPROFILE_EN(3))
static struct ngpio_driver      g_gpiod_driver;

static const struct npdev_pwr   g_gpiod_pwr =
{
    .reg                = &RCC->AHB1ENR,
    .mask               = RCC_AHB1ENR_GPIODEN
};

const struct npdev              g_gpiod =
{
    .recognition        = NP_DEV_RECOGNITION(NPROFILE_CLASS_GPIO, 3),
	.max_ref			= 16,
    .pdrv               = &g_gpiod_driver.pdrv,
    .address            = (volatile void *)GPIOD,
	.isr				= g_exti_isr,
    .pwr                = &g_gpiod_pwr
};
#endif /* (NPROFILE_EN_GPIO & NP_MASK_MAJOR_3) */

/*-- GPIOE  ------------------------------------------------------------------*/
#if (NPROFILE_EN_GPIO & NPROFILE_EN(4))
static struct ngpio_driver      g_gpioe_driver;

static const struct npdev_pwr   g_gpioe_pwr =
{
    .reg                = &RCC->AHB1ENR,
    .mask               = RCC_AHB1ENR_GPIOEEN
};

const struct npdev              g_gpioe =
{
    .recognition        = NP_DEV_RECOGNITION(NPROFILE_CLASS_GPIO, 4),
	.max_ref			= 16,
    .pdrv               = &g_gpioe_driver.pdrv,
    .address            = (volatile void *)GPIOE,
	.isr				= g_exti_isr,
    .pwr                = &g_gpioe_pwr
};


#endif /* (NPROFILE_EN_GPIO & NP_MASK_MAJOR_4) */

/*-- GPIOF  ------------------------------------------------------------------*/
#if (NPROFILE_EN_GPIO & NPROFILE_EN(5))
static struct ngpio_driver      g_gpiof_driver;

static const struct npdev_pwr   g_gpiof_pwr =
{
    .reg                = &RCC->AHB1ENR,
    .mask               = RCC_AHB1ENR_GPIOFEN
};

const struct npdev              g_gpiof =
{
    .recognition        = NP_DEV_RECOGNITION(NPROFILE_CLASS_GPIO, 5),
	.max_ref			= 16,
    .pdrv               = &g_gpiof_driver.pdrv,
    .address            = (volatile void *)GPIOF,
	.isr				= g_exti_isr,
    .pwr                = &g_gpiof_pwr
};
#endif /* (NPROFILE_EN_GPIO & NP_MASK_MAJOR_5) */

/*-- GPIOG  ------------------------------------------------------------------*/
#if (NPROFILE_EN_GPIO & NPROFILE_EN(6))
static struct ngpio_driver      g_gpiog_driver;

static const struct npdev_pwr   g_gpiog_pwr =
{
    .reg                = &RCC->AHB1ENR,
    .mask               = RCC_AHB1ENR_GPIOGEN
};

const struct npdev              g_gpiog =
{
    .recognition        = NP_DEV_RECOGNITION(NPROFILE_CLASS_GPIO, 6),
	.max_ref			= 16,
    .pdrv               = &g_gpiog_driver.pdrv,
    .address            = (volatile void *)GPIOG,
	.isr				= g_exti_isr,
    .pwr                = &g_gpiog_pwr
};
#endif /* (NPROFILE_EN_GPIO & NP_MASK_MAJOR_6) */

/*-- GPIOH  ------------------------------------------------------------------*/
#if (NPROFILE_EN_GPIO & NPROFILE_EN(7))
static struct ngpio_driver      g_gpioh_driver;

static const struct npdev_pwr   g_gpioh_pwr =
{
    .reg                = &RCC->AHB1ENR,
    .mask               = RCC_AHB1ENR_GPIOHEN
};

const struct npdev              g_gpioh =
{
    .recognition        = NP_DEV_RECOGNITION(NPROFILE_CLASS_GPIO, 7),
	.max_ref			= 16,
    .pdrv               = &g_gpioh_driver.pdrv,
    .address            = (volatile void *)GPIOH,
	.isr				= g_exti_isr,
    .pwr                = &g_gpioh_pwr
};
#endif /* (NPROFILE_EN_GPIO & NP_MASK_MAJOR_7) */

/*-- GPIOI  ------------------------------------------------------------------*/
#if (NPROFILE_EN_GPIO & NPROFILE_EN(8))
static struct ngpio_driver      g_gpioi_driver;

static const struct npdev_pwr   g_gpioi_pwr =
{
    .reg                = &RCC->AHB1ENR,
    .mask               = RCC_AHB1ENR_GPIOIEN
};

const struct npdev              g_gpioi =
{
    .recognition        = NP_DEV_RECOGNITION(NPROFILE_CLASS_GPIO, 8),
	.max_ref			= 16,
    .pdrv               = &g_gpioi_driver.pdrv,
    .address            = (volatile void *)GPIOI,
	.isr				= g_exti_isr,
    .pwr                = &g_gpioi_pwr
};
#endif /* (NPROFILE_EN_GPIO & NP_MASK_MAJOR_8) */

/*-- GPIOJ  ------------------------------------------------------------------*/
#if (NPROFILE_EN_GPIO & NPROFILE_EN(9))
static struct ngpio_driver      g_gpioj_driver;

static const struct npdev_pwr   g_gpioj_pwr =
{
    .reg                = &RCC->AHB1ENR,
    .mask               = RCC_AHB1ENR_GPIOJEN
};

const struct npdev              g_gpioj =
{
    .recognition        = NP_DEV_RECOGNITION(NPROFILE_CLASS_GPIO, 9),
	.max_ref			= 16,
    .pdrv               = &g_gpioj_driver.pdrv,
    .address            = (volatile void *)GPIOJ,
	.isr				= g_exti_isr,
    .pwr                = &g_gpioj_pwr
};
#endif /* (NPROFILE_EN_GPIO & NP_MASK_MAJOR_9) */

/*-- GPIOK  ------------------------------------------------------------------*/
#if (NPROFILE_EN_GPIO & NPROFILE_EN(10))
static struct ngpio_driver      g_gpiok_driver;

static const struct npdev_pwr   g_gpiok_pwr =
{
    .reg                = &RCC->AHB1ENR,
    .mask               = RCC_AHB1ENR_GPIOKEN
};

const struct npdev              g_gpiok =
{
    .recognition        = NP_DEV_RECOGNITION(NPROFILE_CLASS_GPIO, 10),
	.max_ref			= 16,
    .pdrv               = &g_gpiok_driver.pdrv,
    .address            = (volatile void *)GPIOK,
	.isr				= g_exti_isr,
    .pwr                = &g_gpiok_pwr
};
#endif /* (NPROFILE_EN_GPIO & NP_MASK_MAJOR_10) */

/*-- I2C1 ------------------------------------------------------------------*/
#if (NPROFILE_EN_I2C & NPROFILE_EN(1))
struct ni2c_bus_driver   g_i2c1_driver;

static const struct npdev_isr   g_i2c1_isr[2] =
{
	{
		.irqn           = I2C1_EV_IRQn,
	},
	{
		.irqn           = I2C1_ER_IRQn,
	}
};

static const struct npdev_pwr   g_i2c1_pwr =
{
    .reg                = &RCC->APB1ENR,
    .mask               = RCC_APB1ENR_I2C1EN
};

static const struct npdev_clk	g_i2c1_clk =
{
	.clk_bus			= ARCH_STM32_APB1
};

const struct npdev              g_i2c1 =
{
    .recognition        = NP_DEV_RECOGNITION(NPROFILE_CLASS_I2C, 1),
    .flags              = NI2C_BUS_MODE_MASTER | NI2C_BUS_MODE_SLAVE | NI2C_BUS_ADDRESS_7BIT |
						  NI2C_BUS_ADDRESS_10BIT | NI2C_BUS_SPEED_100 | NI2C_BUS_SPEED_400  |
						  NI2C_BUS_SPEED_1700 | NI2C_BUS_SPEED_3400 | NI2C_BUS_HANDLING_IT |
						  NI2C_BUS_HANDLING_DMA,

	.max_ref			= 1,
    .pdrv               = &g_i2c1_driver.pdrv,
    .address            = (volatile void *)I2C1_BASE,
    .isr                = g_i2c1_isr,
    .pwr                = &g_i2c1_pwr,
	.clk				= &g_i2c1_clk
};

#endif /* (NPROFILE_EN_I2C & NPROFILE_EN(1)) */

/*-- I2C2 ------------------------------------------------------------------*/
#if (NPROFILE_EN_I2C & NPROFILE_EN(2))
 struct ni2c_bus_driver   g_i2c2_driver;

static const struct npdev_isr   g_i2c2_isr[2] =
{
	{
		.irqn           = I2C2_EV_IRQn,
	},
	{
		.irqn           = I2C2_ER_IRQn,
	}
};

static const struct npdev_pwr   g_i2c2_pwr =
{
    .reg                = &RCC->APB1ENR,
    .mask               = RCC_APB1ENR_I2C2EN
};

static const struct npdev_clk	g_i2c2_clk =
{
	.clk_bus			= ARCH_STM32_APB1
};

const struct npdev              g_i2c2 =
{
    .recognition        = NP_DEV_RECOGNITION(NPROFILE_CLASS_I2C, 2),
    .flags              = NI2C_BUS_MODE_MASTER | NI2C_BUS_MODE_SLAVE | NI2C_BUS_ADDRESS_7BIT |
						  NI2C_BUS_ADDRESS_10BIT | NI2C_BUS_SPEED_100 | NI2C_BUS_SPEED_400  |
						  NI2C_BUS_SPEED_1700 | NI2C_BUS_SPEED_3400 | NI2C_BUS_HANDLING_IT |
						  NI2C_BUS_HANDLING_DMA,

	.max_ref			= 1,
    .pdrv               = &g_i2c2_driver.pdrv,
    .address            = (volatile void *)I2C2_BASE,
    .isr                = g_i2c2_isr,
    .pwr                = &g_i2c2_pwr,
	.clk				= &g_i2c2_clk
};

#endif /* (NPROFILE_EN_I2C & NPROFILE_EN(2)) */

/*-- I2C3 ------------------------------------------------------------------*/
#if (NPROFILE_EN_I2C & NPROFILE_EN(3))
struct ni2c_bus_driver   g_i2c3_driver;

static const struct npdev_isr   g_i2c3_isr[2] =
{
	{
		.irqn           = I2C3_EV_IRQn,
	},
	{
		.irqn           = I2C3_ER_IRQn,
	}
};

static const struct npdev_pwr   g_i2c3_pwr =
{
    .reg                = &RCC->APB1ENR,
    .mask               = RCC_APB1ENR_I2C3EN
};

static const struct npdev_clk	g_i2c3_clk =
{
	.clk_bus			= ARCH_STM32_APB1
};

const struct npdev              g_i2c3 =
{
    .recognition        = NP_DEV_RECOGNITION(NPROFILE_CLASS_I2C, 3),
    .flags              = NI2C_BUS_MODE_MASTER | NI2C_BUS_MODE_SLAVE | NI2C_BUS_ADDRESS_7BIT |
						  NI2C_BUS_ADDRESS_10BIT | NI2C_BUS_SPEED_100 | NI2C_BUS_SPEED_400  |
						  NI2C_BUS_SPEED_1700 | NI2C_BUS_SPEED_3400 | NI2C_BUS_HANDLING_IT |
						  NI2C_BUS_HANDLING_DMA,

	.max_ref			= 1,
    .pdrv               = &g_i2c3_driver.pdrv,
    .address            = (volatile void *)I2C3_BASE,
    .isr                = g_i2c3_isr,
    .pwr                = &g_i2c3_pwr,
	.clk				= &g_i2c3_clk
};

#endif /* (NPROFILE_EN_I2C & NPROFILE_EN(3)) */

#endif /* defined(STM32F429xx) */
/** @} *//*********************************************************************
 * END of p_profile.c
 ******************************************************************************/
