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
 * @brief 	Driver for microchip MCP23008 and MCP23017 i2c gpio expanders
 *********************************************************************//** @{ */

#ifndef PLATFORM_NEON_DRIVERS_INCLUDE_XIO_MCP23017_H_
#define PLATFORM_NEON_DRIVERS_INCLUDE_XIO_MCP23017_H_

/*=========================================================  INCLUDE FILES  ==*/

#include "xio_class.h"

/*===============================================================  MACRO's  ==*/

#define MCP23017_PINS_QUANTITY	16u

#define REG8_IODIRA_ADDR		0x00
#define REG8_IODIRB_ADDR		0x10
#define REG8_IPOLA_ADDR			0x01
#define REG8_IPOLB_ADDR			0x11
#define REG8_GPINTENA_ADDR		0x02
#define REG8_GPINTENB_ADDR		0x12
#define REG8_DEFVALA_ADDR		0x03
#define REG8_DEFVALB_ADDR		0x13
#define REG8_INTCONA_ADDR		0x04
#define REG8_INTCONB_ADDR		0x14
#define REG8_IOCON_ADDR			0x05
#define REG8_GPPUA_ADDR			0x06
#define REG8_GPPUB_ADDR			0x16
#define REG8_INTFA_ADDR			0x07
#define REG8_INTFB_ADDR			0x17
#define REG8_INTCAPA_ADDR		0x08
#define REG8_INTCAPB_ADDR		0x18
#define REG8_GPIOA_ADDR			0x09
#define REG8_GPIOB_ADDR			0x19
#define REG8_OLATA_ADDR			0x0A
#define REG8_OLATB_ADDR			0x1A

#define REG16_IODIR_ADDR		0x00
#define REG16_IPOL_ADDR			0x02
#define REG16_GPINTEN_ADDR		0x04
#define REG16_DEFVAL_ADDR		0x06
#define REG16_INTCON_ADDR		0x08
#define REG16_IOCON_ADDR		0x0A
#define REG16_GPPU_ADDR			0x0C
#define REG16_INTF_ADDR			0x0E
#define REG16_INTCAP_ADDR		0x10
#define REG16_GPIO_ADDR			0x12
#define REG16_OLAT_ADDR			0x14

/*------------------------------------------------------  C++ extern begin  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

struct PORT_C_PACKED mcp23017_byte_registers
{
	uint8_t						iodira;
	uint8_t						iodirb;
	uint8_t						ipola;
	uint8_t						ipolb;
	uint8_t						gpintena;
	uint8_t						gpintenb;
	uint8_t						defvala;
	uint8_t						defvalb;
	uint8_t						intcona;
	uint8_t						intconb;
	uint8_t						iocon;
	uint8_t						iocon_;
	uint8_t						gppua;
	uint8_t						gppub;
	uint8_t						intfa;
	uint8_t						intfb;
	uint8_t						intcapa;
	uint8_t						intcapb;
	uint8_t						gpioa;
	uint8_t						gpiob;
	uint8_t						olata;
	uint8_t						olatb;
};



struct PORT_C_PACKED mcp23017_word_registers
{
	uint16_t					iodir;
	uint16_t					ipol;
	uint16_t					gpinten;
	uint16_t					defval;
	uint16_t					intcon;
	uint16_t					iocon;
	uint16_t					gppu;
	uint16_t					intf;
	uint16_t					intcap;
	uint16_t					gpio;
	uint16_t					olat;
};



struct mcp23017_driver
{
	struct nxio_driver			nxio_driver;
	struct mcp23017_word_registers	regs;
};

/*======================================================  GLOBAL VARIABLES  ==*/

extern struct nepa 				g_mcp23017_epa;
extern const struct nepa_define g_mcp23017_define;

/*===================================================  FUNCTION PROTOTYPES  ==*/
/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of mcp230xx.h
 ******************************************************************************/
#endif /* PLATFORM_NEON_DRIVERS_INCLUDE_XIO_MCP23017_H_ */
