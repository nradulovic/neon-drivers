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
/*======================================================  LOCAL DATA TYPES  ==*/
/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/

static struct ni2c_driver * pdrv_to_ni2c_driver(struct npdrv * pdrv);

/*=======================================================  LOCAL VARIABLES  ==*/

static const NCOMPONENT_DEFINE("STM32Fxxx GPIO driver", "Nenad Radulovic");

/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/

static inline struct ni2c_driver * pdrv_to_ni2c_driver(
	struct npdrv * 				pdrv)
{
	return (PORT_C_CONTAINER_OF(pdrv, struct ni2c_driver, handle));
}

void ni2c_port_config(
	uint32_t 					bus_id,
	uint32_t 					config)
{
	struct ni2c_driver *		ndrv;
	struct npdrv *				pdrv;

	pdrv = npdrv_request(bus_id);

	NREQUIRE(NAPI_USAGE "Invalid bus_id.", pdrv != NULL);

	ndrv = pdrv_to_ni2c_driver(pdrv);

	if (config & I2C_DUAL_ADDRESS == NI2C_DUALADDRESS_ENABLE) {
		ndrv->handle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	} else {
		ndrv->handle.Init.DualAddressMode = I2C_DUALADDRESS_ENABLE;
	}

	if (config & I2C_FM_DUTYCYCLE == NI2C_FM_DUTYCYCLE_2) {
		ndrv->handle.Init.DutyCycle = I2C_DUTYCYCLE_2;
	} else {
		ndrv->handle.Init.DutyCycle = NI2C_FM_DUTYCYCLE_16_9;
	}

	if (config & I2C_NOSTRETCH == NI2C_NOSTRETCH_ENABLE) {
		ndrv->handle.Init.NoStretchMode = I2C_NOSTRETCH_ENABLE;
	} else {
		ndrv->handle.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	}

	ndrv->handle.Init.OwnAddress2 = (config & NI2C_OWN_ADDRESS_2(0xFF)) >> 8 ;
}


/*===========================================  GLOBAL FUNCTION DEFINITIONS  ==*/

void i2c_bus_init(
    uint32_t              		bus_id,
    uint32_t					config)
{
	struct ni2c_driver *		ndrv;
	struct npdrv *				pdrv;
	const struct npdev * 		pdev;

	pdrv = npdrv_request(bus_id);

	NREQUIRE(NAPI_USAGE "Invalid bus_id.", pdrv != NULL);

	pdev = nprofile_pdev(bus_id);
	ndrv = pdrv_to_ni2c_driver(pdrv);

	ndrv->handle.Instance = (I2C_TypeDef *)npdev_address(pdev);

	if (config & NI2C_BUS_ADDRESSING_MODE == I2C_ADDRESSINGMODE_7BIT) {
		ndrv->handle.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	} else {
		ndrv->handle.Init.AddressingMode = I2C_ADDRESSINGMODE_10BIT;
	}

	switch (config & NI2C_BUS_SPEED) {
		case NI2C_BUS_SPEED_100 : {
			ndrv->handle.Init.ClockSpeed = 100000;
			break;
		}
		case NI2C_BUS_SPEED_400 : {
			ndrv->handle.Init.ClockSpeed = 400000;
			break;
		}
		default : {
			NASSERT_ALWAYS(NAPI_USAGE "Invalid speed argument.");
		}
	}

	if (config & NI2C_BUS_HANDLING == NI2C_BUS_HANDLING_IT) {
		ndrv->bus_handling = NI2C_BUS_HANDLING_IT;
	} else {
		ndrv->bus_handling = NI2C_BUS_HANDLING_DMA;
	}

	ni2c_port_config(bus_id, NI2C_DUALADDRESS_DISABLE | NI2C_FM_DUTYCYCLE_2 |
			NI2C_NOSTRETCH_DISABLE | NI2C_OWN_ADDRESS_2(0));

	NREQUIRE(NAPI_USAGE "i2c init failed.", HAL_I2C_Init(&ndrv->handle) == HAL_OK);
}



void i2c_bus_term(
	uint32_t              		bus_id)
{
	struct ni2c_driver *		ndrv;
	struct npdrv *				pdrv;

	pdrv = npdrv_request(bus_id);

	NREQUIRE(NAPI_USAGE "Invalid bus_id.", pdrv != NULL);

	ndrv = pdrv_to_ni2c_driver(pdrv);

	NREQUIRE(NAPI_USAGE "i2c init failed.", HAL_I2C_DeInit(&ndrv->handle) == HAL_OK);
}



nerror i2c_bus_master_write(
	uint32_t					bus_id,
	const void	* 				data,
	uint32_t					size,
	i2c_action_handler *		action)
{
	struct ni2c_driver *		ndrv;
	struct npdrv *				pdrv;

	pdrv = npdrv_request(bus_id);

	NREQUIRE(NAPI_USAGE "Invalid data.", data != NULL);
	NREQUIRE(NAPI_USAGE "Invalid size.", size != 0u);
	NREQUIRE(NAPI_USAGE "Invalid bus_id.", pdrv != NULL);

	ndrv = pdrv_to_ni2c_driver(pdrv);

    ndrv->handle.State = HAL_I2C_STATE_BUSY_TX;
    ndrv->handle.ErrorCode = NI2C_ERROR_NONE;

    ndrv->handle.pBuffPtr = (uint8_t *)data;
    ndrv->handle.XferSize = size;
    ndrv->handle.XferCount = size;

	if (ndrv->bus_handling == NI2C_BUS_HANDLING_IT) {
		__HAL_I2C_ENABLE_IT(&ndrv->handle, I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR);
	}
}



nerror i2c_bus_master_read(
	uint32_t					bus_id,
	void	* 					data,
	uint32_t					size,
	i2c_action_handler *		action)
{

}



nerror i2c_bus_slave_write(
	uint32_t					bus_id,
	const void	* 				data,
	uint32_t					size,
	i2c_action_handler *		action)
{

}



nerror i2c_bus_slave_read(
	uint32_t					bus_id,
	void	* 					data,
	uint32_t					size,
	i2c_action_handler *		action)
{

}



void i2c_bus_start(
	uint32_t 					bus_id,
	i2c_action_handler *		action)
{

}



void i2c_bus_stop(
	uint32_t 					bus_id,
	i2c_action_handler *		action)
{

}



void i2c_bus_ack(
	uint32_t 					bus_id)
{

}



void i2c_bus_nack(
	uint32_t 					bus_id)
{

}

void ni2c_event_isr(
	uint32_t 					bus_id)
{
	struct ni2c_driver *		ndrv;
	struct npdrv *				pdrv;
	uint32_t 					tmp1 = 0;
	uint32_t 					tmp2 = 0;
	uint32_t 					tmp3 = 0;
	uint32_t 					tmp4 = 0;

	pdrv = npdrv_request(bus_id);

	NREQUIRE(NAPI_USAGE "Invalid bus_id.", pdrv != NULL);

	ndrv = pdrv_to_ni2c_driver(pdrv);

    if (__HAL_I2C_GET_FLAG(&ndrv->handle, I2C_FLAG_MSL) == SET) {				/* Master mode selected */
        if (__HAL_I2C_GET_FLAG(&ndrv->handle, I2C_FLAG_TRA) == SET) {			/* I2C in mode Transmitter */
            tmp1 = __HAL_I2C_GET_FLAG(&ndrv->handle, I2C_FLAG_TXE);
            tmp2 = __HAL_I2C_GET_IT_SOURCE(&ndrv->handle, I2C_IT_BUF);
            tmp3 = __HAL_I2C_GET_FLAG(&ndrv->handle, I2C_FLAG_BTF);
            tmp4 = __HAL_I2C_GET_IT_SOURCE(&ndrv->handle, I2C_IT_EVT);
            if ((tmp1 == SET) && (tmp2 == SET) && (tmp3 == RESET)) {			/* TXE set and BTF reset */
            	I2C_MasterTransmit_TXE(&ndrv->handle);
            } else {
            	if((tmp3 == SET) && (tmp4 == SET)) {							/* BTF set */
            		I2C_MasterTransmit_BTF(&ndrv->handle);
            	}
            }
        } else {																/* I2C in mode Receiver */
		    tmp1 = __HAL_I2C_GET_FLAG(&ndrv->handle, I2C_FLAG_RXNE);
		    tmp2 = __HAL_I2C_GET_IT_SOURCE(&ndrv->handle, I2C_IT_BUF);
		    tmp3 = __HAL_I2C_GET_FLAG(&ndrv->handle, I2C_FLAG_BTF);
		    tmp4 = __HAL_I2C_GET_IT_SOURCE(&ndrv->handle, I2C_IT_EVT);
		    if((tmp1 == SET) && (tmp2 == SET) && (tmp3 == RESET)) {				/* RXNE set and BTF reset */
		    	I2C_MasterReceive_RXNE(&ndrv->handle);
		    } else {															/* BTF set */
		    	if((tmp3 == SET) && (tmp4 == SET)) {
		    		I2C_MasterReceive_BTF(&ndrv->handle);
		    	}
		    }
        }
    }
#if 0
  /* Slave mode selected */
  else
  {
    tmp1 = __HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_ADDR);
    tmp2 = __HAL_I2C_GET_IT_SOURCE(hi2c, (I2C_IT_EVT));
    tmp3 = __HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_STOPF);
    tmp4 = __HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_TRA);
    /* ADDR set --------------------------------------------------------------*/
    if((tmp1 == SET) && (tmp2 == SET))
    {
      I2C_Slave_ADDR(hi2c);
    }
    /* STOPF set --------------------------------------------------------------*/
    else if((tmp3 == SET) && (tmp2 == SET))
    {
      I2C_Slave_STOPF(hi2c);
    }
    /* I2C in mode Transmitter -----------------------------------------------*/
    else if(tmp4 == SET)
    {
      tmp1 = __HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_TXE);
      tmp2 = __HAL_I2C_GET_IT_SOURCE(hi2c, I2C_IT_BUF);
      tmp3 = __HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_BTF);
      tmp4 = __HAL_I2C_GET_IT_SOURCE(hi2c, I2C_IT_EVT);
      /* TXE set and BTF reset -----------------------------------------------*/
      if((tmp1 == SET) && (tmp2 == SET) && (tmp3 == RESET))
      {
        I2C_SlaveTransmit_TXE(hi2c);
      }
      /* BTF set -------------------------------------------------------------*/
      else if((tmp3 == SET) && (tmp4 == SET))
      {
        I2C_SlaveTransmit_BTF(hi2c);
      }
    }
    /* I2C in mode Receiver --------------------------------------------------*/
    else
    {
      tmp1 = __HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_RXNE);
      tmp2 = __HAL_I2C_GET_IT_SOURCE(hi2c, I2C_IT_BUF);
      tmp3 = __HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_BTF);
      tmp4 = __HAL_I2C_GET_IT_SOURCE(hi2c, I2C_IT_EVT);
      /* RXNE set and BTF reset ----------------------------------------------*/
      if((tmp1 == SET) && (tmp2 == SET) && (tmp3 == RESET))
      {
        I2C_SlaveReceive_RXNE(hi2c);
      }
      /* BTF set -------------------------------------------------------------*/
      else if((tmp3 == SET) && (tmp4 == SET))
      {
        I2C_SlaveReceive_BTF(hi2c);
      }
    }
  }
#endif
}

#endif /* NPROFILE_EN_I2C */
/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of p_i2c_bus.c
 ******************************************************************************/


