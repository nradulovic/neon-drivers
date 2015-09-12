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
#include "mcu/config.h"
#include "neon-eds/include/neon_eds.h"

/*===============================================================  MACRO's  ==*/
/*-------------------------------------------------------  C++ extern base  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

enum nrtc_time_state
{
    NRTC_TIME_SET                = (0x1u << 0),
    NRTC_TIME_VALID              = (0x1u << 1),
    NRTC_TIME_SET_AND_VALID      = NRTC_TIME_SET | NRTC_TIME_VALID
};



enum nrtc_device_state
{
    NRTC_DEVICE_OK,
    NRTC_DEVICE_INACTIVE,
    NRTC_DEVICE_FAILURE,
    NRTC_DEVICE_NO_COMM
};



enum nrtc_reading_mode
{
	RTC_AUTO_READING,
	RTC_MANUAL_READING
};



enum rtc_event_id
{
	EVT_RTC_OPEN = 				CONFIG_RTC_EVENT_BASE_ID,
	EVT_RTC_SET_TIME,
	EVT_RTC_GET_TIME,
	EVT_RTC_TICK,
	EVT_RTC_TIME,
};



struct nrtc_config
{
	enum nrtc_reading_mode		reading_mode;
	void * 						custom;
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



struct rtc_open_event
{
	nevent   					event;
	struct nrtc_config *		config;
	struct nepa	*				ni2c_epa;
};



struct rtc_set_time_event
{
	nevent   					event;
	struct nrtc_time *			time;
};



struct rtc_time_event
{
	nevent   					event;
	struct nrtc_time *			time;
	struct nrtc_state *			state;
};

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/

struct nrtc_time * nrtc_get_time_immediately(void);



struct nrtc_state * nrtc_get_state_immediately(void);

/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of rtc_class.h
 ******************************************************************************/
#endif /* RTC_CLASS_H_ */
