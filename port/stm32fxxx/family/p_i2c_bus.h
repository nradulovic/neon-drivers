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
 * @brief       I2C header for STM32Fxxx port
 *********************************************************************//** @{ */

#ifndef FAMILY_P_I2C_BUS_H_
#define FAMILY_P_I2C_BUS_H_

/*=========================================================  INCLUDE FILES  ==*/

#include "mcu/profile.h"
#include "mcu/peripheral.h"
#include "mcu/profile.h"
#include "neon_eds.h"

/*===============================================================  MACRO's  ==*/

#define NI2C_DUALADDRESS_DISABLE        (0u << 0)
#define NI2C_DUALADDRESS_ENABLE         (1u << 0)
#define NI2C_FM_DUTYCYCLE_2             (0u << 1)
#define NI2C_FM_DUTYCYCLE_16_9          (1u << 1)
#define NI2C_NOSTRETCH_DISABLE          (0u << 2)
#define NI2C_NOSTRETCH_ENABLE           (1u << 2)
#define NI2C_OWN_ADDRESS_2(x)           ((x) << 8)

#define I2C_DUAL_ADDRESS                                                        \
    (NI2C_DUALADDRESS_DISABLE | NI2C_DUALADDRESS_ENABLE)

#define I2C_FM_DUTYCYCLE                                                        \
    (NI2C_FM_DUTYCYCLE_2 | NI2C_FM_DUTYCYCLE_16_9)

#define I2C_NOSTRETCH                                                           \
    (NI2C_NOSTRETCH_DISABLE | NI2C_NOSTRETCH_ENABLE)

/*-------------------------------------------------------  C++ extern base  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

struct np_dev_i2c
{
    I2C_HandleTypeDef    handle;
};

/*======================================================  GLOBAL VARIABLES  ==*/

void ni2c_port_config(
    uint32_t                    bus_id,
    uint32_t                    config);



void ni2c_port_analog_filter_config(
    uint32_t                    bus_id,
    uint32_t                    config);



void ni2c_port_digital_filter_config(
    uint32_t                    bus_id,
    uint32_t                    config);

/*===================================================  FUNCTION PROTOTYPES  ==*/
/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of p_i2c.h
 ******************************************************************************/
#endif /* FAMILY_P_I2C_H_ */
