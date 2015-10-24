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
#include "family/p_i2c_bus.h"

/*=========================================================  LOCAL MACRO's  ==*/

#define MAX_RETRIES						3u
#define REPEATED_START_TIMEOUT_US		100u

#define get_flag(handle, flag)													\
		__HAL_I2C_GET_FLAG(handle, flag)

#define clear_flag(handle, flag)												\
		__HAL_I2C_CLEAR_FLAG(handle, flag)

#define clear_address_flag(handle)												\
	__HAL_I2C_CLEAR_ADDRFLAG(handle)

#define is_busy(handle)															\
	get_flag(handle, I2C_FLAG_BUSY)

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

enum transfer_phase
{
	ADDRESS_TRANSFER,
	DATA_TRANSFER
};



enum transfer_format
{
	NORMAL_TRANSFER,
	COMBINED_TRANSFER
};



enum transfer_type
{
	WRITE_TO_SLAVE,
	READ_FROM_SLAVE
};

/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/
/*=======================================================  LOCAL VARIABLES  ==*/

static const NCOMPONENT_DEFINE("STM32Fxxx I2C bus driver");
static uint32_t g_repeated_start_timeout;

/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/

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



static inline void handle_combined_transfer(struct ni2c_bus_driver * bus)
{
	I2C_HandleTypeDef  *  		handle;

	handle = &bus->ctx.handle;

	if (bus->phase == ADDRESS_TRANSFER) {
		uint32_t timeout = 0u;

		bus->phase = DATA_TRANSFER;
		handle->pBuffPtr = (uint8_t *)bus->data;
		handle->XferSize = bus->size;
		handle->XferCount = bus->size;
		bus->slave->address = I2C_7BIT_ADD_READ(bus->slave->address);
		generate_start(handle);
		while (handle->Instance->CR1 & I2C_CR1_START) {
			timeout++;
			if (timeout > g_repeated_start_timeout) {
				bus->error = NI2C_BUS_COLISION_ERROR;
				bus->slave->error(bus->slave, bus->error);

				break;
			}
		}
	} else {
		bus->phase = ADDRESS_TRANSFER;
		disable_it(handle, I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR);
		generate_stop(handle);
		ntimer_cancel_i(&bus->recovery_period);

		bus->slave->transfer(bus->slave);
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
		if (bus->format == NORMAL_TRANSFER) {
			if (bus->phase == ADDRESS_TRANSFER) {
				bus->phase = DATA_TRANSFER;
				handle->pBuffPtr = (uint8_t *)bus->data;
				handle->XferSize = bus->size;
				handle->XferCount = bus->size;

				handle->Instance->DR = (*handle->pBuffPtr++);
				handle->XferCount--;
			} else {
				bus->phase = ADDRESS_TRANSFER;
				disable_it(handle, I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR);
				generate_stop(handle);
				ntimer_cancel_i(&bus->recovery_period);

				bus->slave->transfer(bus->slave);
			}
		} else {
			handle_combined_transfer(bus);
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
    	if (bus->format == NORMAL_TRANSFER) {
    		generate_stop(handle);
    	}
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

    if (handle->XferCount == 0) {
		if (bus->format == NORMAL_TRANSFER) {
			if (((bus->slave->flags & NI2C_READ_MODE) == NI2C_READ_WITHOUT_RESTART) &&
				(bus->phase == ADDRESS_TRANSFER)) {
				bus->phase = DATA_TRANSFER;
				handle->pBuffPtr = (uint8_t *)bus->data;
				handle->XferSize = bus->size;
				handle->XferCount = bus->size;
			} else {
				bus->phase = ADDRESS_TRANSFER;
				disable_it(handle, I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR);
				ntimer_cancel_i(&bus->recovery_period);

				bus->slave->transfer(bus->slave);
			}
		} else {
			handle_combined_transfer(bus);
		}
	}
	if (handle->XferCount == 1) {
		disable_ack(handle);
		generate_stop(handle);
	}
}



PORT_C_INLINE_ALWAYS void start_transfer(
	struct ni2c_bus_driver * bus)
{
	I2C_HandleTypeDef  * handle;

	handle = &bus->ctx.handle;
    bus->phase = ADDRESS_TRANSFER;
	if (bus->bus_handling == NI2C_BUS_HANDLING_IT) {
		enable_it(handle, I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR);
		generate_start(handle);
	}
}



static inline void i2c_transfer(
	struct ni2c_slave *			slave,
	uint32_t					reg,
	enum transfer_type			type,
	void *						data,
	size_t						size)
{
	struct ni2c_bus_driver * 	bus;
	I2C_HandleTypeDef  *  		handle;

	slave->bus->reg = reg;
	bus = slave->bus;
	handle = &slave->bus->ctx.handle;

	if (is_busy(handle)) {
		bus->error = NI2C_BUS_OVERFLOW;
		bus->slave->error(bus->slave, bus->error);

		return;
	}
    handle->pBuffPtr = (uint8_t *)&slave->bus->reg;
    handle->XferSize = slave->flags & NI2C_REG_SIZE;
    handle->XferCount = handle->XferSize;
    bus->data = data;
    bus->size = size;
    bus->slave = slave;
    bus->type = type;
    bus->retry = 0;

    if (type == WRITE_TO_SLAVE) {
    	bus->format = NORMAL_TRANSFER;
    	slave->address = I2C_7BIT_ADD_WRITE(slave->address);
    } else {
    	if ((slave->flags & NI2C_READ_MODE) == NI2C_READ_WITH_RESTART) {
    		bus->format = COMBINED_TRANSFER;
    		slave->address = I2C_7BIT_ADD_WRITE(slave->address);
    	} else {
    		bus->format = NORMAL_TRANSFER;
    		slave->address = I2C_7BIT_ADD_READ(slave->address);
    	}
    }
    start_transfer(bus);
}



void recovery(void * arg)
{
	struct ni2c_bus_driver * bus;

	bus = (struct ni2c_bus_driver *)arg;

	start_transfer(bus);
}

/*===========================================  GLOBAL FUNCTION DEFINITIONS  ==*/

void ni2c_bus_init(
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

	ntimer_init(&ndrv->recovery_period);
	g_repeated_start_timeout = SystemCoreClock / (REPEATED_START_TIMEOUT_US * 100u);
	g_repeated_start_timeout /= 4u;												/* Four instructions to load and compare variable */
	if ((config & NI2C_BUS_HANDLING) == NI2C_BUS_HANDLING_IT) {
		ndrv->bus_handling = NI2C_BUS_HANDLING_IT;
		npdrv_isr_set_prio(pdrv, 0, CONFIG_CORE_LOCK_MAX_LEVEL);				/* I2Cx_EV_IRQn Priority */
		npdrv_isr_enable(pdrv, 0);												/* Enable I2Cx_EV_IRQn */
		npdrv_isr_set_prio(pdrv, 1, CONFIG_CORE_LOCK_MAX_LEVEL);				/* I2Cx_ER_IRQn Priority */
		npdrv_isr_enable(pdrv, 1);												/* Enable I2Cx_ER_IRQn */
	} else {
		ndrv->bus_handling = NI2C_BUS_HANDLING_DMA;
	}
}



void ni2c_bus_term(
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



void ni2c_open_slave(
	struct ni2c_slave *			slave,
	struct ni2c_bus_driver *	bus,
	uint32_t					flags,
	uint32_t					address,
	void						(* transfer)(struct ni2c_slave * slave),
	void						(* error)(struct ni2c_slave * slave, enum ni2c_bus_error id))
{
	NREQUIRE(NAPI_USAGE "Invalid slave.", slave != NULL);
	NREQUIRE(NAPI_USAGE "Invalid bus.", bus != NULL);
	NREQUIRE(NAPI_USAGE "Invalid writer.", transfer != NULL);
	NREQUIRE(NAPI_USAGE "Invalid error.", error != NULL);

	slave->bus = bus;
	slave->flags = flags;
	slave->address = address;
	slave->transfer = transfer;
	slave->error = error;
}



void ni2c_write_slave(
	struct ni2c_slave *			slave,
	uint32_t					reg,
	void *						data,
	size_t						size)
{
	NREQUIRE(NAPI_USAGE "Invalid driver.", slave != NULL);
	NREQUIRE(NAPI_USAGE "Invalid data.", data != NULL);
	NREQUIRE(NAPI_USAGE "Invalid size.", size != 0u);

	i2c_transfer(slave, reg, WRITE_TO_SLAVE, data, size);
}



void ni2c_read_slave(
	struct ni2c_slave *			slave,
	uint32_t					reg,
	void *						data,
	size_t						size)
{
	NREQUIRE(NAPI_USAGE "Invalid driver.", slave != NULL);
	NREQUIRE(NAPI_USAGE "Invalid data.", data != NULL);
	NREQUIRE(NAPI_USAGE "Invalid size.", size != 0u);

	i2c_transfer(slave, reg, READ_FROM_SLAVE, data, size);
}



void ni2c_event_isr(
	struct ni2c_bus_driver *	bus)
{
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
}



void ni2c_error_isr(
	struct ni2c_bus_driver *	bus)
{
	I2C_HandleTypeDef  *  		handle;

	NREQUIRE(NAPI_USAGE "Invalid driver.", bus!= NULL);

	handle = &bus->ctx.handle;

	if (is_buss_error(handle) == SET) {
		clear_flag(handle, I2C_FLAG_BERR);
		generate_stop(handle);
		bus->error = NI2C_BUS_COLISION_ERROR;

		goto RETRY;
	}
	if (is_arbitration_lost(handle) == SET) {
		clear_flag(handle, I2C_FLAG_ARLO);
		bus->error = NI2C_BUS_ARBITRATION_LOST;

		goto RETRY;
	}
	if (is_acknowledge_failure(handle) == SET) {
		clear_flag(handle, I2C_FLAG_AF);
		generate_stop(handle);
		bus->error = NI2C_BUS_ACKNOWLEDGE_FAILURE;

		goto RETRY;
	}
	if (is_over_under_run(handle) == SET) {
		clear_flag(handle, I2C_FLAG_OVR);
		generate_stop(handle);
		bus->error = NI2C_BUS_OVERFLOW;

		goto RETRY;
	}

RETRY:
	if (bus->retry < MAX_RETRIES) {
		bus->retry++;
		ntimer_start_i(&bus->recovery_period, 1u, recovery, (void *)bus, NTIMER_ATTR_ONE_SHOT);
	} else {
		bus->slave->error(bus->slave, bus->error);
	}
}

#endif /* NPROFILE_EN_I2C */
/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of p_i2c_bus.c
 ******************************************************************************/


