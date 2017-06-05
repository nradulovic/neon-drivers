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
 * @author      Dejan Ristic
 * @brief       Pin Peripheral Multiplexer
 * @defgroup    mux_intf Pin Peripheral Multiplexer
 * @brief       Pin Peripheral Multiplexer
 *********************************************************************//** @{ */

#ifndef NEON_DRIVERS_MCU_MUX_H_
#define NEON_DRIVERS_MCU_MUX_H_

/*=========================================================  INCLUDE FILES  ==*/

#include "family/p_mux.h"
#include <stddef.h>

/*===============================================================  MACRO's  ==*/
/*------------------------------------------------------  C++ extern begin  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

/**@brief       Portable multiplexer structure
 * @details     This is forward declaration of the multiplexer structure. The
 *              structure is defined in port p_mux.h header.
 */
struct nmux;

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/

/**@brief       Setup multiplexer according to given array
 * @param       mux_array
 *              Pointer to mux array
 * @param       size
 *              The size of mux array in bytes, eg. sizeof(array).
 * @api
 */
void nmux_setup(
    const struct nmux *         mux_array,
    size_t                      size);



/**@brief       Reset mux settings to power-on defaults
 * @param       gpio_id
 *              GPIO id number.
 */
void nmux_reset(
    uint32_t                    gpio_id);

/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of mux.h
 ******************************************************************************/
#endif /* NEON_DRIVERS_MCU_MUX_H_ */
