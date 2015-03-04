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

#ifndef NEON_DRIVERS_PERIPHERAL_H_
#define NEON_DRIVERS_PERIPHERAL_H_

/*=========================================================  INCLUDE FILES  ==*/

#include <device/p_dev.h>
#include <stdint.h>


/*===============================================================  MACRO's  ==*/

#define NPERIPH_MAGIC                   ((uint32_t)0xdead0000u)
#define NPERIPH_DEV_ID(class, id)       (NPERIPH_MAGIC | (((class) & 0xffu) << 8) | ((id) & 0xffu))

#define NPERIPH_MAJOR_ID(id)            ((uint32_t)id >> 16u)
#define NPERIPH_MINOR_ID(id)            ((uint32_t)id & 0xffffu)
#define NPERIPH_ID(major, minor)        (((uint32_t)major << 16u) | ((uint32_t)minor & 0xffffu))

/*-------------------------------------------------------  C++ extern base  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

struct np_drv;
struct np_dev_isr;
struct np_dev_rst;
struct np_dev_clock;
struct np_dev_mux;

struct np_drv
{
    const struct np_dev *       p_dev;
    uint32_t                    ref;
    void *                      data;
};

struct np_drv_id
{
    const struct np_dev *       p_dev;
    struct np_drv *             p_drv;
};

struct np_dev
{
    uint32_t                    id;
    uint32_t                    flags;
    struct np_drv *             p_drv;
    const struct np_dev *       host;
#if (ARCH_ATTR_ADDRESS == 1)
    volatile void *             address;
#endif
#if (ARCH_ATTR_ISR == 1)
    const struct np_dev_isr *   isr;
#endif
#if (ARCH_ATTR_RESET == 1)
    const struct np_dev_rst *   rst;
#endif
#if (ARCH_ATTR_CLOCK == 1)
    const struct np_dev_clock * clock;
#endif
#if (ARCH_ATTR_MUX == 1)
    const struct np_dev_mux *   mux;
#endif
};

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/


#define np_dev_id(device)                                                       \
    (((device)->id >> 0u) & 0xffu)

#define np_dev_class_id(device)                                                 \
    (((device)->id >> 8u) & 0xffu)

#define np_dev_flags(device)                                                    \
    (device)->flags

#define np_dev_driver(device)                                                   \
    (device)->p_drv

#define np_dev_address(device)                                                  \
    (device)->address

#define np_dev_mux(device)                                                      \
    (device)->mux

static inline
const struct np_dev * np_dev_find_by_major(const struct np_dev * dev_class[], uint32_t id)
{
    const struct np_dev *       dev;
    uint32_t                    major;

    major = NPERIPH_MAJOR_ID(id);
    dev   = dev_class[major];

    if (dev) {

        if (dev->id != major) {

        }
    }

    return (dev);
}

#define np_drv_dev(drv)                 (drv)->p_dev

struct np_drv * np_drv_get(
    const struct np_dev *       dev);

void np_drv_put(
    struct np_drv *             drv);

#define np_drv_set_data(drv, priv_data)                                         \
    (drv)->data = (priv_data)

#define np_drv_get_data(drv)                                                    \
    (drv)->data

#define np_drv_ref_up(drv)                                                      \
    (++(drv)->ref)

#define np_drv_ref_down(drv)                                                    \
    (--(drv)->ref)

#define np_drv_clock_enable(drv, num)                                           \
    np_clock_enable(&(drv)->p_dev->clock[num])

#define np_drv_clock_disable(drv, num)                                          \
    np_clock_disable(&(drv)->p_dev->clock[num])

#define np_drv_isr_enable(drv, num)                                             \
    np_isr_enable(&(drv)->p_dev->isr[num])

#define np_drv_isr_disable(drv, num)                                            \
    np_isr_disable(&(drv)->p_dev->isr[num])

#define np_drv_isr_clear_flag(drv, num)                                         \
    np_isr_clear_flag(&(drv)->p_dev->isr[num])

#define np_drv_isr_set_flag(drv, num)                                           \
    np_isr_set_flag(&(drv)->p_dev->isr[num])

#define np_drv_isr_set_prio(drv, num, prio)                                     \
    np_isr_set_prio(&(drv)->p_dev->isr[num], prio)

#define np_drv_isr_get_prio(drv, num)                                           \
    np_isr_get_prio(&(drv)->p_dev->isr[num])

#define np_drv_mux_enable(drv, num, pin_id)                                     \
    np_mux_enable(&(drv)->p_dev->mux[num], pin_id)

#define np_drv_mux_disable(drv, num, pin_id)                                    \
    np_mux_disable(&(drv)->p_dev->mux[num], pin_id)

void np_clock_enable(const struct np_dev_clock * clock);
void np_clock_disable(const struct np_dev_clock * clock);
void np_isr_enable(const struct np_dev_isr * isr);
void np_isr_disable(const struct np_dev_isr * isr);
void np_isr_clear_flag(const struct np_dev_isr * isr);
void np_isr_set_flag(const struct np_dev_isr * isr);
void np_isr_set_prio(const struct np_dev_isr * isr, uint32_t prio);
uint32_t np_isr_get_prio(const struct np_dev_isr * isr);
void np_mux_enable(const struct np_dev_mux * mux, uint32_t pin_id);
void np_mux_disable(const struct np_dev_mux * mux, uint32_t pin_id);

/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of peripheral.h
 ******************************************************************************/
#endif  /* NEON_DRIVERS_PERIPHERAL_H_ */

