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
 * @defgroup    uart_intf UART driver
 * @brief       UART driver
 *********************************************************************//** @{ */

#ifndef NEON_PORT_UART_H_
#define NEON_PORT_UART_H_

/*=========================================================  INCLUDE FILES  ==*/

#include <stddef.h>
#include <stdint.h>

#include "port/compiler.h"
#include "base/error.h"
#include "mcu/peripheral.h"
#include "mcu/profile.h"

/* NOTE:
 * Include family port header file. The file defines custom data types which are
 * needed by the driver.
 */
#include "family/p_uart.h"

/*===============================================================  MACRO's  ==*/

#define NUART_MUX_FN_RX                 0
#define NUART_MUX_FN_TX                 1

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

#define NUART_STATE_READY               (0x0)
#define NUART_STATE_BUSY_RX             (0x1u << 0)
#define NUART_STATE_BUSY_TX             (0x1u << 1)

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

struct nuart_config
{
    uint32_t                    flags;
    uint32_t                    baud_rate;
    uint32_t                    isr_prio;
    const uint32_t *            gpios;
};

/**@brief       UART driver structure
 * @details     The structure is instantiated by profile data source file,
 *              one structure per peripheral.
 */
struct nuart_drv
{
    struct npdrv                pdrv;  /**<@brief Generic peripheral driver
                                         * structure
                                         */
    struct np_dev_uart          ctx;    /**<@brief Custom data type which is
                                         * defined in family port header file.
                                         */
    const uint32_t *            gpios;
    uint32_t                    flags;
    uint32_t                    state;
    void                     (* reader)(struct nuart_drv *, enum nerror, void *, size_t);
    void                     (* writer)(struct nuart_drv *, enum nerror, const void *, size_t);
};

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/

nerror nuart_init(uint32_t uart_id, const struct nuart_config * config);


void nuart_term(
    struct nuart_drv *          drv);



void nuart_rx_start(
    struct nuart_drv *          drv,
    void *                      data,
    size_t                      size);



void np_uart_rx_restart(
    struct nuart_drv *          drv);



void nuart_rx_stop(
    struct nuart_drv *          drv);



void nuart_tx_start(
    struct nuart_drv *          uart_drv,
    const void *                data,
    size_t                      size);



void np_uart_tx_restart(
    struct nuart_drv *          drv);



void nuart_tx_stop(
    struct nuart_drv *          drv);

/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of uart.h
 ******************************************************************************/
#endif /* NEON_PORT_UART_H_ */
