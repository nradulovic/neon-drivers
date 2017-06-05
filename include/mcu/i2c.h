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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Neon. If not, see <http://www.gnu.org/licenses/>.
 *
 * web site: http://github.com/nradulovic
 * e-mail : nenad.b.radulovic@gmail.com
 *//***********************************************************************//**
 * @file
 * @author dejan
 * @brief 
 * @defgroup 
 * @brief 
 *********************************************************************//** @{ */

#ifndef PLATFORM_NEON_DRIVERS_INCLUDE_MCU_I2C_H_
#define PLATFORM_NEON_DRIVERS_INCLUDE_MCU_I2C_H_

/*=========================================================  INCLUDE FILES  ==*/

#include <stdint.h>
#include <stddef.h>

#include "base/error.h"
#include "mcu/i2c_bus.h"
#include "family/p_i2c_bus.h"

/*===============================================================  MACRO's  ==*/

#define NI2C_CONFIG_MODE_MASTER         (0x0u << 0)
#define NI2C_CONFIG_MODE_SLAVE          (0x1u << 0)

#define NI2C_CONFIG_ADDRESS_7BIT        (0x0u << 1)
#define NI2C_CONFIG_ADDRESS_10BIT       (0x1u << 1)

#define NI2C_CONFIG_SPEED_100           (0x0u << 4)
#define NI2C_CONFIG_SPEED_400           (0x1u << 4)
#define NI2C_CONFIG_SPEED_1700          (0x2u << 4)
#define NI2C_CONFIG_SPEED_3400          (0x3u << 4)

#define NI2C_CONFIG_HANDLING_IT         (0x0u << 6)
#define NI2C_CONFIG_HANDLING_DMA        (0x1u << 6)

#define NI2C_CONFIG_REG_SIZE_0          (0u << 0)
#define NI2C_CONFIG_REG_SIZE_1          (1u << 0)
#define NI2C_CONFIG_REG_SIZE_2          (2u << 0)
#define NI2C_CONFIG_REG_SIZE_3          (3u << 0)
#define NI2C_CONFIG_REG_SIZE_4          (4u << 0)

/*------------------------------------------------------  C++ extern begin  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

enum ni2c_basic_genral_call_commands
{
    NI2C_GENERAL_RESET =        0x06,
    NI2C_LATCH_ITS_ADDRESS =    0x04
};



enum ni2c_evt_id
{
    EVT_NI2C_CONFIG =           CONFIG_I2C_EVENT_BASE_ID,
    EVT_NI2C_WRITE_SLAVE,
    EVT_NI2C_READ_SLAVE,
    EVT_NI2C_COMBINED_TRANSFER,
    EVT_NI2C_OPEN_SLAVE,
    EVT_NI2C_GENERAL_CALL,
    EVT_NI2C_TRANSFER_COMPLETED,
    EVT_NI2C_ERROR,
};



enum ni2c_error_id
{
    NI2C_BUS_ERROR,
    NI2C_ACKNOWLEDGE_FAILURE,
    NI2C_ARBITRATION_LOST,
    NI2C_OVERFLOW,
    NI2C_TIMEOUT
};



struct ni2c_config_event
{
    nevent                      event;
    uint32_t                    config;
};



struct ni2c_transfer_event
{
    nevent                      event;
    struct ni2c_slave *         slave;
    uint32_t                    reg;
    void *                      data;
    size_t                      size;
};



struct ni2c_general_call_event
{
    nevent                      event;
    uint8_t                     command;
};



struct ni2c_open_event
{
    nevent                      event;
    struct ni2c_slave *         slave;
    uint32_t                    flags;
    uint32_t                    address;
};



struct ni2c_transfer_completed_event
{
    nevent                      event;
    uint32_t                    retries;
};



struct ni2c_error_event
{
    nevent                      event;
    uint32_t                    retries;
    enum ni2c_error_id          error_id;
};

/*======================================================  GLOBAL VARIABLES  ==*/

#if (NPROFILE_EN_I2C & NPROFILE_EN(1))
extern struct nepa              g_ni2c1_epa;
extern const struct nepa_define g_ni2c1_define;
#endif

#if (NPROFILE_EN_I2C & NPROFILE_EN(2))
extern const struct nepa_define g_ni2c2_define;
extern struct nepa              g_ni2c2_epa;
#endif

#if (NPROFILE_EN_I2C & NPROFILE_EN(3))
extern const struct nepa_define g_ni2c3_define;
extern struct nepa              g_ni2c3_epa;
#endif

#if (NPROFILE_EN_I2C & NPROFILE_EN(4))
extern const struct nepa_define g_ni2c4_define;
extern struct nepa              g_ni2c4_epa;

#endif

#if (NPROFILE_EN_I2C & NPROFILE_EN(5))
extern const struct nepa_define g_ni2c5_define;
extern struct nepa              g_ni2c5_epa;

#endif

#if (NPROFILE_EN_I2C & NPROFILE_EN(6))
extern const struct nepa_define g_ni2c6_define;
extern struct nepa              g_ni2c6_epa;
#endif

#if (NPROFILE_EN_I2C & NPROFILE_EN(7))
extern const struct nepa_define g_ni2c7_define;
extern struct nepa              g_ni2c7_epa;
#endif

#if (NPROFILE_EN_I2C & NPROFILE_EN(8))
extern const struct nepa_define g_ni2c8_define;
extern struct nepa              g_ni2c8_epa;
#endif

#if (NPROFILE_EN_I2C & NPROFILE_EN(9))
extern const struct nepa_define g_ni2c9_define;
extern struct nepa              g_ni2c9_epa;
#endif

#if (NPROFILE_EN_I2C & NPROFILE_EN(10))
extern const struct nepa_define g_ni2c10_define;
extern struct nepa              g_ni2c10_epa;
#endif

#if (NPROFILE_EN_I2C & NPROFILE_EN(11))
extern const struct nepa_define g_ni2c11_define;
extern struct nepa              g_ni2c11_epa;
#endif

#if (NPROFILE_EN_I2C & NPROFILE_EN(12))
extern const struct nepa_define g_ni2c12_define;
extern struct nepa              g_ni2c12_epa;
#endif

#if (NPROFILE_EN_I2C & NPROFILE_EN(13))
extern const struct nepa_define g_ni2c13_define;
extern struct nepa              g_ni2c13_epa;
#endif

#if (NPROFILE_EN_I2C & NPROFILE_EN(14))
extern const struct nepa_define g_ni2c14_define;
extern struct nepa              g_ni2c14_epa;
#endif

#if (NPROFILE_EN_I2C & NPROFILE_EN(15))
extern const struct nepa_define g_ni2c15_define;
extern struct nepa              g_ni2c15_epa;
#endif

/*===================================================  FUNCTION PROTOTYPES  ==*/
/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of i2c.h
 ******************************************************************************/
#endif /* PLATFORM_NEON_DRIVERS_INCLUDE_MCU_I2C_H_ */
