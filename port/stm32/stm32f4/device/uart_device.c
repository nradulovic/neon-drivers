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

#include <string.h>

#include "port/sys_lock.h"
#include "shared/debug.h"
#include "shared/component.h"
#include "mcu/peripheral.h"
#include "mcu/profile.h"
#include "mcu/uart.h"

/*=========================================================  LOCAL MACRO's  ==*/
/*======================================================  LOCAL DATA TYPES  ==*/
/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/

void uart_timeout_handler(
    void *                      arg);



void uart_setup_transmit_i(
    struct nuart_drv *          drv);



void uart_stop_transmit_i(
    struct nuart_drv *          drv);



void uart_setup_receive_i(
    struct nuart_drv *          drv);


void uart_stop_receive_i(
    struct nuart_drv *          drv);

/*=======================================================  LOCAL VARIABLES  ==*/

static const NCOMPONENT_DEFINE("UART device driver", "Nenad Radulovic");

/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/


void uart_timeout_handler(void * arg)
{
    struct nuart_drv *          drv = arg;

    nuart_read_stop(drv);

    if (drv->reader) {
        UART_HandleTypeDef *    huart;

        huart = &drv->ctx.huart;
        drv->reader(
                drv,
                NERROR_TIMEOUT,
                drv->rx_buff,
                huart->RxXferSize - huart->RxXferCount);
    }
}



void uart_setup_transmit_i(
    struct nuart_drv *          drv)
{
    UART_HandleTypeDef *        huart;

    huart = &drv->ctx.huart;
    huart->pTxBuffPtr  = drv->tx_buff;
    huart->TxXferCount = drv->tx_size;
    huart->TxXferSize  = drv->tx_size;

    if (huart->State == HAL_UART_STATE_BUSY_RX) {
        huart->State = HAL_UART_STATE_BUSY_TX_RX;
    } else {
        huart->State = HAL_UART_STATE_BUSY_TX;
    }

    __HAL_UART_ENABLE_IT(huart, UART_IT_PE);
    __HAL_UART_ENABLE_IT(huart, UART_IT_ERR);
    __HAL_UART_ENABLE_IT(huart, UART_IT_TXE);
}



void uart_stop_transmit_i(
    struct nuart_drv *          drv)
{
    UART_HandleTypeDef *        huart;

    huart = &drv->ctx.huart;

    if (huart->State == HAL_UART_STATE_BUSY_TX_RX) {
        huart->State =  HAL_UART_STATE_BUSY_RX;
    } else {
        huart->State =  HAL_UART_STATE_READY;
        __HAL_UART_DISABLE_IT(huart, UART_IT_PE);
        __HAL_UART_DISABLE_IT(huart, UART_IT_ERR);
    }
    __HAL_UART_DISABLE_IT(huart, UART_IT_TXE);
}



void uart_setup_receive_i(
    struct nuart_drv *          drv)
{
    UART_HandleTypeDef *        huart;

    huart = &drv->ctx.huart;
    huart->pRxBuffPtr  = drv->rx_buff;
    huart->RxXferCount = drv->rx_size;
    huart->RxXferSize  = drv->rx_size;

    if (huart->State == HAL_UART_STATE_BUSY_TX) {
        huart->State = HAL_UART_STATE_BUSY_TX_RX;
    } else {
        huart->State = HAL_UART_STATE_BUSY_RX;
    }

    __HAL_UART_ENABLE_IT(huart, UART_IT_PE);
    __HAL_UART_ENABLE_IT(huart, UART_IT_ERR);
    __HAL_UART_ENABLE_IT(huart, UART_IT_RXNE);
}



void uart_stop_receive_i(
    struct nuart_drv *          drv)
{
    UART_HandleTypeDef *        huart;

    huart = &drv->ctx.huart;

    if (huart->State == HAL_UART_STATE_BUSY_TX_RX) {
        huart->State =  HAL_UART_STATE_BUSY_TX;
    } else {
        huart->State =  HAL_UART_STATE_READY;
        __HAL_UART_DISABLE_IT(huart, UART_IT_PE);
        __HAL_UART_DISABLE_IT(huart, UART_IT_ERR);
    }
    __HAL_UART_DISABLE_IT(huart, UART_IT_RXNE);
}

/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/


void np_uart_init(struct nuart_drv * drv, const struct nuart_config * config)
{
    UART_HandleTypeDef *        huart;

    np_drv_ref_up(&drv->p_drv);

    huart = &drv->ctx.huart;
    huart->Instance          = (USART_TypeDef *)np_dev_address(np_drv_dev(&drv->p_drv));
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
    np_drv_ref_up(&drv->p_drv);

    if (config->flags & NUART_MODE_RX) {
#define NMUX_UART_RX 1

        uint32_t nmux_find_gpio_id(uint32_t io_periph_class, uint32_t major, uint32_t minor);

        uint32_t rx_gpio;

        rx_gpio = nmux_find_gpio_id(NMUX_UART_RX, np_dev_id(&drv->p_drv->p_dev), 0);
#if 0
        np_drv_mux_enable(&drv->p_drv, 0, config->rx_gpio);
#endif
    }

    if (config->flags & NUART_MODE_TX) {
#if 0
        np_drv_mux_enable(&drv->p_drv, 0, config->tx_gpio);
#endif
    }
    np_drv_clock_enable(&drv->p_drv, 0);

    if (config->flags & NUART_SET_ISR_PRIO) {
        np_drv_isr_set_prio(&drv->p_drv, 0, config->isr_prio);
    } else {
        np_drv_isr_set_prio(&drv->p_drv, 0, CONFIG_ISR_MAX_PRIO);
    }
    np_drv_isr_clear_flag(&drv->p_drv, 0);
    np_drv_isr_enable(&drv->p_drv, 0);
    HAL_UART_Init(huart);
}



void np_uart_term(
    struct nuart_drv *          drv)
{
    np_drv_isr_disable(&drv->p_drv, 0);
    np_drv_ref_down(&drv->p_drv);
}



void np_uart_rx_start(
    struct nuart_drv *          drv)
{
    np_drv_isr_disable(&drv->p_drv, 0);
    uart_setup_receive_i(drv);
    np_drv_isr_enable(&drv->p_drv, 0);
}



void np_uart_rx_restart(
    struct nuart_drv *          drv)
{
    uart_setup_receive_i(drv);
}



void np_uart_rx_stop(
    struct nuart_drv *          drv)
{
    np_drv_isr_disable(&drv->p_drv, 0);
    uart_stop_receive_i(drv);
    np_drv_isr_enable(&drv->p_drv, 0);
}



void np_uart_tx_start(
    struct nuart_drv *          drv)
{
    np_drv_isr_disable(&drv->p_drv, 0);
    uart_setup_transmit_i(drv);
    np_drv_isr_enable(&drv->p_drv, 0);
}



void np_uart_tx_restart(
    struct nuart_drv *          drv)
{
    uart_setup_transmit_i(drv);
}



void np_uart_tx_stop(
    struct nuart_drv *          drv)
{
    np_drv_isr_disable(&drv->p_drv, 0);
    uart_stop_transmit_i(drv);
    np_drv_isr_enable(&drv->p_drv, 0);
}



void HAL_UART_ErrorCallback(UART_HandleTypeDef * huart)
{
    struct nuart_drv *          drv;

    drv = CONTAINER_OF(huart, struct nuart_drv, ctx.huart);

    if (drv->reader) {
        drv->reader(
                drv,
                NERROR_DEVICE_FAIL,
                drv->rx_buff,
                huart->RxXferSize - huart->RxXferCount);
    }
}



void HAL_UART_RxFirstCallback(UART_HandleTypeDef * huart)
{
    struct nuart_drv *          drv;

    drv = CONTAINER_OF(huart, struct nuart_drv, ctx.huart);

    if (drv->timeout_ticks && drv->reader) {
        ntimer_start_i(
                &drv->rx_timeout,
                drv->timeout_ticks,
                uart_timeout_handler,
                drv,
                NTIMER_ATTR_ONE_SHOT);
    }
}



void HAL_UART_RxCpltCallback(UART_HandleTypeDef * huart)
{
    struct nuart_drv *          drv;

    drv = CONTAINER_OF(huart, struct nuart_drv, ctx.huart);

    if (drv->timeout_ticks) {
        ntimer_cancel_i(&drv->rx_timeout);
    }

    if (drv->reader) {
        drv->reader(drv, NERROR_NONE, drv->rx_buff, drv->rx_size);
    }
}



void HAL_UART_TxCpltCallback(UART_HandleTypeDef * huart)
{
    struct nuart_drv *          drv;

    drv = CONTAINER_OF(huart, struct nuart_drv, ctx.huart);

    if (drv->writer) {
        drv->writer(drv, NERROR_NONE, drv->tx_buff, drv->tx_size);
    }
}



#if defined(USART1) || defined(UART1)
void USART1_IRQHandler(void);

void USART1_IRQHandler(void)
{
    struct nuart_drv *          drv;

    drv = CONTAINER_OF(np_dev_driver(&g_uart1), struct nuart_drv, p_drv);
    HAL_UART_IRQHandler(&drv->ctx.huart);
}
#endif



#if defined(USART2) || defined(UART2)
void USART2_IRQHandler(void);

void USART2_IRQHandler(void)
{
    struct nuart_drv *          drv;

    drv = CONTAINER_OF(np_dev_driver(&g_uart2), struct nuart_drv, p_drv);
    HAL_UART_IRQHandler(&drv->ctx.huart);
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of uart_device.c
 ******************************************************************************/
