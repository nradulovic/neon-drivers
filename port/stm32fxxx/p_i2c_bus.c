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

#include "mcu/peripheral.h"
#include "mcu/profile.h"

/* NOTE:
 * The condition will turn off this module if not enabled or not available in
 * the current port. This kind of if/endif protection is necessary because this
 * source file may define unwanted ISRs.
 */
#if (NPROFILE_EN_I2C)
#include "base/debug.h"
#include "base/component.h"
#include "mcu/i2c_bus.h"
#include "neon-drivers/port/stm32fxxx/family/p_i2c_bus.h"

/*=========================================================  LOCAL MACRO's  ==*/
#define NI2C_BUS_ADDRESSING_MODE												\
	(NI2C_BUS_ADDRESS_7BIT | NI2C_BUS_ADDRESS_10BIT)

#define NI2C_BUS_SPEED															\
	(NI2C_BUS_SPEED_100 | NI2C_BUS_SPEED_400 | NI2C_BUS_SPEED_1700 | 			\
	NI2C_BUS_SPEED_3400)

#define NI2C_BUS_HANDLING														\
	(NI2C_BUS_HANDLING_IT | NI2C_BUS_HANDLING_DMA)

#define get_flag(handle, flag)													\
		__HAL_I2C_GET_FLAG(handle, flag)

#define clear_flag(handle, flag)												\
		__HAL_I2C_CLEAR_FLAG(handle, flag)

#define clear_address_flag(handle)												\
	__HAL_I2C_CLEAR_ADDRFLAG(handle)

#define is_master_mode(handle) 													\
	get_flag(handle, I2C_FLAG_MSL)

#define is_transmit_mode(handle) 												\
	get_flag(handle, I2C_FLAG_TRA)

#define is_start_generated(handle)												\
	get_flag(handle, I2C_FLAG_SB)

#define is_address_sent(handle)													\
	get_flag(handle, I2C_FLAG_ADDR)

#define is_txe(handle)															\
	get_flag(handle, I2C_FLAG_TXE)

#define is_rxne(handle)															\
	get_flag(handle, I2C_FLAG_RXNE)

#define is_btf(handle)															\
	get_flag(handle, I2C_FLAG_BTF)

#define is_buss_error(handle)													\
	get_flag(handle, I2C_FLAG_BERR)

#define is_arbitration_lost(handle)												\
	get_flag(handle, I2C_FLAG_ARLO)

#define is_over_under_run(handle)												\
	get_flag(handle, I2C_FLAG_OVR)

#define is_acknowledge_failure(handle) 											\
	get_flag(handle, I2C_FLAG_AF)

#define generate_start(handle)													\
	handle->Instance->CR1 |= I2C_CR1_START

#define generate_stop(handle)													\
	handle->Instance->CR1 |= I2C_CR1_STOP

#define enable_it(handle, interrupt)											\
	__HAL_I2C_ENABLE_IT(handle, interrupt)

#define disable_it(handle, interrupt)											\
	__HAL_I2C_DISABLE_IT(handle, interrupt)

#define enable_ack(handle)														\
	handle->Instance->CR1 |= I2C_CR1_ACK

#define disable_ack(handle)														\
	handle->Instance->CR1 &= ~I2C_CR1_ACK

#define enable_pos(handle)														\
	handle->Instance->CR1 |= I2C_CR1_POS

#define disable_pos(handle)														\
	handle->Instance->CR1 &= ~I2C_CR1_POS

#define software_reset(handle)													\
	handle->Instance->CR1 |= I2C_CR1_SWRST

/*======================================================  LOCAL DATA TYPES  ==*/
/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/
/*=======================================================  LOCAL VARIABLES  ==*/

static const NCOMPONENT_DEFINE("STM32Fxxx GPIO driver", "Nenad Radulovic");

/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/

static inline void notify_error(struct ni2c_bus_driver * bus, enum ni2c_error_type error)
{
//	nevent * event;
//
//	event = nevent_create_i(sizeof(struct ni2c_error_event), EVT_NI2C_WRITE_COMPLETED);
//	((struct ni2c_error_event *)event)->type = error;
//	nepa_send_event_i(bus->client, event);
}



static inline void notify_write_complete(struct ni2c_bus_driver * bus)
{
//	nevent * event;
//
//	event = nevent_create_i(sizeof(event), EVT_NI2C_WRITE_COMPLETED);
//	nepa_send_event_i(bus->client, event);
}



static inline void notify_read_complete(struct ni2c_bus_driver * bus)
{
//	nevent * event;
//
//	event = nevent_create_i(sizeof(event), EVT_NI2C_READ_COMPLETED);
//	nepa_send_event_i(bus->client, event);
}



static inline struct ni2c_bus_driver * pdrv_to_ni2c_driver(
 	struct npdrv * 				pdrv)
{
 	return (PORT_C_CONTAINER_OF(pdrv, struct ni2c_bus_driver, pdrv));
}



void ni2c_port_config(
 	uint32_t 					bus_id,
 	uint32_t 					config)
{
 	struct ni2c_bus_driver *	ndrv;
 	struct npdrv *				pdrv;
 	I2C_HandleTypeDef  *  		handle;

 	pdrv = npdrv_from_id(bus_id);

 	NREQUIRE(NAPI_USAGE "Invalid bus_id.", pdrv != NULL);

 	ndrv = pdrv_to_ni2c_driver(pdrv);
 	handle = &ndrv->ctx.handle;

 	if ((config & I2C_DUAL_ADDRESS) == NI2C_DUALADDRESS_ENABLE) {
 		handle->Init.DualAddressMode = I2C_DUALADDRESS_ENABLE;
 	} else {
 		handle->Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
 	}

 	if ((config & I2C_FM_DUTYCYCLE) == NI2C_FM_DUTYCYCLE_2) {
 		handle->Init.DutyCycle = I2C_DUTYCYCLE_2;
 	} else {
 		handle->Init.DutyCycle = I2C_DUTYCYCLE_16_9;
 	}

 	if ((config & I2C_NOSTRETCH) == NI2C_NOSTRETCH_ENABLE) {
 		handle->Init.NoStretchMode = I2C_NOSTRETCH_ENABLE;
 	} else {
 		handle->Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
 	}

 	handle->Init.OwnAddress2 = (config & NI2C_OWN_ADDRESS_2(0xFF)) >> 8 ;
}



static inline void terminate_trnsmission(struct ni2c_bus_driver * bus)
{
	I2C_HandleTypeDef  *  		handle;

	handle = &bus->ctx.handle;

	if (bus->slave->flags == NI2C_TRANSFER_NORMAL) {
		generate_stop(handle);
	} else {
		if (bus->slave->require_stop == false) {
			bus->slave->require_stop = true;
		} else {
			bus->slave->require_stop = false;
			generate_stop(handle);
		}
	}
}



static inline void master_transmit(struct ni2c_bus_driver * bus)
{
	I2C_HandleTypeDef  *  		handle;

	handle = &bus->ctx.handle;

	if (handle->XferCount != 0) {
		handle->Instance->DR = (*handle->pBuffPtr++);
		handle->XferCount--;
	} else {
    	disable_it(handle, I2C_IT_BUF);											/* Disable BUF interrupt */
    	if (is_btf(handle) == SET) {
        	disable_it(handle, I2C_IT_EVT | I2C_IT_ERR);						/* Disable EVT and ERR interrupt */
        	terminate_trnsmission(bus);
        	handle->State = HAL_I2C_STATE_BUSY;

        	notify_write_complete(bus);
    	}
    }
}



static inline void prepare_receiver(struct ni2c_bus_driver * bus)
{
	I2C_HandleTypeDef  *  		handle;

	handle = &bus->ctx.handle;

    if (handle->XferSize == 1) {
    	disable_ack(handle);
    	clear_address_flag(handle);
    	terminate_trnsmission(bus);
    } else {
    	enable_ack(handle);
    }
}



static inline void master_receive(struct ni2c_bus_driver * bus)
{
	I2C_HandleTypeDef  *  		handle;

	handle = &bus->ctx.handle;

    (*handle->pBuffPtr++) = handle->Instance->DR;
    handle->XferCount--;

	if (handle->XferCount == 1) {
		disable_ack(handle);
		terminate_trnsmission(bus);
	} else {
		if (handle->XferCount == 0) {
			disable_it(handle, I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR);
			handle->State = HAL_I2C_STATE_BUSY;

		   notify_read_complete(bus);
		}
	}
}

/*===========================================  GLOBAL FUNCTION DEFINITIONS  ==*/

struct ni2c_bus_driver * i2c_bus_init(
    uint32_t              		bus_id,
    uint32_t					config)
{
	struct ni2c_bus_driver *	ndrv;
	struct npdrv *				pdrv;
	const struct npdev * 		pdev;
	I2C_HandleTypeDef  *  		handle;

	pdrv = npdrv_request(bus_id);

	NREQUIRE(NAPI_USAGE "Invalid bus_id.", pdrv != NULL);

	pdev = nprofile_pdev(bus_id);
	ndrv = pdrv_to_ni2c_driver(pdrv);
	handle = &ndrv->ctx.handle;
	handle->Instance = (I2C_TypeDef *)npdev_address(pdev);

	NREQUIRE(NAPI_USAGE "Invalid HAL handle.", handle != NULL);

	if ((config & NI2C_BUS_ADDRESSING_MODE) == NI2C_BUS_ADDRESS_7BIT) {
		handle->Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	} else {
		handle->Init.AddressingMode = I2C_ADDRESSINGMODE_10BIT;
	}

	switch (config & NI2C_BUS_SPEED) {
		case NI2C_BUS_SPEED_100 : {
			handle->Init.ClockSpeed = 100000;
			break;
		}
		case NI2C_BUS_SPEED_400 : {
			handle->Init.ClockSpeed = 400000;
			break;
		}
		default : {
			NASSERT_ALWAYS(NAPI_USAGE "Invalid speed argument.");
		}
	}

	ni2c_port_config(bus_id, NI2C_DUALADDRESS_DISABLE | NI2C_FM_DUTYCYCLE_2 |
			NI2C_NOSTRETCH_DISABLE | NI2C_OWN_ADDRESS_2(0));

	npdrv_pwr_enable(pdrv, 0);
	handle->State = HAL_I2C_STATE_READY;										/* Don't call HAL_I2C_MspInit(); */
	HAL_I2C_Init(handle);

	if ((config & NI2C_BUS_HANDLING) == NI2C_BUS_HANDLING_IT) {
		ndrv->bus_handling = NI2C_BUS_HANDLING_IT;
		npdrv_isr_set_prio(pdrv, 0, CONFIG_CORE_LOCK_MAX_LEVEL);				/* I2Cx_EV_IRQn Priority */
		npdrv_isr_enable(pdrv, 0);												/* Enable I2Cx_EV_IRQn */
		npdrv_isr_set_prio(pdrv, 1, CONFIG_CORE_LOCK_MAX_LEVEL);				/* I2Cx_ER_IRQn Priority */
		npdrv_isr_enable(pdrv, 1);												/* Enable I2Cx_ER_IRQn */
	} else {
		ndrv->bus_handling = NI2C_BUS_HANDLING_DMA;
	}

	return (ndrv);
}



void i2c_bus_term(
	uint32_t              		bus_id)
{
	struct ni2c_bus_driver *	ndrv;
	struct npdrv *				pdrv;
	I2C_HandleTypeDef  *  		handle;

	pdrv = npdrv_request(bus_id);

	NREQUIRE(NAPI_USAGE "Invalid bus_id.", pdrv != NULL);

	ndrv = pdrv_to_ni2c_driver(pdrv);
	handle = &ndrv->ctx.handle;

	NREQUIRE(NAPI_USAGE "i2c init failed.", HAL_I2C_DeInit(handle) == HAL_OK);
}



void i2c_open_slave(
	struct ni2c_slave *			slave,
	struct ni2c_bus_driver *	bus,
	uint32_t					flags,
	uint32_t					address)
{
	NREQUIRE(NAPI_USAGE "Invalid slave.", slave != NULL);
	NREQUIRE(NAPI_USAGE "Invalid bus.", bus != NULL);

	slave->bus = bus;
	slave->flags = flags;
	slave->address = address;
	slave->require_stop = false;
}



void i2c_write_slave(
	struct ni2c_slave *			slave,
	void *						data,
	size_t						size)
{
	I2C_HandleTypeDef  *  		handle;

	NREQUIRE(NAPI_USAGE "Invalid driver.", slave != NULL);
	NREQUIRE(NAPI_USAGE "Invalid data.", data != NULL);
	NREQUIRE(NAPI_USAGE "Invalid size.", size != 0u);

	handle = &slave->bus->ctx.handle;
	slave->bus->client = nepa_get_current();

    if ((get_flag(handle, I2C_FLAG_BUSY) == SET) || handle->State == HAL_I2C_STATE_BUSY) {
    	if (slave->require_stop != true) {
    		/* todo poslji event EVT_NI2C_BUSY */

    		return;
    	}
    }
    handle->State = HAL_I2C_STATE_BUSY;
    handle->pBuffPtr = (uint8_t *)data;
    handle->XferSize = size;
    handle->XferCount = size;
    slave->bus->data = data;
    slave->bus->size = size;
    slave->bus->slave = slave;
    slave->address = I2C_7BIT_ADD_WRITE(slave->address);
//    netimer_after(&slave->bus->timeout, 1, EVT_NI2C_TIMEOUT);

	if (slave->bus->bus_handling == NI2C_BUS_HANDLING_IT) {
		enable_it(handle, I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR);
		generate_start(handle);
	}
}



void i2c_read_slave(
	struct ni2c_slave *			slave,
	void *						data,
	size_t						size)
{
	I2C_HandleTypeDef  *  		handle;

	NREQUIRE(NAPI_USAGE "Invalid driver.", slave != NULL);
	NREQUIRE(NAPI_USAGE "Invalid data.", data != NULL);
	NREQUIRE(NAPI_USAGE "Invalid size.", size != 0u);

	handle = &slave->bus->ctx.handle;
	slave->bus->client = nepa_get_current();

    if ((get_flag(handle, I2C_FLAG_BUSY) == SET) || handle->State == HAL_I2C_STATE_BUSY) {
    	if (slave->require_stop != true) {
    		/* todo poslji event EVT_NI2C_BUSY */

    		return;
    	}
    }
    handle->State = HAL_I2C_STATE_BUSY;
    handle->pBuffPtr = (uint8_t *)data;
    handle->XferSize = size;
    handle->XferCount = size;
    slave->bus->data = data;
    slave->bus->size = size;
    slave->bus->slave = slave;
    slave->address = I2C_7BIT_ADD_READ(slave->address);
    slave->bus->client = nepa_get_current();
//    netimer_after(&slave->bus->timeout, 1, EVT_NI2C_TIMEOUT);

	if (slave->bus->bus_handling == NI2C_BUS_HANDLING_IT) {
		enable_it(handle, I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR);
		generate_start(handle);
	}
}


void i2c_general_call(
	struct ni2c_bus_driver *	bus,
	uint8_t						command)
{
	NREQUIRE(NAPI_USAGE "Invalid command.", command != 0u);

	static struct ni2c_slave 	slave_struct;
	I2C_HandleTypeDef  *  		handle;
	struct ni2c_slave *			slave;

	slave = &slave_struct;
	handle = &slave->bus->ctx.handle;
	slave->bus->client = nepa_get_current();

	i2c_open_slave(slave, bus, NI2C_TRANSFER_NORMAL, 0);

    if ((get_flag(handle, I2C_FLAG_BUSY) == SET) || handle->State == HAL_I2C_STATE_BUSY) {
    	if (slave->require_stop != true) {
    		/* todo poslji event EVT_NI2C_BUSY */

    		return;
    	}
    }
    handle->State = HAL_I2C_STATE_BUSY;
    handle->pBuffPtr = (uint8_t *)&command;
    handle->XferSize = 1;
    handle->XferCount = 1;
    slave->bus->data = &command;
    slave->bus->size = 1;
    slave->bus->slave = slave;
//    netimer_after(&slave->bus->timeout, 1, EVT_NI2C_TIMEOUT);

	if (slave->bus->bus_handling == NI2C_BUS_HANDLING_IT) {
		enable_it(handle, I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR);
		generate_start(handle);
	}
}



#include "mcu/gpio.h"

void ni2c_event_isr(
	struct ni2c_bus_driver *	bus)
{
ngpio_set(NGPIO_ID(NGPIOG, 14));

	I2C_HandleTypeDef  *  		handle;

	NREQUIRE(NAPI_USAGE "Invalid driver.", bus!= NULL);

	handle = &bus->ctx.handle;

	if (is_address_sent(handle) == SET) {
		if (is_transmit_mode(handle) != SET) {
			prepare_receiver(bus);
		}
		clear_address_flag(handle);											/* Clear ADDR flag */
	}
	if (is_start_generated(handle) == SET) {
		handle->Instance->DR = bus->slave->address;							/* Send slave address */
ngpio_clear(NGPIO_ID(NGPIOG, 14));
		return;
	}
	if (is_transmit_mode(handle) == SET) {									/* I2C in mode Transmitter */
		if ((is_txe(handle) == SET) || (is_btf(handle) == SET)) {			/* TXE set or BTF set */
			master_transmit(bus);
		}
	} else {																/* I2C in mode Receiver */
		if ((is_rxne(handle) == SET) || (is_btf(handle) == SET)) {			/* RXNE set or BTF set */
			master_receive(bus);
		}
	}
ngpio_clear(NGPIO_ID(NGPIOG, 14));
}

#include "i2c_test.h"

void ni2c_error_isr(
	struct ni2c_bus_driver *	bus)
{
	I2C_HandleTypeDef  *  		handle;
g_i2c1_error = true;
	NREQUIRE(NAPI_USAGE "Invalid driver.", bus!= NULL);

	handle = &bus->ctx.handle;
	handle->State = HAL_I2C_STATE_READY;
//	netimer_cancel(&slave->bus->timeout);

	if (bus->slave->flags == NI2C_TRANSFER_COMBINED) {
		bus->slave->require_stop = false;
	}

	if (is_buss_error(handle) == SET) {
		clear_flag(handle, I2C_FLAG_BERR);
		generate_stop(handle);
		notify_error(bus, NI2C_BUS_ERROR);

		return;
	}
	if (is_arbitration_lost(handle) == SET) {
		clear_flag(handle, I2C_FLAG_ARLO);
		notify_error(bus, NI2C_ARBITRATION_LOST);

		return;
	}
	if (is_acknowledge_failure(handle) == SET) {
		clear_flag(handle, I2C_FLAG_AF);
		generate_stop(handle);
		notify_error(bus, NI2C_ACKNOWLEDGE_FAILURE);

		return;
	}
	if (is_over_under_run(handle) == SET) {
		clear_flag(handle, I2C_FLAG_OVR);
		generate_stop(handle);
		notify_error(bus, NI2C_BUS_OVERFLOW);

		return;

	}

}

#endif /* NPROFILE_EN_I2C */
/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of p_i2c_bus.c
 ******************************************************************************/


