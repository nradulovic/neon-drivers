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

#ifndef PLATFORM_NEON_DRIVERS_INCLUDE_FRAM_FRAM_H_
#define PLATFORM_NEON_DRIVERS_INCLUDE_FRAM_FRAM_H_

/*=========================================================  INCLUDE FILES  ==*/

#include "mcu/i2c.h"
#include "mcu/config.h"
#include "neon_eds.h"

/*===============================================================  MACRO's  ==*/

#if !defined(CONFIG_FRAM_DEVICE_ADDRES)
# define CONFIG_FRAM_DEVICE_ADDRES	10
#endif

/*------------------------------------------------------  C++ extern begin  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

enum fram_devices
{
	FM24CL04B,
	FM24CL16B,
	FM24CL64B,
	FM24V01,
	FM24V02,
	FM24V05,
	FM24V10,
	FM24W256,
	MB85RC04,
	MB85RC16,
	MB85RC64,
	MB85RC128,
	MB85RC256,
	MR44V064A,
};



enum fram_event_id
{
	EVT_FRAM_OPEN = CONFIG_FRAM_EVENT_BASE_ID,
	EVT_FRAM_WRITE,
	EVT_FRAM_READ,
	EVT_FRAM_COMPLETED,
	EVT_FRAM_ERROR
};



struct fram_driver
{
	struct nepa	*				ni2c_epa;
	struct ni2c_slave 			slave;
	enum fram_devices			device;
	uint32_t					device_id;
	uint32_t					address_space;
};



struct fram_open_event
{
	nevent   					event;
	struct fram_driver *		fram;
	struct nepa	*				ni2c_epa;
	enum fram_devices			device;
	uint32_t					device_id;
};



struct fram_transfer_event
{
	nevent   					event;
	struct fram_driver *		fram;
	uint32_t					address;
	void *						data;
	size_t						size;
};

/*======================================================  GLOBAL VARIABLES  ==*/

extern struct nepa 				g_fram_epa;
extern const struct nepa_define g_fram_define;

/*===================================================  FUNCTION PROTOTYPES  ==*/
/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of fram.h
 ******************************************************************************/
#endif /* PLATFORM_NEON_DRIVERS_INCLUDE_FRAM_FRAM_H_ */
