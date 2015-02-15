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
#include "mcu/gpio.h"
#include "mcu/uart.h"

/*=========================================================  LOCAL MACRO's  ==*/
/*======================================================  LOCAL DATA TYPES  ==*/
/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/

void uart_timeout_handler(
    void *                      arg);



void uart_setup_transmit_i(
    struct nuart_driver *           drv);



void uart_stop_transmit_i(
    struct nuart_driver *           drv);



void uart_setup_receive_i(
    struct nuart_driver *           drv);


void uart_stop_receive_i(
    struct nuart_driver *           drv);

/*=======================================================  LOCAL VARIABLES  ==*/

static const NCOMPONENT_DEFINE("UART device driver", "Nenad Radulovic");

/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/


void uart_timeout_handler(void * arg)
{
    struct nuart_driver *           drv = arg;

    nuart_read_stop(drv);

    if (drv->reader) {
        UART_HandleTypeDef *    device_handle;

        device_handle = &drv->device.handle;
        drv->reader(
                drv,
                NERROR_TIMEOUT,
                drv->rx_buff,
                device_handle->RxXferSize - device_handle->RxXferCount);
    }
}



void uart_setup_transmit_i(
    struct nuart_driver *           drv)
{
    UART_HandleTypeDef *        device_handle;

    device_handle = &drv->device.handle;
    device_handle->pTxBuffPtr  = drv->tx_buff;
    device_handle->TxXferCount = drv->tx_size;
    device_handle->TxXferSize  = drv->tx_size;

    if (device_handle->State == HAL_UART_STATE_BUSY_RX) {
        device_handle->State = HAL_UART_STATE_BUSY_TX_RX;
    } else {
        device_handle->State = HAL_UART_STATE_BUSY_TX;
    }

    __HAL_UART_ENABLE_IT(device_handle, UART_IT_PE);
    __HAL_UART_ENABLE_IT(device_handle, UART_IT_ERR);
    __HAL_UART_ENABLE_IT(device_handle, UART_IT_TXE);
}



void uart_stop_transmit_i(
    struct nuart_driver *           drv)
{
    UART_HandleTypeDef *        device_handle;

    device_handle = &drv->device.handle;

    if (device_handle->State == HAL_UART_STATE_BUSY_TX_RX) {
        device_handle->State =  HAL_UART_STATE_BUSY_RX;
    } else {
        device_handle->State =  HAL_UART_STATE_READY;
        __HAL_UART_DISABLE_IT(device_handle, UART_IT_PE);
        __HAL_UART_DISABLE_IT(device_handle, UART_IT_ERR);
    }
    __HAL_UART_DISABLE_IT(device_handle, UART_IT_TXE);
}



void uart_setup_receive_i(
    struct nuart_driver *           drv)
{
    UART_HandleTypeDef *        device_handle;

    device_handle = &drv->device.handle;
    device_handle->pRxBuffPtr  = drv->rx_buff;
    device_handle->RxXferCount = drv->rx_size;
    device_handle->RxXferSize  = drv->rx_size;

    if (device_handle->State == HAL_UART_STATE_BUSY_TX) {
        device_handle->State = HAL_UART_STATE_BUSY_TX_RX;
    } else {
        device_handle->State = HAL_UART_STATE_BUSY_RX;
    }

    __HAL_UART_ENABLE_IT(device_handle, UART_IT_PE);
    __HAL_UART_ENABLE_IT(device_handle, UART_IT_ERR);
    __HAL_UART_ENABLE_IT(device_handle, UART_IT_RXNE);
}



void uart_stop_receive_i(
    struct nuart_driver *           drv)
{
    UART_HandleTypeDef *        device_handle;

    device_handle = &drv->device.handle;

    if (device_handle->State == HAL_UART_STATE_BUSY_TX_RX) {
        device_handle->State =  HAL_UART_STATE_BUSY_TX;
    } else {
        device_handle->State =  HAL_UART_STATE_READY;
        __HAL_UART_DISABLE_IT(device_handle, UART_IT_PE);
        __HAL_UART_DISABLE_IT(device_handle, UART_IT_ERR);
    }
    __HAL_UART_DISABLE_IT(device_handle, UART_IT_RXNE);
}

/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/


struct nuart_driver * nuart_open(
    const struct nperiph *      periph,
    const struct nuart_config * config)
{
    struct nuart_driver *       driver;
    UART_HandleTypeDef *        device_handle;

    NASSERT(NAPI_POINTER, periph != NULL);
    NASSERT(NAPI_OBJECT,  periph_class_id(periph) == NUART_DEVICE_CLASS_ID);

    driver         = periph_driver(periph);
    memset(driver, 0, sizeof(*driver));
    driver->periph = periph;

    device_handle = &driver->device.handle;
    device_handle->Instance          = (USART_TypeDef *)periph_address(periph);
    device_handle->Init.BaudRate     = config->baud_rate;
    device_handle->Init.HwFlowCtl    = UART_HWCONTROL_NONE;
    device_handle->Init.OverSampling = UART_OVERSAMPLING_16;

    switch (config->flags & NUART_WORDLENGTH) {
        case NUART_WORDLENGTH_8: {
            device_handle->Init.StopBits = UART_WORDLENGTH_8B;
            break;
        }
        case NUART_WORDLENGTH_9: {
            device_handle->Init.StopBits = UART_WORDLENGTH_9B;
            break;
        }
        default : {
            return (NULL);
        }
    }
    switch (config->flags & NUART_STOPBITS) {
        case NUART_STOPBITS_1: {
            device_handle->Init.StopBits = UART_STOPBITS_1;
            break;
        }
        case NUART_STOPBITS_2: {
            device_handle->Init.StopBits = UART_STOPBITS_2;
            break;
        }
        default: {
            return (NULL);
        }
    }
    switch (config->flags & NUART_PARITY) {
        case NUART_PARITY_NONE: {
            device_handle->Init.Parity = UART_PARITY_NONE;
            break;
        }
        case NUART_PARITY_EVEN: {
            device_handle->Init.Parity = UART_PARITY_EVEN;
            break;
        }
        case NUART_PARITY_ODD: {
            device_handle->Init.Parity = UART_PARITY_ODD;
            break;
        }
        default: {
            return (NULL);
        }
    }
    switch (config->flags & NUART_MODE) {
        case NUART_MODE_TX: {
            device_handle->Init.Mode = UART_MODE_TX;
            break;
        }
        case NUART_MODE_RX: {
            device_handle->Init.Mode = UART_MODE_RX;
            break;
        }
        case NUART_MODE_RX_TX: {
            device_handle->Init.Mode = UART_MODE_TX_RX;
            break;
        }
    }

    if (config->flags & NUART_MODE_RX) {
        ngpio_mux_init(config->rx_gpio, driver->periph->mux);
    }

    if (config->flags & NUART_MODE_TX) {
        ngpio_mux_init(config->tx_gpio, driver->periph->mux);
    }
    nperiph_clock_enable(driver->periph);

    if (config->flags & NUART_SET_ISR_PRIO) {
        nperiph_isr_set_prio(driver->periph, 0, config->isr_prio);
    } else {
        nperiph_isr_set_prio(driver->periph, 0, CONFIG_INTR_MAX_ISR_PRIO);
    }
    nperiph_isr_clear_flag(driver->periph, 0);
    nperiph_isr_enable(driver->periph, 0);

    if (HAL_UART_Init(device_handle) != HAL_OK) {
        return (NULL);
    }
    ntimer_init(&driver->timeout);

    return (driver);
}



void nuart_close(
    struct nuart_driver *       driver)
{
    uart_stop_transmit_i(driver);
    uart_stop_receive_i(driver);
    ntimer_cancel(&driver->timeout);
    HAL_UART_DeInit(&driver->device.handle);
    nperiph_isr_disable(driver->periph, 0);
    nperiph_clock_disable(driver->periph);
}



enum nerror nuart_read(
    struct nuart_driver *       driver,
    void *                      buffer,
    size_t                      size,
    nsystimer_tick              timeout)
{
    return (NERROR_NOT_IMPLEMENTED);
}



enum nerror nuart_write(
    struct nuart_driver *       driver,
    const void *                buffer,
    size_t                      size,
    nsystimer_tick              timeout)
{
    return (NERROR_NOT_IMPLEMENTED);
}



enum nerror nuart_read_start(
    struct nuart_driver *       driver,
    void *                      buffer,
    size_t                      size,
    uint32_t                    timeout)
{
    if ((driver->device.handle.State == HAL_UART_STATE_BUSY_RX) ||
        (driver->device.handle.State == HAL_UART_STATE_BUSY_TX_RX)) {
        return (NERROR_DEVICE_BUSY);
    }
    nperiph_isr_disable(driver->periph, 0);
    driver->timeout_ticks = timeout;
    driver->rx_buff       = buffer;
    driver->rx_size       = size;
    uart_setup_receive_i(driver);
    nperiph_isr_enable(driver->periph, 0);

    return (NERROR_NONE);
}



void nuart_read_restart_i(
    struct nuart_driver *       driver)
{
    uart_setup_receive_i(driver);
}



void nuart_read_stop(
    struct nuart_driver *       driver)
{
    nperiph_isr_disable(driver->periph, 0);
    uart_stop_receive_i(driver);
    nperiph_isr_enable(driver->periph, 0);
}



enum nerror nuart_write_start(
    struct nuart_driver *       driver,
    const void *                buffer,
    size_t                      size)
{
    if ((driver->device.handle.State == HAL_UART_STATE_BUSY_TX) ||
        (driver->device.handle.State == HAL_UART_STATE_BUSY_TX_RX)) {
        return (NERROR_DEVICE_BUSY);
    }
    nperiph_isr_disable(driver->periph, 0);
    driver->tx_buff = buffer;
    driver->tx_size = size;
    uart_setup_transmit_i(driver);
    nperiph_isr_enable(driver->periph, 0);

    return (NERROR_NONE);
}



void nuart_write_restart_i(
    struct nuart_driver *       driver)
{
    uart_setup_transmit_i(driver);
}



void nuart_write_stop(
    struct nuart_driver *       driver)
{
    nperiph_isr_disable(driver->periph, 0);
    uart_stop_transmit_i(driver);
    nperiph_isr_enable(driver->periph, 0);
}



void HAL_UART_ErrorCallback(UART_HandleTypeDef * device_handle)
{
    struct nuart_driver *           drv;
    struct nuart_device *       device;

    device = CONTAINER_OF(device_handle, struct nuart_device, handle);
    drv    = NUART_DEVICE_TO_DRV(device);

    if (drv->reader) {
        drv->reader(
                drv,
                NERROR_DEVICE_FAIL,
                drv->rx_buff,
                device_handle->RxXferSize - device_handle->RxXferCount);
    }
}



void HAL_UART_RxFirstCallback(UART_HandleTypeDef * device_handle)
{
    struct nuart_driver *       driver;
    struct nuart_device *       device;

    device = CONTAINER_OF(device_handle, struct nuart_device, handle);
    driver = NUART_DEVICE_TO_DRV(device);

    if (driver->timeout_ticks && driver->reader) {
        ntimer_start_i(
                &driver->timeout,
                driver->timeout_ticks,
                uart_timeout_handler,
                driver,
                NTIMER_ATTR_ONE_SHOT);
    }
}



void HAL_UART_RxCpltCallback(UART_HandleTypeDef * device_handle)
{
    struct nuart_driver *       driver;
    struct nuart_device *       device;

    device = CONTAINER_OF(device_handle, struct nuart_device, handle);
    driver = NUART_DEVICE_TO_DRV(device);

    if (driver->timeout_ticks) {
        ntimer_cancel_i(&driver->timeout);
    }

    if (driver->reader) {
        driver->reader(driver, NERROR_NONE, driver->rx_buff, driver->rx_size);
    }
}



void HAL_UART_TxCpltCallback(UART_HandleTypeDef * device_handle)
{
    struct nuart_driver *       driver;
    struct nuart_device *       device;

    device = CONTAINER_OF(device_handle, struct nuart_device, handle);
    driver = NUART_DEVICE_TO_DRV(device);

    if (driver->writer) {
        driver->writer(driver, NERROR_NONE, driver->tx_buff, driver->tx_size);
    }
}



#if defined(USART1) || defined(UART1)
void USART1_IRQHandler(void);

void USART1_IRQHandler(void)
{
    struct nuart_driver *       driver;

    driver = periph_driver(&g_uart1);
    HAL_UART_IRQHandler(&driver->device.handle);
}
#endif



#if defined(USART2) || defined(UART2)
void USART2_IRQHandler(void);

void USART2_IRQHandler(void)
{
    struct nuart_driver *       driver;

    driver = periph_driver(&g_uart2);
    HAL_UART_IRQHandler(&driver->device.handle);
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of uart_device.c
 ******************************************************************************/
