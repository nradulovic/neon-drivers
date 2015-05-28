/*
 * This file is part of neon-drivers-example.
 *
 * Copyright (C) 2010 - 2015 nenad
 *
 * neon-drivers-example is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * neon-drivers-example is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with neon-drivers-example.  If not, see <http://www.gnu.org/licenses/>.
 *
 * web site:    
 * e-mail  :    
 *//***********************************************************************//**
 * @file
 * @author      nenad
 * @brief       Brief description
 * @defgroup    def_group Name
 * @brief       Brief description
 *********************************************************************//** @{ */

#ifndef NEON_DRIVERS_INCLUDE_MCU_CONFIG_H_
#define NEON_DRIVERS_INCLUDE_MCU_CONFIG_H_

/*=========================================================  INCLUDE FILES  ==*/

#include "neon_drivers_app_config.h"

/*===============================================================  MACRO's  ==*/

/*
 * NOTE: All peripheral is by default turned on.
 */

#if !defined(CONFIG_GPIO)
# define CONFIG_GPIO                    0xffffu
#endif

#if !defined(CONFIG_UART)
# define CONFIG_UART                    0xffffu
#endif

#if !defined(CONFIG_I2C)
# define CONFIG_I2C                     0xffffu
#endif

#if !defined(CONFIG_I2C_QUEUE_SIZE)
# define CONFIG_I2C_QUEUE_SIZE			10
#endif

#if !defined(CONFIG_SPI)
# define CONFIG_SPI                     0xffffu
#endif

#if !defined(CONFIG_DRIVERS_PM_ENABLE)
# define CONFIG_DRIVERS_PM_ENABLE		0
#endif

#if !defined(CONFIG_I2C_EVENT_BASE_ID)
# define CONFIG_I2C_EVENT_BASE_ID		1000	/* todo naci mesto gde treba da stoji vrednost id za sistem i drajvere */
#endif

#if !defined(CONFIG_I2C_EPA_PRIORITY)
#define CONFIG_I2C_EPA_PRIORITY			NTHREAD_PRIORITY_MAX
#endif

#if !defined(CONFIG_FRAM_EVENT_BASE_ID)
# define CONFIG_FRAM_EVENT_BASE_ID		2000	/* todo naci mesto gde treba da stoji vrednost id za sistem i drajvere */
#endif

#if !defined(CONFIG_FRAM_QUEUE_SIZE)
# define CONFIG_FRAM_QUEUE_SIZE			10
#endif

#if !defined(CONFIG_RTC_EVENT_BASE_ID)
# define CONFIG_RTC_EVENT_BASE_ID		3000	/* todo naci mesto gde treba da stoji vrednost id za sistem i drajvere */
#endif

#if !defined(CONFIG_RTC_QUEUE_SIZE)
# define CONFIG_RTC_QUEUE_SIZE			10
#endif


/*-------------------------------------------------------  C++ extern base  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/
/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/
/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of config.h
 ******************************************************************************/
#endif /* NEON_DRIVERS_INCLUDE_MCU_CONFIG_H_ */
