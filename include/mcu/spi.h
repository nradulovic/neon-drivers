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
 * @brief       SPI driver
 * @defgroup    spi_intf SPI driver
 * @brief       SPI driver
 *********************************************************************//** @{ */

#ifndef NEON_MCU_SPI_H_
#define NEON_MCU_SPI_H_

/*=========================================================  INCLUDE FILES  ==*/

#include <stdint.h>

/* NOTE:
 * Include family port header file. The file defines custom data types which are
 * needed by the driver.
 */
#include "family/p_spi.h"

/*===============================================================  MACRO's  ==*/
/*-------------------------------------------------------  C++ extern base  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

struct nspi_config
{
    uint32_t                    speed;
    uint32_t                    flags;
};

/**@brief       SPI driver structure
 * @details     The structure is instantiated by profile data source file,
 *              one structure per peripheral.
 */
struct nspi_driver
{
    struct npdrv                pdrv;  /**<@brief Generic peripheral driver
                                         * structure
                                         */
    struct np_dev_spi           ctx;    /**<@brief Custom data type which is
                                         * defined in family port header file.
                                         */
    void                     (* reader)(struct nspi_driver *, void *, size_t);
    void                     (* writer)(struct nspi_driver *, const void *, size_t);
};

struct nspi_transfer
{
    uint32_t                    flags;
    void *                      data;
    size_t                      size;
    void                     (* complete)(struct nspi_driver *, struct nspi_transfer *);
};

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/

struct nspi_driver * nspi_init(uint32_t spi_id, const struct nspi_config * config);

void nspi_reinit(struct nspi_driver * driver, const struct nspi_config * config);

void nspi_transfer(struct nspi_driver * driver, struct nspi_transfer * transfer);

/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of uart.h
 ******************************************************************************/
#endif /* NEON_MCU_SPI_H_ */
