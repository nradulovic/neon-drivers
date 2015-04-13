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
 * @brief       Peripheral management
 * @defgroup    periph_intf Peripheral management
 * @brief       Peripheral management
 *********************************************************************//** @{ */

#ifndef NEON_MCU_PERIPHERAL_H_
#define NEON_MCU_PERIPHERAL_H_

/*=========================================================  INCLUDE FILES  ==*/

#include <stdint.h>

#include "port/compiler.h"
#include "port/core.h"
#include "mcu/profile.h"

#include "family/p_dev.h"

/*===============================================================  MACRO's  ==*/

#define NP_DEV_RECOGNITION(class, instance)   { (class), (instance) }

#define NP_DEV_ID_TO_MINOR(id)          (((uint32_t)(id) >>  0u) & 0xffu)
#define NP_DEV_ID_TO_MAJOR(id)          (((uint32_t)(id) >>  8u) & 0xffu)
#define NP_DEV_ID_TO_CLASS(id)          (((uint32_t)(id) >> 16u) & 0xffffu)

#define NP_DEV_CREATE_ID(class, major, minor)                                   \
    ((((uint32_t)(class) & 0xffffu) << 16u) |                                   \
     (((uint32_t)(major) &   0xffu) <<  8u) |                                   \
     (((uint32_t)(minor) &   0xffu) <<  0u))



/*-------------------------------------------------------  C++ extern base  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

struct npdrv;
struct npdev_isr;
struct npdev_rst;
struct npdev_clk;
struct npdev_mux;

/**@brief		Peripheral driver
 */
struct npdrv
{
    const struct npdev *        pdev;
    ncore_atomic				ref;
    void *                      data;
};

/**@brief		Peripheral device
 */
struct npdev
{
    struct npdev_recognition
    {
        uint16_t                    dev_class;
        uint16_t                    dev_instance;
    }                           recognition;
    uint32_t                    flags;
    uint32_t					max_ref;
    struct npdrv *             	pdrv;
#if (NP_ATTR_ADDRESS == 1)
    volatile void *             address;
#endif
#if (NP_ATTR_ISR == 1)
    const struct npdev_isr *    isr;
#endif
#if (NP_ATTR_RST == 1)
    const struct npdev_rst *    rst;
#endif
#if (NP_ATTR_PWR == 1)
    const struct npdev_pwr *  	pwr;
#endif
#if (NP_ATTR_CLK == 1)
    const struct npdev_clk * 	clk;
#endif
#if (NP_ATTR_MUX == 1)
    const struct npdev_mux *    mux;
#endif
};

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/

#define npdev_to_pdrv(pdev)				(pdev)->pdrv
#define npdev_instance(pdev) 			((pdev)->recognition.dev_intance)
#define npdev_class(pdev)               ((pdev)->recognition.dev_class)
#define npdev_flags(pdev)             	(pdev)->flags
#define npdev_address(pdev)           	(pdev)->address
#define npdev_isr(pdev, no)			    &((pdev)->isr[no])
#define npdev_rst(pdev, no)			    &((pdev)->rst[no])
#define npdev_pwr(pdev, no)		        &((pdev)->pwr[no])
#define npdev_clk(pdev, no)		    	&((pdev)->clk[no])
#define npdev_mux(pdev, no)             &((pdev)->mux[no])

#define npdrv_to_pdev(pdrv)             (pdrv)->pdev
#define npdrv_instance(pdrv)			npdev_instance(npdrv_to_pdev(pdrv))
#define npdrv_class(pdrv)				npdev_class(npdrv_to_pdev(pdrv))
#define npdrv_flags(pdrv)				npdev_flags(npdrv_to_pdev(pdrv))
#define npdrv_address(pdrv)				npdev_address(npdrv_to_pdev(pdrv))
#define npdrv_isr(pdrv, no)				npdev_isr(npdrv_to_pdev(pdrv), (no))
#define npdrv_rst(pdrv, no)				npdev_rst(npdrv_to_pdev(pdrv), (no))
#define npdrv_pwr(pdrv, no)				npdev_pwr(npdrv_to_pdev(pdrv), (no))
#define npdrv_clk(pdrv, no)				npdev_clk(npdrv_to_pdev(pdrv), (no))
#define npdrv_mux(pdrv, no)				npdev_mux(npdrv_to_pdev(pdrv), (no))

#define npdrv_isr_enable(pdrv, no)      np_isr_enable(npdrv_isr((pdrv), (no)))

#define npdrv_isr_disable(pdrv, no)     np_isr_disable(npdrv_isr((pdrv), (no)))

#define npdrv_isr_clear(pdrv, no)       np_isr_clear_flag(npdrv_isr((pdrv), (no)))

#define npdrv_isr_set(pdrv, no)         np_isr_set_flag(npdrv_isr((pdrv), (no)))

#define npdrv_isr_set_prio(pdrv, no, prio)                                      \
    np_isr_set_prio(npdrv_isr((pdrv), (no)), prio)

#define npdrv_isr_get_prio(pdrv, no)    np_isr_get_prio(npdrv_isr((pdrv), (no)))

#define npdrv_pwr_enable(pdrv, no)      np_pwr_enable(npdrv_pwr((pdrv), (no)))

#define npdrv_pwr_disable(pdrv, no)     np_pwr_disable(npdrv_pwr((pdrv), (no)))

#define npdrv_mux_setup(pdrv, no, pin_id)                                       \
    np_mux_enable(npdrv_mux((pdrv), (no)), (pin_id))

#define npdrv_mux_reset(pin_id)                                     			\
    np_mux_disable(pin_id)

struct npdrv * npdrv_request(uint32_t dev_id);

void npdrv_release(struct npdrv * pdrv);

#define npdrv_set_data(pdrv, priv_data) (pdrv)->data = (priv_data)

#define npdrv_get_data(pdrv)            (pdrv)->data

void np_pwr_enable(const struct npdev_pwr * pwr);
void np_pwr_disable(const struct npdev_pwr * pwr);
void np_isr_enable(const struct npdev_isr * isr);
void np_isr_disable(const struct npdev_isr * isr);
void np_isr_clear_flag(const struct npdev_isr * isr);
void np_isr_set_flag(const struct npdev_isr * isr);
void np_isr_set_prio(const struct npdev_isr * isr, uint32_t prio);
uint32_t np_isr_get_prio(const struct npdev_isr * isr);
void np_mux_enable(const struct npdev_mux * mux, uint32_t pin_id);
void np_mux_disable(uint32_t pin_id);

/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of peripheral.h
 ******************************************************************************/
#endif  /* NEON_MCU_PERIPHERAL_H_ */

