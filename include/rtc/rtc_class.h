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
 * @brief       RTC Class
 * @defgroup    RTC_intf RTC Class
 * @brief       RTC Class
 *********************************************************************//** @{ */

#ifndef RTC_CLASS_H_
#define RTC_CLASS_H_

/*=========================================================  INCLUDE FILES  ==*/

#include <stdint.h>
#include "base/error.h"

/*===============================================================  MACRO's  ==*/
/*-------------------------------------------------------  C++ extern base  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

enum nrtc_time_state
{
    RTC_TIME_SET                = (0x1u << 0),
    RTC_TIME_VALID              = (0x1u << 1),
    RTC_TIME_SET_AND_VALID      = RTC_TIME_SET | RTC_TIME_VALID
};



enum nrtc_device_state
{
    RTC_DEVICE_OK,
    RTC_DEVICE_INACTIVE,
    RTC_DEVICE_FAILURE,
    RTC_DEVICE_NO_COMM
};



struct nrtc_config
{
	uint32_t					rtc_id;
	bool						notification_on;
	struct nrtc_custom_config * custom;
};



struct nrtc_time
{
    uint16_t                    year;
    uint8_t                     month;
    uint8_t                     day;
    uint8_t                     hour;
    uint8_t                     minute;
    uint8_t                     second;
};



struct nrtc_state
{
    enum nrtc_time_state        time;
    nerror                      device;
};



struct nrtc_class
{
    void                     (* init)(struct nrtc_config *);
    void                     (* term)(void);
    void                     (* set_time)(const struct nrtc_time *);
    enum nrtc_time_state     (* get_time)(struct nrtc_time *);
    void                     (* tick_isr)(void);
    const struct nrtc_state  (* get_state)(void);

};

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/
/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of rtc_class.h
 ******************************************************************************/
#endif /* RTC_CLASS_H_ */
