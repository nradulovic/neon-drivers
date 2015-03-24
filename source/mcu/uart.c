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

#include "shared/component.h"
#include "shared/debug.h"
#include "mcu/uart.h"

/*=========================================================  LOCAL MACRO's  ==*/
/*======================================================  LOCAL DATA TYPES  ==*/
/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/


static void timeout_handler(void * arg);

/*=======================================================  LOCAL VARIABLES  ==*/

static const NCOMPONENT_DEFINE("UART device driver", "Nenad Radulovic");

/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/
/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/

struct nuart_drv * nuart_open(
    const struct np_dev *       dev,
    const struct nuart_config * config)
{
    struct nuart_drv *          drv;

    NASSERT(NAPI_POINTER, dev != NULL);
    NASSERT(NAPI_OBJECT,  np_dev_class_id(dev) == NUART_DEVICE_CLASS_ID);

    drv = CONTAINER_OF(np_drv_get(dev), struct nuart_drv, p_drv);

    NASSERT(NAPI_USAGE, (config->flags & dev->flags) == config->flags);
    drv->cfg_flags      = config->flags;
    drv->state          = 0u;
    drv->tx_buff        = NULL;
    drv->tx_size        = 0u;
    drv->rx_buff        = NULL;
    drv->rx_size        = 0u;
    drv->reader         = NULL;
    drv->writer         = NULL;
    drv->timeout_ticks  = 0u;
    np_uart_init(drv, config);
    ntimer_init(&drv->rx_timeout);

    return (drv);
}

void nuart_close(
    struct nuart_drv *          drv)
{
    ntimer_cancel(&drv->rx_timeout);
    np_uart_term(drv);
    drv->state          = 0u;
    drv->cfg_flags      = 0u;
    drv->cfg_tx_gpio    = 0u;
    drv->cfg_rx_gpio    = 0u;
    drv->tx_buff        = NULL;
    drv->tx_size        = 0u;
    drv->rx_buff        = NULL;
    drv->rx_size        = 0u;
    drv->reader         = NULL;
    drv->writer         = NULL;
    drv->timeout_ticks  = 0u;
    np_drv_put(&drv->p_drv);
}



enum nerror nuart_read(
    struct nuart_drv *          drv,
    void *                      buffer,
    size_t                      size)
{
    enum nerror                 error;

    drv->reader = NULL;

    error = nuart_read_start(drv, buffer, size);

    return (error);
}



enum nerror nuart_write(
    struct nuart_drv *          drv,
    const void *                buffer,
    size_t                      size)
{
    enum nerror                 error;

    drv->writer = NULL;

    error = nuart_write_start(drv, buffer, size);

    return (error);
}



void nuart_set_reader(
    struct nuart_drv *          drv,
    void                     (* reader)(struct nuart_drv *, enum nerror, void *, size_t))
{
    drv->reader = reader;
}



void nuart_set_writer(
    struct nuart_drv *          drv,
    void                     (* writer)(struct nuart_drv *, enum nerror, const void *, size_t))
{
    drv->writer = writer;
}



enum nerror nuart_read_start(
    struct nuart_drv *          drv,
    void *                      buffer,
    size_t                      size)
{
    /*
     * TODO: use spinlock here
     */
    if (drv->state & NUART_MODE_RX) {
        return (NERROR_DEVICE_BUSY);
    }
    drv->state        |= NUART_MODE_RX;
    drv->rx_buff       = buffer;
    drv->rx_size       = size;
    np_uart_rx_start(drv);

    return (NERROR_NONE);
}



void nuart_read_restart_isr(
    struct nuart_drv *          drv)
{
    drv->state |= NUART_MODE_RX;
    np_uart_rx_restart(drv);
}



void nuart_read_stop(
    struct nuart_drv *          drv)
{
    np_uart_rx_stop(drv);
}



enum nerror nuart_write_start(
    struct nuart_drv *          drv,
    const void *                buffer,
    size_t                      size)
{
    /*
     * TODO: use spinlock here
     */
    if (drv->state & NUART_MODE_TX) {
        return (NERROR_DEVICE_BUSY);
    }
    drv->state  |= NUART_MODE_TX;
    drv->tx_buff = buffer;
    drv->tx_size = size;
    np_uart_tx_start(drv);

    return (NERROR_NONE);
}



void nuart_write_restart_i(
    struct nuart_drv *          drv)
{
    drv->state |= NUART_MODE_TX;
    np_uart_tx_restart(drv);
}



void nuart_write_stop(
    struct nuart_drv *          drv)
{
    np_uart_tx_stop(drv);
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of i2c.c
 ******************************************************************************/
