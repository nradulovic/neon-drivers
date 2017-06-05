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
 * @brief       I2C operations
 * @defgroup    i2C_intf I2C operations
 * @brief       I2C operations
 *********************************************************************//** @{ */

#ifndef NI2C_BUS_H_
#define NI2C_BUS_H_

/*=========================================================  INCLUDE FILES  ==*/

#include <stdint.h>
#include <stddef.h>

#include "base/error.h"

#include "family/p_i2c_bus.h"
#include "mcu/peripheral.h"
#include "mcu/profile.h"

/*===============================================================  MACRO's  ==*/
/**@brief       Convenience wrapper macro around @ref NP_DEV_CREATE_ID().
 * @param       port
 *              Specify bus number in range 1 - 32, where NI2C1 is 0 etc.
 * @param       pin
 * @return      Returns valid I2C bus ID number which can be used with other
 *              functions and macros.
 */
#define NI2C_ID(bus)                NP_DEV_CREATE_ID(NPROFILE_CLASS_I2C, bus, 0)

/**@brief       Convenience wrapper macro around @ref NP_DEV_ID_TO_MAJOR().
 * @param       id
 *              Valid I2C bus ID
 * @return      Returns I2C bus number
 */
#define NI2C_ID_TO_BUS(id)          NP_DEV_ID_TO_MAJOR(id)

#define NI2C0                       0
#define NI2C1                       1
#define NI2C2                       2
#define NI2C3                       3
#define NI2C4                       4
#define NI2C5                       5
#define NI2C6                       6
#define NI2C7                       7
#define NI2C8                       8
#define NI2C9                       9
#define NI2C10                      10
#define NI2C11                      11
#define NI2C12                      12
#define NI2C13                      13
#define NI2C14                      14
#define NI2C15                      15

#define NI2C_BUS_MODE_MASTER        (0x0u << 0)
#define NI2C_BUS_MODE_SLAVE         (0x1u << 0)
#define NI2C_BUS_ADDRESS_7BIT       (0x0u << 1)
#define NI2C_BUS_ADDRESS_10BIT      (0x1u << 1)
#define NI2C_BUS_SPEED_100          (0x0u << 4)
#define NI2C_BUS_SPEED_400          (0x1u << 4)
#define NI2C_BUS_SPEED_1700         (0x2u << 4)
#define NI2C_BUS_SPEED_3400         (0x3u << 4)
#define NI2C_BUS_HANDLING_IT        (0x0u << 6)
#define NI2C_BUS_HANDLING_DMA       (0x1u << 6)

#define NI2C_REG_SIZE_0             (0u << 0)
#define NI2C_REG_SIZE_1             (1u << 0)
#define NI2C_REG_SIZE_2             (2u << 0)
#define NI2C_REG_SIZE_3             (3u << 0)
#define NI2C_REG_SIZE_4             (4u << 0)

#define NI2C_READ_WITH_RESTART      (0u << 4)
#define NI2C_READ_WITHOUT_RESTART   (1u << 4)


#define NI2C_BUS_ADDRESSING_MODE                                                \
    (NI2C_BUS_ADDRESS_7BIT | NI2C_BUS_ADDRESS_10BIT)

#define NI2C_BUS_SPEED                                                          \
    (NI2C_BUS_SPEED_100 | NI2C_BUS_SPEED_400 | NI2C_BUS_SPEED_1700 |            \
    NI2C_BUS_SPEED_3400)

#define NI2C_BUS_HANDLING                                                       \
    (NI2C_BUS_HANDLING_IT | NI2C_BUS_HANDLING_DMA)

#define NI2C_REG_SIZE                                                           \
    (NI2C_REG_SIZE_0 | NI2C_REG_SIZE_1 | NI2C_REG_SIZE_2                        \
    | NI2C_REG_SIZE_3 | NI2C_REG_SIZE_4)

#define NI2C_READ_MODE                                                          \
    (NI2C_READ_WITH_RESTART | NI2C_READ_WITHOUT_RESTART)

#define ni2c_get_reg_size(slave, value)                                         \
    ((slave)->flags & NI2C_REG_SIZE)

#define ni2c_set_reg_size(slave, value)                                         \
    (slave)->flags = (value & NI2C_REG_SIZE)

#define ni2c_get_slave_address(slave)                                           \
    (slave)->address

#define ni2c_set_slave_address(slave, slave_address)                            \
    (slave)->address = (slave_address)

/*-------------------------------------------------------  C++ extern base  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

enum ni2c_bus_error
{
    NI2C_BUS_COLISION_ERROR,
    NI2C_BUS_ACKNOWLEDGE_FAILURE,
    NI2C_BUS_ARBITRATION_LOST,
    NI2C_BUS_OVERFLOW
};



struct np_dev_i2c               ctx;



struct ni2c_bus_driver
{
    struct npdrv                pdrv;
    struct np_dev_i2c           ctx;
    struct ni2c_slave *         slave;
    uint32_t                    bus_handling;
    uint32_t                    reg;
    void *                      data;
    size_t                      size;
    uint32_t                    phase;
    uint32_t                    format;
    uint32_t                    type;
    uint32_t                    read_mode;
    uint32_t                    retry;
    enum ni2c_bus_error         error;
    struct ntimer               recovery_period;
};



struct ni2c_slave;



struct ni2c_slave
{
    uint32_t                    address;
    uint32_t                    flags;
    struct ni2c_bus_driver *    bus;
    void                        (* transfer)(struct ni2c_slave * slave);
    void                        (* error)(struct ni2c_slave * slave, enum ni2c_bus_error error);
};

/*======================================================  GLOBAL VARIABLES  ==*/

#if (NPROFILE_EN_I2C & NPROFILE_EN(1))
extern struct ni2c_bus_driver  g_i2c1_driver;
#endif

#if (NPROFILE_EN_I2C & NPROFILE_EN(2))
extern struct ni2c_bus_driver  g_i2c2_driver;
#endif

#if (NPROFILE_EN_I2C & NPROFILE_EN(3))
extern struct ni2c_bus_driver  g_i2c3_driver;
#endif

#if (NPROFILE_EN_I2C & NPROFILE_EN(4))
extern struct ni2c_bus_driver  g_i2c4_driver;
#endif

#if (NPROFILE_EN_I2C & NPROFILE_EN(5))
extern struct ni2c_bus_driver  g_i2c5_driver;
#endif

#if (NPROFILE_EN_I2C & NPROFILE_EN(6))
extern struct ni2c_bus_driver  g_i2c6_driver;
#endif

#if (NPROFILE_EN_I2C & NPROFILE_EN(7))
extern struct ni2c_bus_driver  g_i2c7_driver;
#endif

#if (NPROFILE_EN_I2C & NPROFILE_EN(8))
extern struct ni2c_bus_driver  g_i2c8_driver;
#endif

#if (NPROFILE_EN_I2C & NPROFILE_EN(9))
extern struct ni2c_bus_driver  g_i2c9_driver;
#endif

#if (NPROFILE_EN_I2C & NPROFILE_EN(10))
extern struct ni2c_bus_driver  g_i2c10_driver;
#endif

#if (NPROFILE_EN_I2C & NPROFILE_EN(11))
extern struct ni2c_bus_driver  g_i2c11_driver;
#endif

#if (NPROFILE_EN_I2C & NPROFILE_EN(12))
extern struct ni2c_bus_driver  g_i2c12_driver;
#endif

#if (NPROFILE_EN_I2C & NPROFILE_EN(13))
extern struct ni2c_bus_driver  g_i2c13_driver;
#endif

#if (NPROFILE_EN_I2C & NPROFILE_EN(14))
extern struct ni2c_bus_driver  g_i2c14_driver;
#endif

#if (NPROFILE_EN_I2C & NPROFILE_EN(15))
extern struct ni2c_bus_driver  g_i2c15_driver;
#endif

/*===================================================  FUNCTION PROTOTYPES  ==*/

void ni2c_bus_init(
    uint32_t                    bus_id,
    uint32_t                    config);



void ni2c_bus_term(
    uint32_t                    bus_id);



void ni2c_open_slave(
    struct ni2c_slave *         slave,
    struct ni2c_bus_driver *    bus,
    uint32_t                    flags,
    uint32_t                    address,
    void                        (* transfer)(struct ni2c_slave * slave),
    void                        (* error)(struct ni2c_slave * slave, enum ni2c_bus_error id));



void ni2c_write_slave(
    struct ni2c_slave *         slave,
    uint32_t                    reg,
    void *                      data,
    size_t                      size);



void ni2c_read_slave(
    struct ni2c_slave *         slave,
    uint32_t                    reg,
    void *                      data,
    size_t                      size);



void ni2c_event_isr(
    struct ni2c_bus_driver *    bus);



void ni2c_error_isr(
    struct ni2c_bus_driver *    bus);

/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of gpio.h
 ******************************************************************************/
#endif /* NI2C_H_ */
