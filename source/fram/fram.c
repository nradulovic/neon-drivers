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
 *********************************************************************//** @{ */

/*=========================================================  INCLUDE FILES  ==*/

#include "fram/fram.h"

/*=========================================================  LOCAL MACRO's  ==*/

#define A0_BIT_POSITION			1u

/*======================================================  LOCAL DATA TYPES  ==*/

enum address_space
{
	ADDRESS_SPACE_9_BIT,
	ADDRESS_SPACE_16_BIT,
	ADDRESS_SPACE_17_BIT
};



struct fram_workspace
{
	struct nepa	*				client;
	uint8_t						memory_address[2];
};

/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/

static size_t write_9bit_address(
	struct fram_driver * 		fram,
	uint32_t 					address,
	uint8_t * 					buff);



static size_t write_16bit_address(
		struct fram_driver * 		fram,
		uint32_t 					address,
		uint8_t * 					buff);



static size_t write_17bit_address(
		struct fram_driver * 		fram,
		uint32_t 					address,
		uint8_t * 					buff);



static naction state_active(
	struct nsm *,
	const struct nevent *);

/*=======================================================  LOCAL VARIABLES  ==*/

static const uint32_t g_address_spaces[] =
{
	ADDRESS_SPACE_9_BIT,
	ADDRESS_SPACE_16_BIT,
	ADDRESS_SPACE_16_BIT,
	ADDRESS_SPACE_16_BIT,
	ADDRESS_SPACE_16_BIT,
	ADDRESS_SPACE_16_BIT,
	ADDRESS_SPACE_17_BIT,
	ADDRESS_SPACE_16_BIT,
	ADDRESS_SPACE_9_BIT,
	ADDRESS_SPACE_16_BIT,
	ADDRESS_SPACE_16_BIT,
	ADDRESS_SPACE_16_BIT,
	ADDRESS_SPACE_16_BIT,
	ADDRESS_SPACE_16_BIT
};



static size_t (* g_write_memory_address[3])(struct fram_driver * fram, uint32_t address, uint8_t * buff) =
{
	write_9bit_address,
	write_16bit_address,
	write_17bit_address
};



static struct nevent * 		  	g_fram_queue_storage[CONFIG_FRAM_QUEUE_SIZE];
static struct fram_workspace  	g_fram_workspace;

/*======================================================  GLOBAL VARIABLES  ==*/

struct nepa    g_fram_epa;
const struct nepa_define g_fram_define =
{
    .sm.wspace                  = &g_fram_workspace,
    .sm.init_state              = &state_active,
    .sm.type                    = NSM_TYPE_FSM,
    .working_queue.storage      = g_fram_queue_storage,
    .working_queue.size         = sizeof(g_fram_queue_storage),
    .thread.priority            = 1,
    .thread.name                = "fram"
};

/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/

static inline uint32_t get_address_space(
	enum fram_devices device)
{
	return (g_address_spaces[device]);
}



static size_t write_9bit_address(
		struct fram_driver * 		fram,
		uint32_t 					address,
		uint8_t * 					buff)
{
	*buff = (uint8_t)address;
	if (address > UINT8_MAX) {
		fram->slave.address |= 1u << A0_BIT_POSITION;
	} else {
		fram->slave.address &= ~(1u << A0_BIT_POSITION);
	}

	return (1);
}



static size_t write_16bit_address(
		struct fram_driver * 		fram,
		uint32_t 					address,
		uint8_t * 					buff)
{
	uint16_t * add;

	(void *)fram;

	add = (uint16_t *)buff;
	*add = (uint16_t)address;

	return (2);
}



static size_t write_17bit_address(
		struct fram_driver * 		fram,
		uint32_t 					address,
		uint8_t * 					buff)
{
	uint16_t * add;

	add = (uint16_t *)buff;
	*add = (uint16_t)address;
	if (address > UINT16_MAX) {
		fram->slave.address |= 1u << A0_BIT_POSITION;
	} else {
		fram->slave.address &= ~(1u << A0_BIT_POSITION);
	}

	return (2);
}



static size_t write_memory_addres(
	struct fram_driver *		fram,
	uint32_t 					address,
	uint8_t	*					buff)
{
	return (g_write_memory_address[fram->address_space](fram, address, buff));
}



static naction state_active(
		struct nsm * sm,
		const struct nevent * event)
{
	struct fram_workspace * ws = sm->wspace;

	switch (event->id) {
		case EVT_FRAM_OPEN: {
			struct fram_open_event  * 	fram_open;
			struct ni2c_open_event	* 	i2c_open;
			struct fram_driver * 	  	fram;

			fram_open = (struct fram_open_event *)event;

			i2c_open = (struct ni2c_open_event *)nevent_create(
					sizeof(struct ni2c_open_event), EVT_NI2C_OPEN_SLAVE);

			fram = fram_open->fram;
			fram->device = fram_open->device;
			fram->device_id = fram_open->device_id;
			fram->ni2c_epa = fram_open->ni2c_epa;
			fram->address_space = get_address_space(fram->device);

			i2c_open->slave = &fram->slave;
			i2c_open->address = ((CONFIG_FRAM_DEVICE_ADDRES << 4) & 0xF0);
			i2c_open->address += (fram->device_id & 0x7);
			i2c_open->flags = 0u;
			ws->client = event->producer;

			nepa_send_event(fram_open->ni2c_epa, (nevent *)i2c_open);

			return (naction_handled());
		}
		case EVT_FRAM_WRITE: {
			struct ni2c_combined_transfer_event * 	combined;
			struct fram_transfer_event * 			transfer;
			struct fram_driver * 					fram;
			size_t 									size;

			transfer = (struct fram_transfer_event *)event;
			fram = transfer->fram;

			ws->client = event->producer;

			size = write_memory_addres(fram, transfer->address, ws->memory_address);

			combined = (struct ni2c_combined_transfer_event *)nevent_create(
					sizeof(struct ni2c_combined_transfer_event),
					EVT_NI2C_COMBINED_TRANSFER);

			combined->slave = &fram->slave;
			combined->type = NI2C_WRITE_THEN_WRITE;
			combined->first_data = ws->memory_address;
			combined->first_size = size;
			combined->second_data = transfer->data;
			combined->second_size = transfer->size;

			nepa_send_event(fram->ni2c_epa, (nevent *)combined);

			return (naction_handled());
		}
		case EVT_FRAM_READ: {
			struct ni2c_combined_transfer_event * 	combined;
			struct fram_transfer_event * 			transfer;
			struct fram_driver * 					fram;
			size_t 									size;

			transfer = (struct fram_transfer_event *)event;
			fram = transfer->fram;

			ws->client = event->producer;

			size = write_memory_addres(fram, transfer->address, ws->memory_address);

			combined = (struct ni2c_combined_transfer_event *)nevent_create(
					sizeof(struct ni2c_combined_transfer_event),
					EVT_NI2C_COMBINED_TRANSFER);

			combined->slave = &fram->slave;
			combined->type = NI2C_WRITE_THEN_READ;
			combined->first_data = ws->memory_address;
			combined->first_size = size;
			combined->second_data = transfer->data;
			combined->second_size = transfer->size;

			nepa_send_event(fram->ni2c_epa, (nevent *)combined);

			return (naction_handled());
		}
		case EVT_NI2C_TRANSFER_COMPLETED: {
			nepa_send_signal(
				ws->client,
				EVT_FRAM_COMPLETED);

			return (naction_handled());
		}
		case EVT_NI2C_ERROR: {
			nepa_send_signal(
				ws->client,
				EVT_FRAM_ERROR);

			return (naction_handled());
		}
		default: {

			return (naction_ignored());
		}
	}
}

/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/
/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of fram.c
 ******************************************************************************/

