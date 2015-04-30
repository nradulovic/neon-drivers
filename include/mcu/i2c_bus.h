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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Neon.  If not, see <http://www.gnu.org/licenses/>.
 *
 * web site:    http://github.com/nradulovic
 * e-mail  :    nenad.b.radulovic@gmail.com
 *//***********************************************************************//**
 * @file
 * @author      Nenad Radulovic
 * @brief       I2C operations
 * @defgroup    i2C_intf I2C operations
 * @brief       I2C operations
 *********************************************************************//** @{ */

#ifndef NI2C_BUS_H_
#define NI2C_BUS_H_

/*=========================================================  INCLUDE FILES  ==*/

#include <stdint.h>
#include <stddef.h>

#include "base/error.h"

#include "../../port/stm32fxxx/family/p_i2c_bus.h"
#include "mcu/peripheral.h"
#include "mcu/profile.h"

/*===============================================================  MACRO's  ==*/
/**@brief       Convenience wrapper macro around @ref NP_DEV_CREATE_ID().
 * @param       port
 *              Specify bus number in range 1 - 32, where NI2C1 is 0 etc.
 * @param       pin
 * @return      Returns valid I2C bus ID number which can be used with other
 *              functions and macros.
 */
#define NI2C_ID(bus)             		NP_DEV_CREATE_ID(NPROFILE_CLASS_I2C, bus, 0)

/**@brief       Convenience wrapper macro around @ref NP_DEV_ID_TO_MAJOR().
 * @param       id
 *              Valid I2C bus ID
 * @return      Returns I2C bus number
 */
#define NI2C_ID_TO_BUS(id)            	NP_DEV_ID_TO_MAJOR(id)

#define NI2C1							1
#define NI2C2							2
#define NI2C3							3
#define NI2C4							4
#define NI2C5							5
#define NI2C6							6

#define NI2C_BUS_MODE_MASTER             (0x0u << 0)
#define NI2C_BUS_MODE_SLAVE              (0x1u << 0)
#define NI2C_BUS_ADDRESS_7BIT            (0x0u << 1)
#define NI2C_BUS_ADDRESS_10BIT           (0x1u << 1)
#define NI2C_BUS_ADDRESS(x)              ((x) << 16)

#define NI2C_SLAVE_RD_NACK               (0x1u << 2)
#define NI2C_SLAVE_RD_START_STOP         (0x1u << 2)
#define NI2C_SLAVE_RD_REPEAT             (0x2u << 2)

#define NI2C_BUS_SPEED_100				(0x0u <<  4)
#define NI2C_BUS_SPEED_400				(0x1u <<  4)
#define NI2C_BUS_SPEED_1700				(0x2u <<  4)
#define NI2C_BUS_SPEED_3400				(0x3u <<  4)

#define NI2C_BUS_HANDLING_IT			(0x0u <<  6)
#define NI2C_BUS_HANDLING_DMA			(0x1u <<  6)

#define NI2C_BUS_ADDRESSING_MODE												\
	(NI2C_BUS_ADDRESS_7BIT | NI2C_BUS_ADDRESS_10BIT)

#define NI2C_BUS_SPEED															\
	(NI2C_BUS_SPEED_100 | NI2C_BUS_SPEED_400 | NI2C_BUS_SPEED_1700 | 			\
	NI2C_BUS_SPEED_3400)

#define NI2C_BUS_HANDLING														\
	(NI2C_BUS_HANDLING_IT | NI2C_BUS_HANDLING_DMA)

#define NI2C_ERROR_NONE       ((uint32_t)0x00000000)    /*!< No error           */
#define NI2C_ERROR_BERR       ((uint32_t)0x00000001)    /*!< BERR error         */
#define NI2C_ERROR_ARLO       ((uint32_t)0x00000002)    /*!< ARLO error         */
#define NI2C_ERROR_AF         ((uint32_t)0x00000004)    /*!< AF error           */
#define NI2C_ERROR_OVR        ((uint32_t)0x00000008)    /*!< OVR error          */
#define NI2C_ERROR_DMA        ((uint32_t)0x00000010)    /*!< DMA transfer error */
#define NI2C_ERROR_TIMEOUT    ((uint32_t)0x00000020)    /*!< Timeout Error      */

/*-------------------------------------------------------  C++ extern base  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

struct ni2c_driver
{
    struct npdrv               pdrv;
    I2C_HandleTypeDef 		   handle;
    uint32_t				   bus_handling;
};

typedef void (i2c_action_handler)(uint32_t bus_id);

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/

void i2c_bus_init(
    uint32_t              		bus_id,
    uint32_t					config);



void i2c_bus_term(
	uint32_t              		bus_id);



nerror i2c_bus_master_write(
	uint32_t					bus_id,
	const void	* 				data,
	uint32_t					size,
	i2c_action_handler *		action);



nerror i2c_bus_master_read(
	uint32_t					bus_id,
	void	* 					data,
	uint32_t					size,
	i2c_action_handler *		action);



nerror i2c_bus_slave_write(
	uint32_t					bus_id,
	const void	* 				data,
	uint32_t					size,
	i2c_action_handler *		action);



nerror i2c_bus_slave_read(
	uint32_t					bus_id,
	void	* 					data,
	uint32_t					size,
	i2c_action_handler *		action);



void i2c_bus_start(
	uint32_t 					bus_id,
	i2c_action_handler *		action);



void i2c_bus_stop(
	uint32_t 					bus_id,
	i2c_action_handler *		action);



void i2c_bus_ack(
	uint32_t 					bus_id);



void i2c_bus_nack(
	uint32_t 					bus_id);



/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of gpio.h
 ******************************************************************************/
#endif /* NI2C_H_ */
