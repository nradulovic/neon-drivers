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
 * @author      dejan
 * @brief       ab_rtcmc_32768_eoz9.h
 * @defgroup    ab_rtcmc_32768_eoz9.h_intf ab_rtcmc_32768_eoz9.h
 * @brief       ab_rtcmc_32768_eoz9.h
 *********************************************************************//** @{ */

#ifndef AB_RTCMC_32768_EOZ9_H_
#define AB_RTCMC_32768_EOZ9_H_

/*=========================================================  INCLUDE FILES  ==*/

#include <stdint.h>
#include "mcu/i2c.h"
#include "mcu/config.h"
#include "neon_eds.h"
#include "rtc_class.h"

/*===============================================================  MACRO's  ==*/

#define AB_RTC_CLOCKOUT_32768HZ     (0u << 0)
#define AB_RTC_CLOCKOUT_1024HZ      (1u << 0)
#define AB_RTC_CLOCKOUT_32HZ        (2u << 0)
#define AB_RTC_CLOCKOUT_1HZ         (3u << 0)

#define AB_RTC_CLOCKOUT_FREQ                                                    \
    (AB_RTC_CLOCKOUT_32768HZ | AB_RTC_CLOCKOUT_1024HZ | AB_RTC_CLOCKOUT_32HZ |  \
    AB_RTC_CLOCKOUT_1HZ)

/*-------------------------------------------------------  C++ extern base  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

struct ab_nrtc_custom_config
{
    uint32_t                    config;
};

/*======================================================  GLOBAL VARIABLES  ==*/

extern struct nepa              g_ab_rtc_epa;
extern const struct nepa_define g_ab_rtc_define;

/*===================================================  FUNCTION PROTOTYPES  ==*/
/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of ab_rtcmc_32768_eoz9.h
 ******************************************************************************/
#endif /* AB_RTCMC_32768_EOZ9_H_ */
