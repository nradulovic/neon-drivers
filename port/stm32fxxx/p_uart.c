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
 * @brief       UART driver implementation for STM32Fxxx port
 *********************************************************************//** @{ */

/* NOTE:
 * The driver currently supports UART1, UART2 and UART6 peripherals.
 *
 * TODOs:
 * - Implement timeouts
 * - How to handle HAL callbacks? If application code defines and uses UART HAL
 * 		API callbacks then it's not possible to use callbacks in this source.
 */

/*=========================================================  INCLUDE FILES  ==*/

#include <string.h>

#include "mcu/peripheral.h"
#include "mcu/profile.h"

/* NOTE:
 * The condition will turn off this module if not enabled or not available in
 * the current port. This kind of if/endif protection is necessary because this
 * source file may define unwanted ISRs.
 */
#if (NPROFILE_EN_UART)
#include "base/debug.h"
#include "base/component.h"
#include "mcu/uart.h"

/*=========================================================  LOCAL MACRO's  ==*/
/*======================================================  LOCAL DATA TYPES  ==*/
/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/
/*=======================================================  LOCAL VARIABLES  ==*/

static const NCOMPONENT_DEFINE("UART device driver", "Nenad Radulovic");

/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/
/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/


void np_uart_init(struct nuart_drv * uart_drv, const struct nuart_config * config)
{
    UART_HandleTypeDef *        huart;
    struct np_drv *             p_drv;

    p_drv = &uart_drv->p_drv;
    huart = &uart_drv->ctx.huart;
    huart->Instance          = (USART_TypeDef *)np_dev_address(np_drv_dev(p_drv));
    huart->Init.BaudRate     = config->baud_rate;
    huart->Init.HwFlowCtl    = UART_HWCONTROL_NONE;
    huart->Init.OverSampling = UART_OVERSAMPLING_16;

    switch (config->flags & NUART_WORDLENGTH) {
        case NUART_WORDLENGTH_8: {
            huart->Init.WordLength = UART_WORDLENGTH_8B;
            break;
        }
        case NUART_WORDLENGTH_9: {
            huart->Init.WordLength = UART_WORDLENGTH_9B;
            break;
        }
        default : {
            return;
        }
    }
    switch (config->flags & NUART_STOPBITS) {
        case NUART_STOPBITS_1: {
            huart->Init.StopBits = UART_STOPBITS_1;
            break;
        }
        case NUART_STOPBITS_2: {
            huart->Init.StopBits = UART_STOPBITS_2;
            break;
        }
        default: {
            NASSERT_ALWAYS(NAPI_USAGE);
            return;
        }
    }
    switch (config->flags & NUART_PARITY) {
        case NUART_PARITY_NONE: {
            huart->Init.Parity = UART_PARITY_NONE;
            break;
        }
        case NUART_PARITY_EVEN: {
            huart->Init.Parity = UART_PARITY_EVEN;
            break;
        }
        case NUART_PARITY_ODD: {
            huart->Init.Parity = UART_PARITY_ODD;
            break;
        }
        default: {
            NASSERT_ALWAYS(NAPI_USAGE);
            return;
        }
    }
    switch (config->flags & NUART_MODE) {
        case NUART_MODE_TX: {
            huart->Init.Mode = UART_MODE_TX;
            break;
        }
        case NUART_MODE_RX: {
            huart->Init.Mode = UART_MODE_RX;
            break;
        }
        case NUART_MODE_RX_TX: {
            huart->Init.Mode = UART_MODE_TX_RX;
            break;
        }
        default: {
            NASSERT_ALWAYS(NAPI_USAGE);
            return;
        }
    }
    np_drv_ref_up(p_drv);
    np_drv_clock_enable(p_drv, 0);
    uart_drv->gpios = config->gpios;
    uart_drv->flags = config->flags;

    if (config->flags & NUART_MODE_RX) {
        np_drv_mux_enable(p_drv, 0, uart_drv->gpios[NUART_MUX_FN_RX]);
    }

    if (config->flags & NUART_MODE_TX) {
        np_drv_mux_enable(p_drv, 0, uart_drv->gpios[NUART_MUX_FN_TX]);
    }

    if (config->flags & NUART_SET_ISR_PRIO) {
        np_drv_isr_set_prio(&uart_drv->p_drv, 0, config->isr_prio);
    } else {
        np_drv_isr_set_prio(&uart_drv->p_drv, 0, CONFIG_CORE_LOCK_MAX_LEVEL);
    }
    np_drv_isr_clear_flag(&uart_drv->p_drv, 0);
    np_drv_isr_enable(&uart_drv->p_drv, 0);
    HAL_UART_Init(huart);
}



void np_uart_term(
    struct nuart_drv *          uart_drv)
{
    UART_HandleTypeDef *        huart = &uart_drv->ctx.huart;
    struct np_drv *             p_drv = &uart_drv->p_drv;

    np_drv_isr_disable(&uart_drv->p_drv, 0);
    HAL_UART_DeInit(huart);

    if (uart_drv->flags & NUART_MODE_RX) {
        np_drv_mux_disable(p_drv, 0, uart_drv->gpios[NUART_MUX_FN_RX]);
    }

    if (uart_drv->flags & NUART_MODE_TX) {
        np_drv_mux_disable(p_drv, 0, uart_drv->gpios[NUART_MUX_FN_TX]);
    }
    np_drv_ref_down(&uart_drv->p_drv);
}



void np_uart_rx_start(
    struct nuart_drv *          uart_drv,
    void *                      data,
    size_t                      size)
{
    UART_HandleTypeDef *        huart = &uart_drv->ctx.huart;
    struct np_drv *             p_drv = &uart_drv->p_drv;

    np_drv_isr_disable(p_drv, 0);
    HAL_UART_Receive_IT(huart, data, size);
    np_drv_isr_enable(p_drv, 0);
}



void np_uart_rx_stop(
    struct nuart_drv *          uart_drv)
{
    UART_HandleTypeDef *        huart = &uart_drv->ctx.huart;
    struct np_drv *             p_drv = &uart_drv->p_drv;

    np_drv_isr_disable(p_drv, 0);
    __HAL_UART_DISABLE_IT(huart, UART_IT_RXNE);
    if(huart->State != HAL_UART_STATE_BUSY_TX) {
        if (huart->State == HAL_UART_STATE_BUSY_TX_RX) {
            huart->State = HAL_UART_STATE_BUSY_TX;
        } else {
            __HAL_UART_DISABLE_IT(huart, UART_IT_PE);
            __HAL_UART_DISABLE_IT(huart, UART_IT_ERR);
            huart->State = HAL_UART_STATE_READY;
        }
    }
    __HAL_UNLOCK(huart);
    np_drv_isr_enable(p_drv, 0);
}



void np_uart_tx_start(
    struct nuart_drv *          uart_drv,
    const void *                data,
    size_t                      size)
{
    UART_HandleTypeDef *        huart = &uart_drv->ctx.huart;
    struct np_drv *             p_drv = &uart_drv->p_drv;

    np_drv_isr_disable(p_drv, 0);
    /* NOTE:
     * STM32 HAL poorly handles const pointers, so an explicit cast is needed
     * here.
     */
    HAL_UART_Transmit_IT(huart, (uint8_t *)data, size);
    np_drv_isr_enable(p_drv, 0);
}



void np_uart_tx_stop(
    struct nuart_drv *          uart_drv)
{
    UART_HandleTypeDef *        huart = &uart_drv->ctx.huart;
    struct np_drv *             p_drv = &uart_drv->p_drv;

    /* NOTE:
     * STM32 HAL uses __HAL_LOCK()/__HAL_UNLOCK() macro pair to protect its
     * critical code sections. These macros can't be used in here since they
     * are returning a value, but this function returns void.
     */
    np_drv_isr_disable(p_drv, 0);
    __HAL_UART_DISABLE_IT(huart, UART_IT_RXNE);

    if(huart->State != HAL_UART_STATE_BUSY_TX) {
        if (huart->State == HAL_UART_STATE_BUSY_TX_RX) {
            huart->State = HAL_UART_STATE_BUSY_TX;
        } else {
            __HAL_UART_DISABLE_IT(huart, UART_IT_PE);
            __HAL_UART_DISABLE_IT(huart, UART_IT_ERR);
            huart->State = HAL_UART_STATE_READY;
        }
    }
    np_drv_isr_enable(p_drv, 0);
}



void HAL_UART_ErrorCallback(UART_HandleTypeDef * huart)
{
    struct nuart_drv *          drv;

    drv = CONTAINER_OF(huart, struct nuart_drv, ctx.huart);

    if (drv->reader) {
        drv->reader(
                drv,
                NERROR_DEVICE_FAIL,
                huart->pRxBuffPtr,
                huart->RxXferSize - huart->RxXferCount);
    }
}



void HAL_UART_RxFirstCallback(UART_HandleTypeDef * huart)
{
    struct nuart_drv *          drv;

    drv = CONTAINER_OF(huart, struct nuart_drv, ctx.huart);
}



void HAL_UART_RxCpltCallback(UART_HandleTypeDef * huart)
{
    struct nuart_drv *          drv;

    drv = CONTAINER_OF(huart, struct nuart_drv, ctx.huart);

    if (drv->reader) {
        drv->reader(drv, NERROR_NONE, huart->pRxBuffPtr, huart->RxXferSize);
    }
}



void HAL_UART_TxCpltCallback(UART_HandleTypeDef * huart)
{
    struct nuart_drv *          drv;

    drv = CONTAINER_OF(huart, struct nuart_drv, ctx.huart);

    if (drv->writer) {
        drv->writer(drv, NERROR_NONE, huart->pTxBuffPtr, huart->TxXferSize);
    }
}



#if (NPROFILE_EN_UART & NP_EN_MAJOR(1))
void USART1_IRQHandler(void);

void USART1_IRQHandler(void)
{
    struct nuart_drv *          drv;

    drv = CONTAINER_OF(np_dev_driver(&g_uart1), struct nuart_drv, p_drv);
    HAL_UART_IRQHandler(&drv->ctx.huart);
}
#endif /* (NP_EN_UART & NP_EN_MAJOR(1)) */



#if (NPROFILE_EN_UART & NP_EN_MAJOR(2))
void USART2_IRQHandler(void);

void USART2_IRQHandler(void)
{
    struct nuart_drv *          drv;

    drv = CONTAINER_OF(np_dev_driver(&g_uart2), struct nuart_drv, p_drv);
    HAL_UART_IRQHandler(&drv->ctx.huart);
}
#endif /* (NP_EN_UART & NP_EN_MAJOR(2)) */

#if (NPROFILE_EN_UART & NP_EN_MAJOR(6))
void USART6_IRQHandler(void);

void USART6_IRQHandler(void)
{
    struct nuart_drv *          drv;

    drv = CONTAINER_OF(np_dev_driver(&g_uart6), struct nuart_drv, p_drv);
    HAL_UART_IRQHandler(&drv->ctx.huart);
}
#endif /* (NP_EN_UART & NP_EN_MAJOR(6)) */

#endif /* (NPROFILE_EN_UART) */

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of uart_device.c
 ******************************************************************************/