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

#include "mcu/i2c.h"

/*=========================================================  LOCAL MACRO's  ==*/
/*======================================================  LOCAL DATA TYPES  ==*/
/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/
/*=======================================================  LOCAL VARIABLES  ==*/
/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/
/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/

struct i2c_bus * i2c_bus_from_id(
    uint8_t                     bus_id)
{
    return (NULL);
}



void i2c_slave_open(
    struct i2c_slave *          slave,
    const struct i2c_slave_config * config,
    struct i2c_bus *            bus,
    uint8_t                     id)
{

}



void i2c_slave_close(
    struct i2c_slave *          slave)
{

}



nerror i2c_slave_read(
    struct i2c_slave *          slave,
    uint8_t                     address,
    void *                      data,
    size_t                      size)
{
    return (NERROR_NOT_IMPLEMENTED);
}



nerror i2c_slave_write(
    struct i2c_slave *          slave,
    uint8_t                     address,
    const void *                data,
    size_t                      size)
{
    return (NERROR_NOT_IMPLEMENTED);
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of i2c.c
 ******************************************************************************/
