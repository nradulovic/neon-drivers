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

#include "lib/string/num_conv.h"
#include <string.h>

#include "port/compiler.h"
#include "base/error.h"
#include "mcu/i2c_bus.h"
#include "rtc/ab_rtcmc_32768_eoz9.h"
#include "rtc/rtc_class.h"

/*=========================================================  LOCAL MACRO's  ==*/

/* TODO: Move to configuration file
 */
#define NCONFIG_RTC_EPA_PRIO            1

#define CONFIG_DEFAULT_RTC_YEAR         2015
#define CONFIG_DEFAULT_RTC_MONTH        1
#define CONFIG_DEFAULT_RTC_DAY          1
#define CONFIG_DEFAULT_RTC_HOUR         0
#define CONFIG_DEFAULT_RTC_MINUTE       0
#define CONFIG_DEFAULT_RTC_SECOND       0

#define REG_CONTROL_1                   0x00
#define REG_CONTROL_INT                 0x01
#define REG_CONTROL_INT_FLAG            0x02
#define REG_CONTROL_STATUS              0x03
#define REG_CONTROL_RESET               0x04

#define REG_SECONDS                     0x08
#define REG_MINUTES                     0x09
#define REG_HOURS                       0x0a
#define REG_DAYS                        0x0b
#define REG_WEEKDAYS                    0x0c
#define REG_MONTHS                      0x0d
#define REG_YEARS                       0x0e

#define REG_ALARM_SECONDS               0x10
#define REG_ALARM_MINUTE                0x11
#define REG_ALARM_HOUR                  0x12
#define REG_ALARM_DAYS                  0x13
#define REG_ALARM_WEEKDAYS              0x14
#define REG_ALARM_MONTHS                0x15
#define REG_ALARM_YEAR                  0x16

#define REG_TIMER_LOW                   0x18
#define REG_TIMER_HIGH                  0x19

#define REG_TEMPERATURE                 0x20

#define REG_EEPROM_CONTROL				0x30

#define CONTROL_1_WE                    (0x1u << 0)
#define CONTROL_1_TE                    (0x1u << 1)
#define CONTROL_1_TAR                   (0x1u << 2)
#define CONTROL_1_EERE                  (0x1u << 3)
#define CONTROL_1_SRON                  (0x1u << 4)
#define CONTROL_1_TD0                   (0x1u << 5)
#define CONTROL_1_TD1                   (0x1u << 6)
#define CONTROL_1_CLK_INT               (0x1u << 7)

#define CONTROL_INT_AIE                 (0x1u << 0)
#define CONTROL_INT_TIE                 (0x1u << 1)
#define CONTROL_INT_V1IE                (0x1u << 2)
#define CONTROL_INT_V2IE                (0x1u << 3)
#define CONTROL_INT_SRIE                (0x1u << 4)

#define CONTROL_INT_FLAG_AF             (0x1u << 0)
#define CONTROL_INT_FLAG_TF             (0x1u << 1)
#define CONTROL_INT_FLAG_V1IF           (0x1u << 2)
#define CONTROL_INT_FLAG_V2IF           (0x1u << 3)
#define CONTROL_INT_FLAG_SRF            (0x1u << 4)

#define CONTROL_STATUS_V1F              (0x1u << 2)
#define CONTROL_STATUS_V2F              (0x1u << 3)
#define CONTROL_STATUS_SR               (0x1u << 4)
#define CONTROL_STATUS_PON              ((uint8_t)0x1u << 5u)
#define CONTROL_STATUS_EEBUSY           (0x1u << 7)

#define CONTROL_RESET_SYSR              (0x1u << 4)

#define EEPROM_CONTROL_FD0				(0x1u << 2)
#define EEPROM_CONTROL_FD1				(0x1u << 3)

#define HOURS_12_24_MODE                (0x1u << 6)
#define HOURS_AM_PM                     (0x1u << 5)

#define RTC_SLAVE_ADDRESS               0xac
#define RTC_READ_CMD                    (RTC_SLAVE_ADDRESS |  0x1u)
#define RTC_WRITE_CMD                   (RTC_SLAVE_ADDRESS & ~0x1u)

/*======================================================  LOCAL DATA TYPES  ==*/

struct context
{
    struct ni2c_slave           i2c_slave;
    struct nrtc_time            time;
    struct nrtc_state           state;
};



struct PORT_C_PACKED ab_rtcmc_time_registers
{
    uint8_t                     seconds;
    uint8_t                     minutes;
    uint8_t                     hours;
    uint8_t                     days;
    uint8_t                     weekdays;
    uint8_t                     months;
    uint8_t                     years;
};



struct rtc_workspace
{
	struct PORT_C_PACKED ab_rtcmc_time_registers 	regs;
	struct nepa	*									client;
	struct nepa	*									ni2c_epa;
	uint8_t											reg;
	struct nrtc_config *							config;
	struct ab_nrtc_custom_config *					custom_config;
};

/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/

static naction state_open(
		struct nsm * sm,
		const struct nevent * event);



static naction state_read_control_status(
		struct nsm * sm,
		const struct nevent * event);



static naction state_clear_pon(
		struct nsm * sm,
		const struct nevent * event);



static naction state_set_default_time(
		struct nsm * sm,
		const struct nevent * event);



static naction state_clear_control_int_reg(
		struct nsm * sm,
		const struct nevent * event);



static naction state_watch_enable(
		struct nsm * sm,
		const struct nevent * event);



static naction state_write_timer_low(
		struct nsm * sm,
		const struct nevent * event);



static naction state_write_timer_high(
		struct nsm * sm,
		const struct nevent * event);



static naction state_enable_autoreload_mode(
		struct nsm * sm,
		const struct nevent * event);


static naction state_select_clkout_freq(
		struct nsm * sm,
		const struct nevent * event);


static naction state_enable_timer(
		struct nsm * sm,
		const struct nevent * event);



static naction state_clear_control_int_flag_reg(
		struct nsm * sm,
		const struct nevent * event);



static naction state_enable_timer_int(
		struct nsm * sm,
		const struct nevent * event);



static naction state_tick(
		struct nsm * sm,
		const struct nevent * event);



static naction state_set_time(
		struct nsm * sm,
		const struct nevent * event);



static naction state_read_time(
		struct nsm * sm,
		const struct nevent * event);

/*=======================================================  LOCAL VARIABLES  ==*/

static struct context           g_context;
static struct nevent * 		  	g_rtc_queue_storage[CONFIG_RTC_QUEUE_SIZE];
static struct rtc_workspace  	g_rtc_workspace;

/*======================================================  GLOBAL VARIABLES  ==*/

struct nepa 					g_ab_rtc_epa;
const struct nepa_define		g_ab_rtc_define =
{
    NSM_DEF_INIT(&g_rtc_workspace, &state_open, NSM_TYPE_FSM),
    NEQUEUE_DEF_INIT(g_rtc_queue_storage, sizeof(g_rtc_queue_storage)),
    NTHREAD_DEF_INIT("rtc", NCONFIG_RTC_EPA_PRIO),
};

/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/

static naction state_open(
		struct nsm * sm,
		const struct nevent * event)
{
	struct rtc_workspace * ws = (struct rtc_workspace *)sm;

	switch (event->id) {
		case EVT_RTC_OPEN: {
			struct rtc_open_event *		rtc_open;
			struct ni2c_open_event	* 	i2c_open;

			rtc_open = (struct rtc_open_event *)event;

			i2c_open = (struct ni2c_open_event *)nevent_create(
					sizeof(struct ni2c_open_event), EVT_NI2C_OPEN_SLAVE);

			i2c_open->slave = &g_context.i2c_slave;
			i2c_open->address = RTC_SLAVE_ADDRESS;
			i2c_open->flags = NI2C_READ_WITH_RESTART;
			ws->client = event->producer;
			ws->ni2c_epa = rtc_open->ni2c_epa;
			ws->config = rtc_open->config;
			ws->custom_config = (struct ab_nrtc_custom_config *)rtc_open->config->custom;

			nepa_send_event(rtc_open->ni2c_epa, (nevent *)i2c_open);

			return (naction_transit_to(sm, state_read_control_status));
		}
		default: {
			return (naction_ignored());
		}
	}
}



static naction state_read_control_status(
	struct nsm * sm,
	const struct nevent * event)
{
	struct rtc_workspace * ws = (struct rtc_workspace *)sm;

	switch (event->id) {
		case NSM_ENTRY: {
			struct ni2c_transfer_event	* 	i2c_transfer;

			i2c_transfer = (struct ni2c_transfer_event *)nevent_create(
					sizeof(struct ni2c_transfer_event),
					EVT_NI2C_READ_SLAVE);

			i2c_transfer->slave = &g_context.i2c_slave;
			i2c_transfer->reg = REG_CONTROL_STATUS;
			i2c_transfer->data = &ws->reg;
			i2c_transfer->size = 1u;

			nepa_send_event(ws->ni2c_epa, (nevent *)i2c_transfer);

			return (naction_handled());
		}
		case EVT_NI2C_TRANSFER_COMPLETED: {
			if (ws->reg & CONTROL_STATUS_PON) {

				return (naction_transit_to(sm, state_clear_pon));
			} else {
				g_context.state.time |=  NRTC_TIME_SET;

				return (naction_transit_to(sm, state_clear_control_int_reg));
			}
		}
		case EVT_NI2C_ERROR: {
		    g_context.state.time  &= ~NRTC_TIME_VALID;
		    g_context.state.device = NRTC_DEVICE_NO_COMM;
		    g_context.time.year    = CONFIG_DEFAULT_RTC_YEAR;
		    g_context.time.month   = CONFIG_DEFAULT_RTC_MONTH;
		    g_context.time.day     = CONFIG_DEFAULT_RTC_DAY;
		    g_context.time.hour    = CONFIG_DEFAULT_RTC_HOUR;
		    g_context.time.minute  = CONFIG_DEFAULT_RTC_MINUTE;
		    g_context.time.second  = CONFIG_DEFAULT_RTC_SECOND;

		    return (naction_transit_to(sm, state_tick));
		}
		default: {
			return (naction_ignored());
		}
	}
}



static naction state_clear_pon(
	struct nsm * sm,
	const struct nevent * event)
{
	struct rtc_workspace * ws = (struct rtc_workspace *)sm;

	switch (event->id) {
		case NSM_ENTRY: {
			struct ni2c_transfer_event	* 	i2c_transfer;

			ws->reg &= (uint8_t)~CONTROL_STATUS_PON;

			i2c_transfer = (struct ni2c_transfer_event *)nevent_create(
					sizeof(struct ni2c_transfer_event),
					EVT_NI2C_WRITE_SLAVE);

			i2c_transfer->slave = &g_context.i2c_slave;
			i2c_transfer->reg = REG_CONTROL_STATUS;
			i2c_transfer->data = &ws->reg;
			i2c_transfer->size = 1u;

			nepa_send_event(ws->ni2c_epa, (nevent *)i2c_transfer);

			return (naction_handled());
		}
		case EVT_NI2C_TRANSFER_COMPLETED: {

			return (naction_transit_to(sm, state_set_default_time));
		}
		case EVT_NI2C_ERROR: {
		    g_context.state.time  &= ~NRTC_TIME_VALID;
		    g_context.state.device = NRTC_DEVICE_NO_COMM;
		    g_context.time.year    = CONFIG_DEFAULT_RTC_YEAR;
		    g_context.time.month   = CONFIG_DEFAULT_RTC_MONTH;
		    g_context.time.day     = CONFIG_DEFAULT_RTC_DAY;
		    g_context.time.hour    = CONFIG_DEFAULT_RTC_HOUR;
		    g_context.time.minute  = CONFIG_DEFAULT_RTC_MINUTE;
		    g_context.time.second  = CONFIG_DEFAULT_RTC_SECOND;

		    return (naction_transit_to(sm, state_tick));
		}
		default: {
			return (naction_ignored());
		}
	}
}



static naction state_set_default_time(
	struct nsm * sm,
	const struct nevent * event)
{
	struct rtc_workspace * ws = (struct rtc_workspace *)sm;

	switch (event->id) {
		case NSM_ENTRY: {
			struct ni2c_transfer_event	* 					i2c_transfer;
			struct nrtc_time 								time;

			time.year   = CONFIG_DEFAULT_RTC_YEAR;
			time.month  = CONFIG_DEFAULT_RTC_MONTH;
			time.day    = CONFIG_DEFAULT_RTC_DAY;
			time.hour   = CONFIG_DEFAULT_RTC_HOUR;
			time.minute = CONFIG_DEFAULT_RTC_MINUTE;
			time.second = CONFIG_DEFAULT_RTC_SECOND;

		    ws->regs.years   = bin_to_bcd((uint8_t)(time.year - 2000u));
		    ws->regs.months  = bin_to_bcd(time.month);
		    ws->regs.days    = bin_to_bcd(time.day);
		    ws->regs.hours   = bin_to_bcd(time.hour);
		    ws->regs.minutes = bin_to_bcd(time.minute);
		    ws->regs.seconds = bin_to_bcd(time.second);

			i2c_transfer = (struct ni2c_transfer_event *)nevent_create(
					sizeof(struct ni2c_transfer_event),
					EVT_NI2C_WRITE_SLAVE);

			i2c_transfer->slave = &g_context.i2c_slave;
			i2c_transfer->reg = REG_SECONDS;
			i2c_transfer->data = &ws->regs;
			i2c_transfer->size = sizeof(ws->regs);

			nepa_send_event(ws->ni2c_epa, (nevent *)i2c_transfer);

		    memcpy(&g_context.time, &time, sizeof(g_context.time));
		    g_context.state.time  |= NRTC_TIME_SET_AND_VALID;
		    g_context.state.device = NRTC_DEVICE_OK;
			g_context.state.time &= ~NRTC_TIME_SET;

			return (naction_handled());
		}
		case EVT_NI2C_TRANSFER_COMPLETED: {

			return (naction_transit_to(sm, state_clear_control_int_reg));
		}
		case EVT_NI2C_ERROR: {
		    g_context.state.time  &= ~NRTC_TIME_VALID;
		    g_context.state.device = NRTC_DEVICE_NO_COMM;
		    g_context.time.year    = CONFIG_DEFAULT_RTC_YEAR;
		    g_context.time.month   = CONFIG_DEFAULT_RTC_MONTH;
		    g_context.time.day     = CONFIG_DEFAULT_RTC_DAY;
		    g_context.time.hour    = CONFIG_DEFAULT_RTC_HOUR;
		    g_context.time.minute  = CONFIG_DEFAULT_RTC_MINUTE;
		    g_context.time.second  = CONFIG_DEFAULT_RTC_SECOND;

		    return (naction_transit_to(sm, state_tick));
		}
		default: {
			return (naction_ignored());
		}
	}
}



static naction state_clear_control_int_reg(
	struct nsm * sm,
	const struct nevent * event)
{
	struct rtc_workspace * ws = (struct rtc_workspace *)sm;

	switch (event->id) {
		case NSM_ENTRY: {
			struct ni2c_transfer_event	* 	i2c_transfer;

			ws->reg = 0u;

			i2c_transfer = (struct ni2c_transfer_event *)nevent_create(
					sizeof(struct ni2c_transfer_event),
					EVT_NI2C_WRITE_SLAVE);

			i2c_transfer->slave = &g_context.i2c_slave;
			i2c_transfer->reg = REG_CONTROL_INT;
			i2c_transfer->data = &ws->reg;
			i2c_transfer->size = 1u;

			nepa_send_event(ws->ni2c_epa, (nevent *)i2c_transfer);

			return (naction_handled());
		}
		case EVT_NI2C_TRANSFER_COMPLETED: {

			return (naction_transit_to(sm, state_watch_enable));
		}
		case EVT_NI2C_ERROR: {
		    g_context.state.time  &= ~NRTC_TIME_VALID;
		    g_context.state.device = NRTC_DEVICE_NO_COMM;
		    g_context.time.year    = CONFIG_DEFAULT_RTC_YEAR;
		    g_context.time.month   = CONFIG_DEFAULT_RTC_MONTH;
		    g_context.time.day     = CONFIG_DEFAULT_RTC_DAY;
		    g_context.time.hour    = CONFIG_DEFAULT_RTC_HOUR;
		    g_context.time.minute  = CONFIG_DEFAULT_RTC_MINUTE;
		    g_context.time.second  = CONFIG_DEFAULT_RTC_SECOND;

		    return (naction_transit_to(sm, state_tick));
		}
		default: {
			return (naction_ignored());
		}
	}
}



static naction state_watch_enable(
	struct nsm * sm,
	const struct nevent * event)
{
	struct rtc_workspace * ws = (struct rtc_workspace *)sm;

	switch (event->id) {
		case NSM_ENTRY: {
			struct ni2c_transfer_event	* 	i2c_transfer;

			ws->reg = CONTROL_1_WE;

			i2c_transfer = (struct ni2c_transfer_event *)nevent_create(
					sizeof(struct ni2c_transfer_event),
					EVT_NI2C_WRITE_SLAVE);

			i2c_transfer->slave = &g_context.i2c_slave;
			i2c_transfer->reg = REG_CONTROL_1;
			i2c_transfer->data = &ws->reg;
			i2c_transfer->size = 1u;

			nepa_send_event(ws->ni2c_epa, (nevent *)i2c_transfer);

			return (naction_handled());
		}
		case EVT_NI2C_TRANSFER_COMPLETED: {

			return (naction_transit_to(sm, state_write_timer_low));
		}
		case EVT_NI2C_ERROR: {
		    g_context.state.time  &= ~NRTC_TIME_VALID;
		    g_context.state.device = NRTC_DEVICE_NO_COMM;
		    g_context.time.year    = CONFIG_DEFAULT_RTC_YEAR;
		    g_context.time.month   = CONFIG_DEFAULT_RTC_MONTH;
		    g_context.time.day     = CONFIG_DEFAULT_RTC_DAY;
		    g_context.time.hour    = CONFIG_DEFAULT_RTC_HOUR;
		    g_context.time.minute  = CONFIG_DEFAULT_RTC_MINUTE;
		    g_context.time.second  = CONFIG_DEFAULT_RTC_SECOND;

		    return (naction_transit_to(sm, state_tick));
		}
		default: {
			return (naction_ignored());
		}
	}
}



static naction state_write_timer_low(
	struct nsm * sm,
	const struct nevent * event)
{
	struct rtc_workspace * ws = (struct rtc_workspace *)sm;

	switch (event->id) {
		case NSM_ENTRY: {
			struct ni2c_transfer_event	* 	i2c_transfer;

			ws->reg = 32u;

			i2c_transfer = (struct ni2c_transfer_event *)nevent_create(
					sizeof(struct ni2c_transfer_event),
					EVT_NI2C_WRITE_SLAVE);

			i2c_transfer->slave = &g_context.i2c_slave;
			i2c_transfer->reg = REG_TIMER_LOW;
			i2c_transfer->data = &ws->reg;
			i2c_transfer->size = 1u;

			nepa_send_event(ws->ni2c_epa, (nevent *)i2c_transfer);

			return (naction_handled());
		}
		case EVT_NI2C_TRANSFER_COMPLETED: {

			return (naction_transit_to(sm, state_write_timer_high));
		}
		case EVT_NI2C_ERROR: {
		    g_context.state.time  &= ~NRTC_TIME_VALID;
		    g_context.state.device = NRTC_DEVICE_NO_COMM;
		    g_context.time.year    = CONFIG_DEFAULT_RTC_YEAR;
		    g_context.time.month   = CONFIG_DEFAULT_RTC_MONTH;
		    g_context.time.day     = CONFIG_DEFAULT_RTC_DAY;
		    g_context.time.hour    = CONFIG_DEFAULT_RTC_HOUR;
		    g_context.time.minute  = CONFIG_DEFAULT_RTC_MINUTE;
		    g_context.time.second  = CONFIG_DEFAULT_RTC_SECOND;

		    return (naction_transit_to(sm, state_tick));
		}
		default: {
			return (naction_ignored());
		}
	}
}



static naction state_write_timer_high(
	struct nsm * sm,
	const struct nevent * event)
{
	struct rtc_workspace * ws = (struct rtc_workspace *)sm;

	switch (event->id) {
		case NSM_ENTRY: {
			struct ni2c_transfer_event	* 	i2c_transfer;

			ws->reg = 0u;

			i2c_transfer = (struct ni2c_transfer_event *)nevent_create(
					sizeof(struct ni2c_transfer_event),
					EVT_NI2C_WRITE_SLAVE);

			i2c_transfer->slave = &g_context.i2c_slave;
			i2c_transfer->reg = REG_TIMER_HIGH;
			i2c_transfer->data = &ws->reg;
			i2c_transfer->size = 1u;

			nepa_send_event(ws->ni2c_epa, (nevent *)i2c_transfer);

			return (naction_handled());
		}
		case EVT_NI2C_TRANSFER_COMPLETED: {

			return (naction_transit_to(sm, state_enable_autoreload_mode));
		}
		case EVT_NI2C_ERROR: {
		    g_context.state.time  &= ~NRTC_TIME_VALID;
		    g_context.state.device = NRTC_DEVICE_NO_COMM;
		    g_context.time.year    = CONFIG_DEFAULT_RTC_YEAR;
		    g_context.time.month   = CONFIG_DEFAULT_RTC_MONTH;
		    g_context.time.day     = CONFIG_DEFAULT_RTC_DAY;
		    g_context.time.hour    = CONFIG_DEFAULT_RTC_HOUR;
		    g_context.time.minute  = CONFIG_DEFAULT_RTC_MINUTE;
		    g_context.time.second  = CONFIG_DEFAULT_RTC_SECOND;

		    return (naction_transit_to(sm, state_tick));
		}
		default: {
			return (naction_ignored());
		}
	}
}



static naction state_enable_autoreload_mode(
	struct nsm * sm,
	const struct nevent * event)
{
	struct rtc_workspace * ws = (struct rtc_workspace *)sm;

	switch (event->id) {
		case NSM_ENTRY: {
			struct ni2c_transfer_event	* 	i2c_transfer;

			ws->reg = CONTROL_1_TAR | CONTROL_1_WE;

			i2c_transfer = (struct ni2c_transfer_event *)nevent_create(
					sizeof(struct ni2c_transfer_event),
					EVT_NI2C_WRITE_SLAVE);

			i2c_transfer->slave = &g_context.i2c_slave;
			i2c_transfer->reg = REG_CONTROL_1;
			i2c_transfer->data = &ws->reg;
			i2c_transfer->size = 1u;

			nepa_send_event(ws->ni2c_epa, (nevent *)i2c_transfer);

			return (naction_handled());
		}
		case EVT_NI2C_TRANSFER_COMPLETED: {

			//return (naction_transit_to(sm, state_enable_timer));
			return (naction_transit_to(sm, state_select_clkout_freq));
		}
		case EVT_NI2C_ERROR: {
		    g_context.state.time  &= ~NRTC_TIME_VALID;
		    g_context.state.device = NRTC_DEVICE_NO_COMM;
		    g_context.time.year    = CONFIG_DEFAULT_RTC_YEAR;
		    g_context.time.month   = CONFIG_DEFAULT_RTC_MONTH;
		    g_context.time.day     = CONFIG_DEFAULT_RTC_DAY;
		    g_context.time.hour    = CONFIG_DEFAULT_RTC_HOUR;
		    g_context.time.minute  = CONFIG_DEFAULT_RTC_MINUTE;
		    g_context.time.second  = CONFIG_DEFAULT_RTC_SECOND;

		    return (naction_transit_to(sm, state_tick));
		}
		default: {
			return (naction_ignored());
		}
	}
}


static naction state_select_clkout_freq(
		struct nsm * sm,
		const struct nevent * event)
{
	struct rtc_workspace * ws = (struct rtc_workspace *)sm;

	switch (event->id) {
		case NSM_ENTRY: {
			struct ni2c_transfer_event	* 	i2c_transfer;

			ws->reg = ws->custom_config->config & AB_RTC_CLOCKOUT_FREQ;

			i2c_transfer = (struct ni2c_transfer_event *)nevent_create(
					sizeof(struct ni2c_transfer_event),
					EVT_NI2C_WRITE_SLAVE);

			i2c_transfer->slave = &g_context.i2c_slave;
			i2c_transfer->reg = REG_EEPROM_CONTROL;
			i2c_transfer->data = &ws->reg;
			i2c_transfer->size = 1u;

			nepa_send_event(ws->ni2c_epa, (nevent *)i2c_transfer);

			return (naction_handled());
		}
		case EVT_NI2C_TRANSFER_COMPLETED: {

			return (naction_transit_to(sm, state_enable_timer));
		}
		case EVT_NI2C_ERROR: {
		    g_context.state.time  &= ~NRTC_TIME_VALID;
		    g_context.state.device = NRTC_DEVICE_NO_COMM;
		    g_context.time.year    = CONFIG_DEFAULT_RTC_YEAR;
		    g_context.time.month   = CONFIG_DEFAULT_RTC_MONTH;
		    g_context.time.day     = CONFIG_DEFAULT_RTC_DAY;
		    g_context.time.hour    = CONFIG_DEFAULT_RTC_HOUR;
		    g_context.time.minute  = CONFIG_DEFAULT_RTC_MINUTE;
		    g_context.time.second  = CONFIG_DEFAULT_RTC_SECOND;

		    return (naction_transit_to(sm, state_tick));
		}
		default: {
			return (naction_ignored());
		}
	}
}



static naction state_enable_timer(
	struct nsm * sm,
	const struct nevent * event)
{
	struct rtc_workspace * ws = (struct rtc_workspace *)sm;

	switch (event->id) {
		case NSM_ENTRY: {
			struct ni2c_transfer_event	* 	i2c_transfer;

			ws->reg = CONTROL_1_TE;

			i2c_transfer = (struct ni2c_transfer_event *)nevent_create(
					sizeof(struct ni2c_transfer_event),
					EVT_NI2C_WRITE_SLAVE);

			i2c_transfer->slave = &g_context.i2c_slave;
			i2c_transfer->reg = REG_CONTROL_1;
			i2c_transfer->data = &ws->reg;
			i2c_transfer->size = 1u;

			nepa_send_event(ws->ni2c_epa, (nevent *)i2c_transfer);

			return (naction_handled());
		}
		case EVT_NI2C_TRANSFER_COMPLETED: {

			return (naction_transit_to(sm, state_clear_control_int_flag_reg));
		}
		case EVT_NI2C_ERROR: {
		    g_context.state.time  &= ~NRTC_TIME_VALID;
		    g_context.state.device = NRTC_DEVICE_NO_COMM;
		    g_context.time.year    = CONFIG_DEFAULT_RTC_YEAR;
		    g_context.time.month   = CONFIG_DEFAULT_RTC_MONTH;
		    g_context.time.day     = CONFIG_DEFAULT_RTC_DAY;
		    g_context.time.hour    = CONFIG_DEFAULT_RTC_HOUR;
		    g_context.time.minute  = CONFIG_DEFAULT_RTC_MINUTE;
		    g_context.time.second  = CONFIG_DEFAULT_RTC_SECOND;

		    return (naction_transit_to(sm, state_tick));
		}
		default: {
			return (naction_ignored());
		}
	}
}



static naction state_clear_control_int_flag_reg(
	struct nsm * sm,
	const struct nevent * event)
{
	struct rtc_workspace * ws = (struct rtc_workspace *)sm;

	switch (event->id) {
		case NSM_ENTRY: {
			struct ni2c_transfer_event	* 	i2c_transfer;

			ws->reg = 0u;

			i2c_transfer = (struct ni2c_transfer_event *)nevent_create(
					sizeof(struct ni2c_transfer_event),
					EVT_NI2C_WRITE_SLAVE);

			i2c_transfer->slave = &g_context.i2c_slave;
			i2c_transfer->reg = REG_CONTROL_INT_FLAG;
			i2c_transfer->data = &ws->reg;
			i2c_transfer->size = 1u;

			nepa_send_event(ws->ni2c_epa, (nevent *)i2c_transfer);

			return (naction_handled());
		}
		case EVT_NI2C_TRANSFER_COMPLETED: {

			return (naction_transit_to(sm, state_enable_timer_int));
		}
		case EVT_NI2C_ERROR: {
		    g_context.state.time  &= ~NRTC_TIME_VALID;
		    g_context.state.device = NRTC_DEVICE_NO_COMM;
		    g_context.time.year    = CONFIG_DEFAULT_RTC_YEAR;
		    g_context.time.month   = CONFIG_DEFAULT_RTC_MONTH;
		    g_context.time.day     = CONFIG_DEFAULT_RTC_DAY;
		    g_context.time.hour    = CONFIG_DEFAULT_RTC_HOUR;
		    g_context.time.minute  = CONFIG_DEFAULT_RTC_MINUTE;
		    g_context.time.second  = CONFIG_DEFAULT_RTC_SECOND;

		    return (naction_transit_to(sm, state_tick));
		}
		default: {
			return (naction_ignored());
		}
	}
}



static naction state_enable_timer_int(
	struct nsm * sm,
	const struct nevent * event)
{
	struct rtc_workspace * ws = (struct rtc_workspace *)sm;

	switch (event->id) {
		case NSM_ENTRY: {
			struct ni2c_transfer_event	* 	i2c_transfer;

			ws->reg = CONTROL_INT_TIE;

			i2c_transfer = (struct ni2c_transfer_event *)nevent_create(
					sizeof(struct ni2c_transfer_event),
					EVT_NI2C_WRITE_SLAVE);

			i2c_transfer->slave = &g_context.i2c_slave;
			i2c_transfer->reg = REG_CONTROL_INT;
			i2c_transfer->data = &ws->reg;
			i2c_transfer->size = 1u;

			nepa_send_event(ws->ni2c_epa, (nevent *)i2c_transfer);

			return (naction_handled());
		}
		case EVT_NI2C_TRANSFER_COMPLETED: {

			return (naction_transit_to(sm, state_tick));
		}
		case EVT_NI2C_ERROR: {
		    g_context.state.time  &= ~NRTC_TIME_VALID;
		    g_context.state.device = NRTC_DEVICE_NO_COMM;
		    g_context.time.year    = CONFIG_DEFAULT_RTC_YEAR;
		    g_context.time.month   = CONFIG_DEFAULT_RTC_MONTH;
		    g_context.time.day     = CONFIG_DEFAULT_RTC_DAY;
		    g_context.time.hour    = CONFIG_DEFAULT_RTC_HOUR;
		    g_context.time.minute  = CONFIG_DEFAULT_RTC_MINUTE;
		    g_context.time.second  = CONFIG_DEFAULT_RTC_SECOND;

		    return (naction_transit_to(sm, state_tick));
		}
		default: {
			return (naction_ignored());
		}
	}
}



static naction state_tick(
		struct nsm * sm,
		const struct nevent * event)
{
	struct rtc_workspace * ws = (struct rtc_workspace *)sm;

	switch (event->id) {
		case EVT_RTC_TICK: {
			struct ni2c_transfer_event	* 	i2c_transfer;

			ws->reg = 0u;

			i2c_transfer = (struct ni2c_transfer_event *)nevent_create(
					sizeof(struct ni2c_transfer_event),
					EVT_NI2C_WRITE_SLAVE);

			i2c_transfer->slave = &g_context.i2c_slave;
			i2c_transfer->reg = REG_CONTROL_INT_FLAG;
			i2c_transfer->data = &ws->reg;
			i2c_transfer->size = 1u;

			nepa_send_event(ws->ni2c_epa, (nevent *)i2c_transfer);

			return (naction_handled());
		}
		case EVT_NI2C_TRANSFER_COMPLETED: {

			return (naction_transit_to(sm, state_read_time));
		}
		case EVT_NI2C_ERROR: {
			struct rtc_time_event * get_time;

			get_time = (struct rtc_time_event *)nevent_create(
					sizeof(struct rtc_time_event),
					EVT_RTC_TIME);

			get_time->time = nrtc_get_time_immediately();
			get_time->state = nrtc_get_state_immediately();
		    g_context.state.time  &= ~NRTC_TIME_VALID;
		    g_context.state.device = NRTC_DEVICE_NO_COMM;

			nepa_send_event(ws->client, (nevent *)get_time);

		    return (naction_handled());
		}
		case EVT_RTC_SET_TIME: {

			return (naction_super(sm, state_set_time));
		}
		case EVT_RTC_GET_TIME: {
			if (ws->config->reading_mode == RTC_AUTO_READING) {
				struct rtc_time_event * get_time;

				ws->client = event->producer;

				get_time = (struct rtc_time_event *)nevent_create(
						sizeof(struct rtc_time_event),
						EVT_RTC_TIME);

				get_time->time = nrtc_get_time_immediately();
				get_time->state = nrtc_get_state_immediately();

				nepa_send_event(ws->client, (nevent *)get_time);

				return (naction_handled());
			} else {
				return (naction_transit_to(sm, state_read_time));
			}
		}
		default: {
			return (naction_ignored());
		}
	}
}



static naction state_set_time(
		struct nsm * sm,
		const struct nevent * event)
{
	struct rtc_workspace * ws = (struct rtc_workspace *)sm;

	switch (event->id) {
		case NSM_ENTRY: {
			struct rtc_set_time_event *						set_time;
			struct ni2c_transfer_event	* 					i2c_transfer;

			set_time = (struct rtc_set_time_event *)event;

			ws->regs.years   = bin_to_bcd((uint8_t)(set_time->time->year - 2000u));
			ws->regs.months  = bin_to_bcd(set_time->time->month);
			ws->regs.days    = bin_to_bcd(set_time->time->day);
			ws->regs.hours   = bin_to_bcd(set_time->time->hour);
			ws->regs.minutes = bin_to_bcd(set_time->time->minute);
			ws->regs.seconds = bin_to_bcd(set_time->time->second);

			i2c_transfer = (struct ni2c_transfer_event *)nevent_create(
					sizeof(struct ni2c_transfer_event),
					EVT_NI2C_WRITE_SLAVE);

			i2c_transfer->slave = &g_context.i2c_slave;
			i2c_transfer->reg = REG_SECONDS;
			i2c_transfer->data = &ws->regs;
			i2c_transfer->size = sizeof(ws->regs);

			nepa_send_event(ws->ni2c_epa, (nevent *)i2c_transfer);

		    memcpy(&g_context.time, set_time->time, sizeof(g_context.time));
		    g_context.state.time  |= NRTC_TIME_SET_AND_VALID;
		    g_context.state.device = NRTC_DEVICE_OK;

			return (naction_handled());
		}
		case EVT_NI2C_TRANSFER_COMPLETED: {

			return (naction_transit_to(sm, state_tick));
		}
		case EVT_NI2C_ERROR: {
		    g_context.state.time  &= ~(NRTC_TIME_SET_AND_VALID);
		    g_context.state.device = NRTC_DEVICE_NO_COMM;

		    return (naction_transit_to(sm, state_tick));
		}
		default: {
			return (naction_ignored());
		}
	}
}



static naction state_read_time(
		struct nsm * sm,
		const struct nevent * event)
{
	struct rtc_workspace * ws = (struct rtc_workspace *)sm;

	switch (event->id) {
		case NSM_ENTRY: {
			struct ni2c_transfer_event	* 	i2c_transfer;

			i2c_transfer = (struct ni2c_transfer_event *)nevent_create(
					sizeof(struct ni2c_transfer_event),
					EVT_NI2C_READ_SLAVE);

			i2c_transfer->slave = &g_context.i2c_slave;
			i2c_transfer->reg = REG_SECONDS;
			i2c_transfer->data = &ws->regs;
			i2c_transfer->size = sizeof(ws->regs);

			nepa_send_event(ws->ni2c_epa, (nevent *)i2c_transfer);

			return (naction_handled());
		}
		case EVT_NI2C_TRANSFER_COMPLETED: {
			struct rtc_time_event * get_time;

		    g_context.state.time  |= NRTC_TIME_VALID;
		    g_context.state.device = NRTC_DEVICE_OK;
		    g_context.time.year    = (uint16_t)(bcd_to_bin(ws->regs.years) + 2000u);
		    g_context.time.month   = bcd_to_bin(ws->regs.months);
		    g_context.time.day     = bcd_to_bin(ws->regs.days);
		    g_context.time.hour    = bcd_to_bin(ws->regs.hours);
		    g_context.time.minute  = bcd_to_bin(ws->regs.minutes);
		    g_context.time.second  = bcd_to_bin(ws->regs.seconds);

			get_time = (struct rtc_time_event *)nevent_create(
					sizeof(struct rtc_time_event),
					EVT_RTC_TIME);

			get_time->time = nrtc_get_time_immediately();
			get_time->state = nrtc_get_state_immediately();
		    g_context.state.time  &= ~NRTC_TIME_VALID;
		    g_context.state.device = NRTC_DEVICE_NO_COMM;

			nepa_send_event(ws->client, (nevent *)get_time);

			return (naction_transit_to(sm, state_tick));
		}
		case EVT_NI2C_ERROR: {
			struct rtc_time_event * get_time;

			get_time = (struct rtc_time_event *)nevent_create(
					sizeof(struct rtc_time_event),
					EVT_RTC_TIME);

			get_time->time = nrtc_get_time_immediately();
			get_time->state = nrtc_get_state_immediately();
		    g_context.state.time  &= ~NRTC_TIME_VALID;
		    g_context.state.device = NRTC_DEVICE_NO_COMM;

			nepa_send_event(ws->client, (nevent *)get_time);

		    return (naction_transit_to(sm, state_tick));
		}
		default: {
			return (naction_ignored());
		}
	}
}


/*===========================================  GLOBAL FUNCTION DEFINITIONS  ==*/

struct nrtc_time * nrtc_get_time_immediately(void)
{
    return (&g_context.time);
}

struct nrtc_state * nrtc_get_state_immediately(void)
{
    return (&g_context.state);
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of ab_rtcmc_32768_eoz9.c
 ******************************************************************************/
