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
 * @brief       UART driver
 * @defgroup    uart UART driver
 * @brief       UART driver
 *********************************************************************//** @{ */

#ifndef NEON_DRIVERS_UART_H_
#define NEON_DRIVERS_UART_H_

/*=========================================================  INCLUDE FILES  ==*/


#include <stddef.h>
#include <stdint.h>

#include "port/compiler.h"
#include "shared/error.h"
#include "timer/timer.h"
#include "device/uart_device.h"

/*===============================================================  MACRO's  ==*/

#define NUART_DEVICE_TO_DRV(uart_device)                                        \
    CONTAINER_OF(uart_device, struct nuart_driver, device)

#define NUART_DEVICE_CLASS_ID           ((uint32_t)0xdead0001u)

#define NUART_WORDLENGTH_8              (0x1u << 0)
#define NUART_WORDLENGTH_9              (0x1u << 1)

#define NUART_STOPBITS_1                (0x1u << 2)
#define NUART_STOPBITS_1_5              (0x1u << 3)
#define NUART_STOPBITS_2                (0x1u << 4)

#define NUART_PARITY_NONE               (0x1u << 5)
#define NUART_PARITY_EVEN               (0x1u << 6)
#define NUART_PARITY_ODD                (0x1u << 7)

#define NUART_MODE_RX                   (0x1u << 8)
#define NUART_MODE_TX                   (0x1u << 9)
#define NUART_MODE_RX_TX                (NUART_MODE_RX | NUART_MODE_TX)

#define NUART_SET_ISR_PRIO              (0x1u << 15)

#define NUART_WORDLENGTH                                                        \
    (NUART_WORDLENGTH_8 | NUART_WORDLENGTH_9)

#define NUART_STOPBITS                                                          \
    (NUART_STOPBITS_1 | NUART_STOPBITS_1_5 | NUART_STOPBITS_2)

#define NUART_PARITY                                                            \
    (NUART_PARITY_NONE | NUART_PARITY_EVEN | NUART_PARITY_ODD)

#define NUART_MODE                                                              \
    (NUART_MODE_RX | NUART_MODE_TX | NUART_MODE_RX_TX)

/*-------------------------------------------------------  C++ extern base  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

struct nperiph;

struct nuart_config
{
    uint32_t                    flags;
    uint32_t                    baud_rate;
    uint32_t                    isr_prio;
    uint32_t                    tx_gpio;
    uint32_t                    rx_gpio;
};



struct nuart_driver
{
    const struct nperiph *      periph;
    struct nuart_device         device;
    const uint8_t *             tx_buff;
    size_t                      tx_size;
    uint8_t *                   rx_buff;
    size_t                      rx_size;
    void *                      priv_data;
    void                     (* reader)(struct nuart_driver *, enum nerror, void *, size_t);
    void                     (* writer)(struct nuart_driver *, enum nerror, const void *, size_t);
    nsystimer_tick              timeout_ticks;
    struct ntimer               timeout;
};

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/

struct nuart_driver * nuart_open(
    const struct nperiph *      peripheral,
    const struct nuart_config * config);



void nuart_close(
    struct nuart_driver *       driver);



enum nerror nuart_read(
    struct nuart_driver *       driver,
    void *                      buffer,
    size_t                      size,
    nsystimer_tick              timeout);



enum nerror nuart_write(
    struct nuart_driver *       driver,
    const void *                buffer,
    size_t                      size,
    nsystimer_tick              timeout);



void nuart_set_priv_data(
    struct nuart_driver *       driver,
    void *                      data);



PORT_C_INLINE
void * nuart_priv_data(
    struct nuart_driver *       driver)
{
    return (driver->priv_data);
}



void nuart_set_reader(
    struct nuart_driver *       driver,
    void                     (* reader)(struct nuart_driver *, enum nerror, void *, size_t));



void nuart_set_writer(
    struct nuart_driver *       driver,
    void                     (* writer)(struct nuart_driver *, enum nerror, const void *, size_t));


enum nerror nuart_read_start(
    struct nuart_driver *       driver,
    void *                      buffer,
    size_t                      size,
    uint32_t                    timeout);



void nuart_read_restart_i(
    struct nuart_driver *       driver);



void nuart_read_stop(
    struct nuart_driver *       driver);



enum nerror nuart_write_start(
    struct nuart_driver *       driver,
    const void *                buffer,
    size_t                      size);



void nuart_write_restart_i(
    struct nuart_driver *       driver);



void nuart_write_stop(
    struct nuart_driver *       driver);

/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of uart.h
 ******************************************************************************/
#endif /* NEON_DRIVERS_UART_H_ */
