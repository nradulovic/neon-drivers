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

#ifndef PLATFORM_NEON_DRIVERS_INCLUDE_XIO_XIO_CLASS_H_
#define PLATFORM_NEON_DRIVERS_INCLUDE_XIO_XIO_CLASS_H_

/*=========================================================  INCLUDE FILES  ==*/

#include <stdint.h>

#include "ep/event.h"
#include "ep/epa.h"
#include "mcu/config.h"
#include "mcu/i2c.h"

/*===============================================================  MACRO's  ==*/
/*------------------------------------------------------  C++ extern begin  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

enum nxio_pin_state
{
	NXIO_PIN_STATE_LOW			= 0u,
	NXIO_PIN_STATE_HIGH
};



enum nxio_pin_mode
{
	NXIO_MODE_INPUT_FLOAT	 	= 0u,
	NXIO_MODE_INPUT_PULL_UP,
	NXIO_MODE_INPUT_PULL_DOWN,
	NXIO_MODE_OUTPUT_PUSH_PULL,
	NXIO_MODE_OUTPUT_OPEN_DRAIN
};



enum nxio_int_pin_mode
{
	NXIO_INT_MODE_NONE			= 0u,
	NXIO_INT_MODE_RISING_EDGE,
	NXIO_INT_MODE_FALLING_EDGE,
	NXIO_INT_MODE_LOW_LEVEL,
	NXIO_INT_MOODE_HIGH_HEVEL,
	NXIO_INT_MODE_PIN_CHANGE,
};



enum nxio_event_id
{
	EVT_NXIO_OPEN				= CONFIG_NXIO_EVENT_BASE_ID,
	EVT_NXIO_CONFIG,
	EVT_NXIO_WRITE_PIN,
	EVT_NXIO_READ_PIN,
	EVT_NXIO_WRITE,
	EVT_NXIO_READ,
	EVT_NXIO_COMPLETED,
	EVT_NXIO_FAILED
};



struct nxio_driver
{
	struct nepa	*				ni2c_epa;
	struct ni2c_slave 			slave;
	uint32_t					address;
	uint32_t					device_id;
};



struct nxio_event_open
{
	nevent   					event;
	struct nxio_driver *		nxio;
	struct nepa	*				ni2c_epa;
	uint32_t					address;
	uint32_t					device_id;
};



struct nxio_event_config
{
	nevent   					event;
	struct nxio_driver *		nxio;
	uint8_t	*					modes;
	uint8_t	*					interrupts;
};



struct nxio_event_write_pin
{
	nevent   					event;
	struct nxio_driver *		nxio;
	uint32_t					pin;
	enum nxio_pin_state			state;
};



struct nxio_event_read_pin
{
	nevent   					event;
	struct nxio_driver *		nxio;
	uint32_t					pin;
	enum nxio_pin_state *		state;
};



struct nxio_event_write
{
	nevent   					event;
	struct nxio_driver *		nxio;
	uint32_t					value;
};



struct nxio_event_read
{
	nevent   					event;
	struct nxio_driver *		nxio;
	uint32_t *					value;
};

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/
/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of xio_class.h
 ******************************************************************************/
#endif /* PLATFORM_NEON_DRIVERS_INCLUDE_XIO_XIO_CLASS_H_ */
