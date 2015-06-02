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

#include "mcu/i2c.h"
#include "mcu/i2c_bus.h"
#include "mcu/peripheral.h"
#include "mcu/profile.h"
#include "neon_eds.h"

/*=========================================================  LOCAL MACRO's  ==*/

#define I2C_MAX_RETRIES			3u

/*======================================================  LOCAL DATA TYPES  ==*/

enum i2c_local_events
{
	EVT_TRANSFER_COMPLETE = NEVENT_LOCAL_ID,
	EVT_TIMEOUT,
	EVT_ERROR
};



enum i2c_action
{
	I2C_WRITE,
	I2C_READ,
};



struct i2c_workspace
{
	const struct npdev *    	dev;
	struct netimer			 	timeout;
	struct ni2c_slave *			slave;
	struct nepa	*				client;
	enum i2c_action				action;
	void *						data;
	size_t						size;
	uint32_t					speed;
	uint32_t					retries;
	uint32_t					reg;
};

/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/

static void transfer(
	struct ni2c_slave * slave);



static void error(
	struct ni2c_slave * slave,
	enum ni2c_bus_error id);



static naction state_init(
	struct nsm *,
	const struct nevent *);



static naction state_idle(
	struct nsm *,
	const struct nevent *);



static naction state_transfer(
	struct nsm *,
	const struct nevent *);

/*=======================================================  LOCAL VARIABLES  ==*/

static const NCOMPONENT_DEFINE("Generic I2C driver");

#if (NPROFILE_EN_I2C & NPROFILE_EN(1))
static struct nevent * 		  	g_ni2c1_queue_storage[CONFIG_I2C_QUEUE_SIZE];
static struct i2c_workspace  	g_ni2c1_workspace =
{
	.dev = &g_i2c1
};
#endif

#if (NPROFILE_EN_I2C & NPROFILE_EN(2))
static struct nevent * 		  	g_ni2c2_queue_storage[CONFIG_I2C_QUEUE_SIZE];
static struct i2c_workspace  	g_ni2c2_workspace =
{
		.dev = &g_i2c2
};
#endif

#if (NPROFILE_EN_I2C & NPROFILE_EN(3))
static struct nevent * 		  	g_ni2c3_queue_storage[CONFIG_I2C_QUEUE_SIZE];
static struct i2c_workspace  	g_ni2c3_workspace =
{
	.dev = &g_i2c3
};
#endif

#if (NPROFILE_EN_I2C & NPROFILE_EN(4))
static struct nevent  		  	g_ni2c4_queue_storage[CONFIG_I2C_QUEUE_SIZE];
static struct i2c_workspace  	g_ni2c4_workspace =
{
	.dev = &g_i2c4
};
#endif

#if (NPROFILE_EN_I2C & NPROFILE_EN(5))
static struct nevent  		  	g_ni2c5_queue_storage[CONFIG_I2C_QUEUE_SIZE];
static struct i2c_workspace  	g_ni2c5_workspace =
{
	.dev = &g_i2c4
};
#endif

#if (NPROFILE_EN_I2C & NPROFILE_EN(6))
static struct nevent  		  	g_ni2c6_queue_storage[CONFIG_I2C_QUEUE_SIZE];
static struct i2c_workspace  	g_ni2c6_workspace =
{
	.dev = &g_i2c4
};
#endif

#if (NPROFILE_EN_I2C & NPROFILE_EN(7))
static struct nevent  		  	g_ni2c7_queue_storage[CONFIG_I2C_QUEUE_SIZE];
static struct i2c_workspace  	g_ni2c7_workspace =
{
	.dev = &g_i2c4
};
#endif

#if (NPROFILE_EN_I2C & NPROFILE_EN(8))
static struct nevent  		  	g_ni2c8_queue_storage[CONFIG_I2C_QUEUE_SIZE];
static struct i2c_workspace  	g_ni2c8_workspace =
{
	.dev = &g_i2c4
};
#endif

#if (NPROFILE_EN_I2C & NPROFILE_EN(9))
static struct nevent  		  	g_ni2c9_queue_storage[CONFIG_I2C_QUEUE_SIZE];
static struct i2c_workspace  	g_ni2c9_workspace =
{
	.dev = &g_i2c4
};
#endif

#if (NPROFILE_EN_I2C & NPROFILE_EN(10))
static struct nevent  		  	g_ni2c10_queue_storage[CONFIG_I2C_QUEUE_SIZE];
static struct i2c_workspace  	g_ni2c10_workspace =
{
	.dev = &g_i2c4
};
#endif

#if (NPROFILE_EN_I2C & NPROFILE_EN(11))
static struct nevent  		  	g_ni2c11_queue_storage[CONFIG_I2C_QUEUE_SIZE];
static struct i2c_workspace  	g_ni2c11_workspace =
{
	.dev = &g_i2c4
};
#endif

#if (NPROFILE_EN_I2C & NPROFILE_EN(12))
static struct nevent  		  	g_ni2c12_queue_storage[CONFIG_I2C_QUEUE_SIZE];
static struct i2c_workspace  	g_ni2c12_workspace =
{
	.dev = &g_i2c4
};
#endif

#if (NPROFILE_EN_I2C & NPROFILE_EN(13))
static struct nevent  		  	g_ni2c13_queue_storage[CONFIG_I2C_QUEUE_SIZE];
static struct i2c_workspace  	g_ni2c13_workspace =
{
	.dev = &g_i2c4
};
#endif

#if (NPROFILE_EN_I2C & NPROFILE_EN(14))
static struct nevent  		  	g_ni2c14_queue_storage[CONFIG_I2C_QUEUE_SIZE];
static struct i2c_workspace  	g_ni2c14_workspace =
{
	.dev = &g_i2c4
};
#endif

#if (NPROFILE_EN_I2C & NPROFILE_EN(15))
static struct nevent  		  	g_ni2c15_queue_storage[CONFIG_I2C_QUEUE_SIZE];
static struct i2c_workspace  	g_ni2c15_workspace =
{
	.dev = &g_i2c4
};
#endif

static struct ni2c_slave g_general_call_slave;

/*======================================================  GLOBAL VARIABLES  ==*/

struct ni2c_bus_driver * g_i2c1_bus;

#if (NPROFILE_EN_I2C & NPROFILE_EN(1))
struct nepa    g_ni2c1_epa;
const struct nepa_define g_ni2c1_define =
{
    .sm.wspace                  = &g_ni2c1_workspace,
    .sm.init_state              = &state_init,
    .sm.type                    = NSM_TYPE_FSM,
    .working_queue.storage      = g_ni2c1_queue_storage,
    .working_queue.size         = sizeof(g_ni2c1_queue_storage),
    .thread.priority            = CONFIG_I2C_EPA_PRIORITY,
    .thread.name                = "ni2c1"
};
#endif

#if (NPROFILE_EN_I2C & NPROFILE_EN(2))
struct nepa    g_ni2c2_epa;
const struct nepa_define g_ni2c2_define =
{
    .sm.wspace                  = &g_ni2c2_workspace,
    .sm.init_state              = &state_init,
    .sm.type                    = NSM_TYPE_FSM,
    .working_queue.storage      = g_ni2c2_queue_storage,
    .working_queue.size         = sizeof(g_ni2c2_queue_storage),
    .thread.priority            = CONFIG_I2C_EPA_PRIORITY,
    .thread.name                = "ni2c2"
};
#endif

#if (NPROFILE_EN_I2C & NPROFILE_EN(3))
struct nepa    g_ni2c3_epa;
const struct nepa_define g_ni2c3_define =
{
    .sm.wspace                  = &g_ni2c3_workspace,
    .sm.init_state              = &state_init,
    .sm.type                    = NSM_TYPE_FSM,
    .working_queue.storage      = g_ni2c3_queue_storage,
    .working_queue.size         = sizeof(g_ni2c3_queue_storage),
    .thread.priority            = CONFIG_I2C_EPA_PRIORITY,
    .thread.name                = "ni2c3"
};
#endif

#if (NPROFILE_EN_I2C & NPROFILE_EN(4))
struct nepa    g_ni2c4_epa;
const struct nepa_define g_ni2c4_define =
{
    .sm.wspace                  = &g_ni2c4_workspace,
    .sm.init_state              = &state_init,
    .sm.type                    = NSM_TYPE_FSM,
    .working_queue.storage      = g_ni2c4_queue_storage,
    .working_queue.size         = sizeof(g_ni2c4_queue_storage),
    .thread.priority            = CONFIG_I2C_EPA_PRIORITY,
    .thread.name                = "ni2c4"
};
#endif

#if (NPROFILE_EN_I2C & NPROFILE_EN(5))
struct nepa    g_ni2c5_epa;
const struct nepa_define g_ni2c5_define =
{
    .sm.wspace                  = &g_ni2c5_workspace,
    .sm.init_state              = &state_init,
    .sm.type                    = NSM_TYPE_FSM,
    .working_queue.storage      = g_ni2c5_queue_storage,
    .working_queue.size         = sizeof(g_ni2c5_queue_storage),
    .thread.priority            = CONFIG_I2C_EPA_PRIORITY,
    .thread.name                = "ni2c5"
};
#endif

#if (NPROFILE_EN_I2C & NPROFILE_EN(6))
struct nepa    g_ni2c6_epa;
const struct nepa_define g_ni2c6_define =
{
    .sm.wspace                  = &g_ni2c6_workspace,
    .sm.init_state              = &state_init,
    .sm.type                    = NSM_TYPE_FSM,
    .working_queue.storage      = g_ni2c6_queue_storage,
    .working_queue.size         = sizeof(g_ni2c6_queue_storage),
    .thread.priority            = CONFIG_I2C_EPA_PRIORITY,
    .thread.name                = "ni2c6"
};
#endif

#if (NPROFILE_EN_I2C & NPROFILE_EN(7))
struct nepa    g_ni2c7_epa;
const struct nepa_define g_ni2c7_define =
{
    .sm.wspace                  = &g_ni2c7_workspace,
    .sm.init_state              = &state_init,
    .sm.type                    = NSM_TYPE_FSM,
    .working_queue.storage      = g_ni2c7_queue_storage,
    .working_queue.size         = sizeof(g_ni2c7_queue_storage),
    .thread.priority            = CONFIG_I2C_EPA_PRIORITY,
    .thread.name                = "ni2c7"
};
#endif

#if (NPROFILE_EN_I2C & NPROFILE_EN(8))
struct nepa    g_ni2c8_epa;
const struct nepa_define g_ni2c8_define =
{
    .sm.wspace                  = &g_ni2c8_workspace,
    .sm.init_state              = &state_init,
    .sm.type                    = NSM_TYPE_FSM,
    .working_queue.storage      = g_ni2c8_queue_storage,
    .working_queue.size         = sizeof(g_ni2c8_queue_storage),
    .thread.priority            = CONFIG_I2C_EPA_PRIORITY,
    .thread.name                = "ni2c8"
};
#endif

#if (NPROFILE_EN_I2C & NPROFILE_EN(9))
struct nepa    g_ni2c9_epa;
const struct nepa_define g_ni2c9_define =
{
    .sm.wspace                  = &g_ni2c9_workspace,
    .sm.init_state              = &state_init,
    .sm.type                    = NSM_TYPE_FSM,
    .working_queue.storage      = g_ni2c9_queue_storage,
    .working_queue.size         = sizeof(g_ni2c9_queue_storage),
    .thread.priority            = CONFIG_I2C_EPA_PRIORITY,
    .thread.name                = "ni2c9"
};
#endif

#if (NPROFILE_EN_I2C & NPROFILE_EN(10))
struct nepa    g_ni2c10_epa;
const struct nepa_define g_ni2c10_define =
{
    .sm.wspace                  = &g_ni2c10_workspace,
    .sm.init_state              = &state_init,
    .sm.type                    = NSM_TYPE_FSM,
    .working_queue.storage      = g_ni2c10_queue_storage,
    .working_queue.size         = sizeof(g_ni2c10_queue_storage),
    .thread.priority            = CONFIG_I2C_EPA_PRIORITY,
    .thread.name                = "ni2c10"
};
#endif

#if (NPROFILE_EN_I2C & NPROFILE_EN(11))
struct nepa    g_ni2c11_epa;
const struct nepa_define g_ni2c11_define =
{
    .sm.wspace                  = &g_ni2c11_workspace,
    .sm.init_state              = &state_init,
    .sm.type                    = NSM_TYPE_FSM,
    .working_queue.storage      = g_ni2c11_queue_storage,
    .working_queue.size         = sizeof(g_ni2c11_queue_storage),
    .thread.priority            = CONFIG_I2C_EPA_PRIORITY,
    .thread.name                = "ni2c11"
};
#endif

#if (NPROFILE_EN_I2C & NPROFILE_EN(12))
struct nepa    g_ni2c12_epa;
const struct nepa_define g_ni2c12_define =
{
    .sm.wspace                  = &g_ni2c12_workspace,
    .sm.init_state              = &state_init,
    .sm.type                    = NSM_TYPE_FSM,
    .working_queue.storage      = g_ni2c12_queue_storage,
    .working_queue.size         = sizeof(g_ni2c12_queue_storage),
    .thread.priority            = CONFIG_I2C_EPA_PRIORITY,
    .thread.name                = "ni2c12"
};
#endif

#if (NPROFILE_EN_I2C & NPROFILE_EN(13))
struct nepa    g_ni2c13_epa;
const struct nepa_define g_ni2c13_define =
{
    .sm.wspace                  = &g_ni2c13_workspace,
    .sm.init_state              = &state_init,
    .sm.type                    = NSM_TYPE_FSM,
    .working_queue.storage      = g_ni2c13_queue_storage,
    .working_queue.size         = sizeof(g_ni2c13_queue_storage),
    .thread.priority            = CONFIG_I2C_EPA_PRIORITY,
    .thread.name                = "ni2c13"
};
#endif

#if (NPROFILE_EN_I2C & NPROFILE_EN(14))
struct nepa    g_ni2c14_epa;
const struct nepa_define g_ni2c14_define =
{
    .sm.wspace                  = &g_ni2c14_workspace,
    .sm.init_state              = &state_init,
    .sm.type                    = NSM_TYPE_FSM,
    .working_queue.storage      = g_ni2c14_queue_storage,
    .working_queue.size         = sizeof(g_ni2c14_queue_storage),
    .thread.priority            = CONFIG_I2C_EPA_PRIORITY,
    .thread.name                = "ni2c14"
};
#endif

#if (NPROFILE_EN_I2C & NPROFILE_EN(15))
struct nepa    g_ni2c15_epa;
const struct nepa_define g_ni2c15_define =
{
    .sm.wspace                  = &g_ni2c15_workspace,
    .sm.init_state              = &state_init,
    .sm.type                    = NSM_TYPE_FSM,
    .working_queue.storage      = g_ni2c15_queue_storage,
    .working_queue.size         = sizeof(g_ni2c15_queue_storage),
    .thread.priority            = CONFIG_I2C_EPA_PRIORITY,
    .thread.name                = "ni2c15"
};
#endif

/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/

static inline uint32_t compute_timeout(
		uint32_t size,
		uint32_t speed)
{
	uint32_t timeout;

	switch (speed) {
		case NI2C_BUS_SPEED_100: {

			return (timeout = size / 10);
		}
		case NI2C_BUS_SPEED_400: {

			return (timeout = size / 40);
		}
		case NI2C_BUS_SPEED_1700: {

			return (timeout = size / 160);
		}
		case NI2C_BUS_SPEED_3400: {

			return (timeout = size / 340);
		}
		default: {

			return size;
		}
	}
}



static inline struct ni2c_bus_driver * pdrv_to_ni2c_driver(
 	struct npdrv * 				pdrv)
{
 	return (PORT_C_CONTAINER_OF(pdrv, struct ni2c_bus_driver, pdrv));
}



static void transfer(
	struct ni2c_slave * slave)
{
	struct nepa	* epa;
	struct ni2c_bus_driver * bus;
	static const struct nevent transfer =
	{
		.id     = EVT_TRANSFER_COMPLETE,
#if (CONFIG_API_VALIDATION == 1)
		.signature = NSIGNATURE_EVENT,
#endif
	};

	bus = slave->bus;
	epa = npdrv_get_data(&bus->pdrv);
	nepa_send_event_i(
		epa,
		(nevent *)&transfer);
}



static void error(
	struct ni2c_slave * slave,
	enum ni2c_bus_error id)
{
	struct nepa	* epa;
	struct ni2c_bus_driver * bus;
	static struct ni2c_error_event error =
	{
		{
			.attrib = NEVENT_ATTR_RESERVED,
			.id     = EVT_ERROR,
#if (CONFIG_API_VALIDATION == 1)
			.signature = NSIGNATURE_EVENT,
#endif
		}
	};

	bus = slave->bus;
	epa = npdrv_get_data(&bus->pdrv);
	error.error_id = id;
	nepa_send_event_i(
		epa,
		(nevent *)&error);
}



static naction state_init(
		struct nsm * sm,
		const struct nevent * event)
{
	struct i2c_workspace * ws = sm->wspace;

	switch (event->id) {
		case NSM_INIT: {
			netimer_init(&ws->timeout);

			return (naction_handled());
		}
		case EVT_NI2C_CONFIG: {
			struct ni2c_config_event * config;

			config = (struct ni2c_config_event *)event;

			ni2c_bus_init(
				NP_DEV_CREATE_ID(NPROFILE_CLASS_I2C, npdev_instance(ws->dev), 0),
				config->config);

			ws->speed = (config->config & NI2C_BUS_SPEED);
			npdrv_set_data(
				ws->dev->pdrv,
				nepa_get_current());

			return (naction_transit_to(sm, state_idle));
		}
		default: {

			return (naction_ignored());
		}
	}
}



static naction state_idle(
	struct nsm * sm,
	const struct nevent * event)
{
	struct i2c_workspace * ws = sm->wspace;
	struct ni2c_bus_driver * bus;

	bus = pdrv_to_ni2c_driver(ws->dev->pdrv);

	switch (event->id) {
		case NSM_ENTRY: {

			nepa_fetch_one_deferred();

			return (naction_handled());
		}
		case EVT_NI2C_OPEN_SLAVE: {
			struct ni2c_open_event * open;

			open = (struct ni2c_open_event *)event;

			ni2c_open_slave(open->slave, bus, open->flags, open->address, transfer, error);

			return (naction_handled());
		}
		case EVT_NI2C_WRITE_SLAVE: {
			struct ni2c_transfer_event * transfer;

			transfer = (struct ni2c_transfer_event *)event;

			NREQUIRE(NAPI_USAGE "Invalid slave.", transfer->slave != NULL);
			NREQUIRE(NAPI_USAGE "Invalid data.", transfer->data != NULL);
			NREQUIRE(NAPI_USAGE "Invalid size.", transfer->size != 0);

			ws->slave = transfer->slave;
			ws->data = transfer->data;
			ws->size = transfer->size;
			ws->reg  = transfer->reg;
			ws->action = I2C_WRITE;
			ws->client = event->producer;
			ws->retries = 0;

			return (naction_transit_to(sm, state_transfer));
		}
		case EVT_NI2C_READ_SLAVE: {
			struct ni2c_transfer_event * transfer;

			transfer = (struct ni2c_transfer_event *)event;

			NREQUIRE(NAPI_USAGE "Invalid slave.", transfer->slave != NULL);
			NREQUIRE(NAPI_USAGE "Invalid data.", transfer->data != NULL);
			NREQUIRE(NAPI_USAGE "Invalid size.", transfer->size != 0);

			ws->slave = transfer->slave;
			ws->data = transfer->data;
			ws->size = transfer->size;
			ws->reg  = transfer->reg;
			ws->action = I2C_READ;
			ws->client = event->producer;
			ws->retries = 0;

			return (naction_transit_to(sm, state_transfer));
		}
		case EVT_NI2C_GENERAL_CALL: {
			struct ni2c_general_call_event * general;

			general = (struct ni2c_general_call_event *)event;

			NREQUIRE(NAPI_USAGE "Invalid command.", general->command != 0);

			ws->slave = &g_general_call_slave;
			ws->data = &general->command;
			ws->size = 1u;
			ws->action = I2C_WRITE;
			ws->client = event->producer;
			ws->retries = 0;

			ni2c_open_slave(ws->slave, bus, 0, 0, transfer, error);

			return (naction_transit_to(sm, state_transfer));
		}
		default: {

			return (naction_ignored());
		}
	}
}



static naction state_transfer(
		struct nsm * sm,
		const struct nevent * event)
{
	struct i2c_workspace * ws = sm->wspace;

	switch (event->id) {
		case NSM_ENTRY: {
			uint32_t	timeout;

			switch (ws->action) {
				case I2C_WRITE: {
					ni2c_write_slave(ws->slave, ws->reg, ws->data, ws->size);
					break;
				}
				case I2C_READ: {
					ni2c_read_slave(ws->slave, ws->reg, ws->data, ws->size);
					break;
				}
				default: {
					;
				}
			}
			ws->retries++;
			timeout = compute_timeout(ws->size, ws->speed) + 1u;
			netimer_after(&ws->timeout, timeout, EVT_TIMEOUT);

		    return (naction_handled());
		}
		case EVT_TRANSFER_COMPLETE: {
			struct ni2c_transfer_completed_event * completed;

			completed = (struct ni2c_transfer_completed_event *)nevent_create(
					sizeof(struct ni2c_transfer_completed_event),
					EVT_NI2C_TRANSFER_COMPLETED);

			completed->retries = ws->retries;
			nepa_send_event(
				ws->client,
				(nevent *)completed);
			netimer_cancel(&ws->timeout);

			return (naction_transit_to(sm, state_idle));
		}
		case EVT_ERROR: {
			netimer_cancel(&ws->timeout);
			if (ws->retries == I2C_MAX_RETRIES) {
				struct ni2c_error_event * error;

				error = (struct ni2c_error_event *)event;

				error->event.id = EVT_NI2C_ERROR;
				error->retries = ws->retries;
				nepa_send_event(
					ws->client,
					(nevent *)error);

				return (naction_transit_to(sm, state_idle));
			}

			return (naction_transit_to(sm, state_transfer));
		}
		case EVT_TIMEOUT: {
			netimer_cancel(&ws->timeout);
			if (ws->retries == I2C_MAX_RETRIES) {
				struct ni2c_error_event * error;

				error = (struct ni2c_error_event *)event;

				error->event.id = EVT_NI2C_ERROR;
				error->retries = ws->retries;
				error->error_id = NI2C_TIMEOUT;
				nepa_send_event(
					ws->client,
					(nevent *)error);

				return (naction_transit_to(sm, state_idle));
			}

			return (naction_transit_to(sm, state_transfer));
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
 * END of i2c.c
 ******************************************************************************/


