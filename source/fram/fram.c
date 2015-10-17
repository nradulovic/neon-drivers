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
#include "neon_drivers_app_config.h"

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
};

/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/

static uint32_t write_9bit_address(
	struct fram_driver * 		fram,
	uint32_t 					address);



static uint32_t write_16bit_address(
		struct fram_driver * 		fram,
		uint32_t 					address);



static uint32_t write_17bit_address(
		struct fram_driver * 		fram,
		uint32_t 					address);



static uint32_t write_memory_addres(
	struct fram_driver *		fram,
	uint32_t 					address);



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



static uint32_t (* g_write_memory_address[3])(struct fram_driver * fram, uint32_t address) =
{
	write_9bit_address,
	write_16bit_address,
	write_17bit_address
};



static struct nevent * 		  	g_fram_queue_storage[CONFIG_FRAM_QUEUE_SIZE];
static struct fram_workspace  	g_fram_workspace;

/*======================================================  GLOBAL VARIABLES  ==*/

struct nepa    					g_fram_epa;
const struct nepa_define		g_fram_define =
{
    NSM_DEF_INIT(&g_fram_workspace, state_active, NSM_TYPE_FSM),
    NEQUEUE_DEF_INIT(g_fram_queue_storage,  sizeof(g_fram_queue_storage)),
    NTHREAD_DEF_INIT("fram", CONFIG_FRAM_PRIORITY),
};

/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/

static inline uint32_t get_address_space(
	enum fram_devices device)
{
	return (g_address_spaces[device]);
}



static uint32_t write_9bit_address(
		struct fram_driver * 		fram,
		uint32_t 					address)
{
	uint32_t slave_address;

	slave_address = ni2c_get_slave_address(&fram->slave);

	if (address > UINT8_MAX) {
		slave_address |= 1u << A0_BIT_POSITION;
	} else {
		slave_address &= ~(1u << A0_BIT_POSITION);
	}

	ni2c_set_slave_address(&fram->slave, slave_address);
	ni2c_set_reg_size(&fram->slave, NI2C_CONFIG_REG_SIZE_1);

	return (address & 0x1FF);
}



static uint32_t write_16bit_address(
		struct fram_driver * 		fram,
		uint32_t 					address)
{
	ni2c_set_reg_size(&fram->slave, NI2C_CONFIG_REG_SIZE_2);

	return (address & 0xFFFF);
}



static uint32_t write_17bit_address(
		struct fram_driver * 		fram,
		uint32_t 					address)
{
	uint32_t slave_address;

	slave_address = ni2c_get_slave_address(&fram->slave);

	if (address > UINT16_MAX) {
		slave_address |= 1u << A0_BIT_POSITION;
	} else {
		slave_address &= ~(1u << A0_BIT_POSITION);
	}

	ni2c_set_slave_address(&fram->slave, slave_address);
	ni2c_set_reg_size(&fram->slave, NI2C_CONFIG_REG_SIZE_2);

	return (address & 0x1FFFF);
}



static uint32_t write_memory_addres(
	struct fram_driver *		fram,
	uint32_t 					address)
{
	return (g_write_memory_address[fram->address_space](fram, address));
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
			struct fram_transfer_event * 			fram_transfer;
			struct fram_driver * 					fram;
			struct ni2c_transfer_event	*			i2c_transfer;

			fram_transfer = (struct fram_transfer_event *)event;
			fram = fram_transfer->fram;
			ws->client = event->producer;

			i2c_transfer = (struct ni2c_transfer_event *)nevent_create(
					sizeof(struct ni2c_transfer_event),
					EVT_NI2C_WRITE_SLAVE);

			i2c_transfer->slave = &fram->slave;
			i2c_transfer->reg = write_memory_addres(fram, fram_transfer->address);
			i2c_transfer->data = fram_transfer->data;
			i2c_transfer->size = fram_transfer->size;

			nepa_send_event(fram->ni2c_epa, (nevent *)i2c_transfer);

			return (naction_handled());
		}
		case EVT_FRAM_READ: {
			struct fram_transfer_event * 			fram_transfer;
			struct fram_driver * 					fram;
			struct ni2c_transfer_event	*			i2c_transfer;

			fram_transfer = (struct fram_transfer_event *)event;
			fram = fram_transfer->fram;
			ws->client = event->producer;

			i2c_transfer = (struct ni2c_transfer_event *)nevent_create(
					sizeof(struct ni2c_transfer_event),
					EVT_NI2C_READ_SLAVE);

			i2c_transfer->slave = &fram->slave;
			i2c_transfer->reg = write_memory_addres(fram, fram_transfer->address);
			i2c_transfer->data = fram_transfer->data;
			i2c_transfer->size = fram_transfer->size;

			nepa_send_event(fram->ni2c_epa, (nevent *)i2c_transfer);

			return (naction_handled());
		}
		case EVT_NI2C_TRANSFER_COMPLETED: {
			nepa_send_signal(ws->client, EVT_FRAM_COMPLETED);

			return (naction_handled());
		}
		case EVT_NI2C_ERROR: {
			nepa_send_signal(ws->client, EVT_FRAM_ERROR);

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


