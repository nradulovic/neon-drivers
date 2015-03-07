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

#include <stddef.h>

#include "shared/debug.h"
#include "shared/component.h"
#include "mcu/peripheral.h"

/*=========================================================  LOCAL MACRO's  ==*/

#define IS_DEVICE(device)               ((device)->recognition.magic == NP_DEV_MAGIC)

/*======================================================  LOCAL DATA TYPES  ==*/

static const NCOMPONENT_DEFINE("Peripheral device driver", "Nenad Radulovic");

/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/
/*=======================================================  LOCAL VARIABLES  ==*/
/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/
/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/


struct np_drv * np_drv_get(const struct np_dev * device)
{
    struct np_drv *            driver;

    NASSERT(NAPI_POINTER, device != NULL);
    NASSERT(NAPI_OBJECT,  IS_DEVICE(device));
    driver = device->p_drv;

    NASSERT(NAPI_USAGE, driver->ref == 0u);
    driver->p_dev  = device;
    driver->data = NULL;

    return (driver);
}

void np_drv_put(struct np_drv * driver)
{
    NASSERT(NAPI_POINTER, driver != NULL);
    NASSERT(NAPI_OBJECT,  IS_DEVICE(driver->p_dev));
    NASSERT(NAPI_USAGE,   driver->ref == 0u);
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of peripheral.c
 ******************************************************************************/
