
#include "lib/string/num_conv.h"
#include <string.h>

#include "port/compiler.h"
#include "shared/error.h"
#include "mcu/i2c.h"
#include "rtc/ab_rtcmc_32768_eoz9.h"
#include "rtc/rtc_class.h"

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

#define HOURS_12_24_MODE                (0x1u << 6)
#define HOURS_AM_PM                     (0x1u << 5)

#define RTC_SLAVE_ADDRESS               0xac
#define RTC_READ_CMD                    (RTC_SLAVE_ADDRESS |  0x1u)
#define RTC_WRITE_CMD                   (RTC_SLAVE_ADDRESS & ~0x1u)

struct context
{
    struct i2c_slave            i2c_slave;
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

static struct context                g_context;

static const struct i2c_slave_config g_rtc_i2c_config =
{
    I2C_SLAVE_RD_NACK | I2C_SLAVE_RD_REPEAT,
    RTC_SLAVE_ADDRESS
};

void ab_rtcmc_init_driver(
    const void *                config)
{
    uint8_t                     reg;
    nerror                      error;

    g_context.state.time = 0;
    i2c_slave_open(&g_context.i2c_slave, &g_rtc_i2c_config,
        i2c_bus_from_id(((const struct ab_rtcmc_config *)config)->bus_id),
        ((const struct ab_rtcmc_config *)config)->device_id);
    error = i2c_slave_read(&g_context.i2c_slave, REG_CONTROL_STATUS, &reg, 1);

    if (error) {
        goto NO_COMM_FAILURE;
    }

    if (reg & CONTROL_STATUS_PON) {
        struct nrtc_time        new_time;

        reg &= ~CONTROL_STATUS_PON;

        error = i2c_slave_write(&g_context.i2c_slave, REG_CONTROL_STATUS, &reg,
            1);

        if (error) {
            goto NO_COMM_FAILURE;
        }
        new_time.year   = CONFIG_DEFAULT_RTC_YEAR;
        new_time.month  = CONFIG_DEFAULT_RTC_MONTH;
        new_time.day    = CONFIG_DEFAULT_RTC_DAY;
        new_time.hour   = CONFIG_DEFAULT_RTC_HOUR;
        new_time.minute = CONFIG_DEFAULT_RTC_MINUTE;
        new_time.second = CONFIG_DEFAULT_RTC_SECOND;
        ab_rtcmc_set_time(&new_time);
        g_context.state.time &= ~RTC_TIME_SET;
    } else {
        g_context.state.time |=  RTC_TIME_SET;
    }
    reg   = 0;
    error = i2c_slave_write(&g_context.i2c_slave, REG_CONTROL_INT, &reg, 1);

    if (error) {
        goto NO_COMM_FAILURE;
    }
    reg   = CONTROL_1_WE;
    error = i2c_slave_write(&g_context.i2c_slave, REG_CONTROL_1, &reg, 1);

    if (error) {
        goto NO_COMM_FAILURE;
    }
    reg   = 32;
    error = i2c_slave_write(&g_context.i2c_slave, REG_TIMER_LOW, &reg, 1);

    if (error) {
        goto NO_COMM_FAILURE;
    }
    reg   = 0;
    error = i2c_slave_write(&g_context.i2c_slave, REG_TIMER_HIGH, &reg, 1);

    if (error) {
        goto NO_COMM_FAILURE;
    }
    reg   = CONTROL_1_TAR | CONTROL_1_WE;
    error = i2c_slave_write(&g_context.i2c_slave, REG_CONTROL_1, &reg, 1);

    if (error) {
        goto NO_COMM_FAILURE;
    }
    reg  |= CONTROL_1_TE;
    error = i2c_slave_write(&g_context.i2c_slave, REG_CONTROL_1, &reg, 1);

    if (error) {
        goto NO_COMM_FAILURE;
    }
    reg   = 0;
    error = i2c_slave_write(&g_context.i2c_slave, REG_CONTROL_INT_FLAG, &reg, 0);

    if (error) {
        goto NO_COMM_FAILURE;
    }
    reg   = CONTROL_INT_TIE;
    error = i2c_slave_write(&g_context.i2c_slave, REG_CONTROL_INT, &reg, 0);
    
    if (error) {
        goto NO_COMM_FAILURE;
    }
    g_context.state.time  |= RTC_TIME_VALID;
    g_context.state.device = RTC_DEVICE_OK;

    return;
NO_COMM_FAILURE:
    g_context.state.time  &= ~RTC_TIME_VALID;
    g_context.state.device = RTC_DEVICE_NO_COMM;
    g_context.time.year    = CONFIG_DEFAULT_RTC_YEAR;
    g_context.time.month   = CONFIG_DEFAULT_RTC_MONTH;
    g_context.time.day     = CONFIG_DEFAULT_RTC_DAY;
    g_context.time.hour    = CONFIG_DEFAULT_RTC_HOUR;
    g_context.time.minute  = CONFIG_DEFAULT_RTC_MINUTE;
    g_context.time.second  = CONFIG_DEFAULT_RTC_SECOND;

    return;
}

void ab_rtcmc_term_driver(void)
{
    g_context.state.time  &= ~(RTC_TIME_SET_AND_VALID);
    g_context.state.device = RTC_DEVICE_INACTIVE;
}

void ab_rtcmc_set_time(
    const struct nrtc_time *    time)
{
    struct PORT_C_PACKED ab_rtcmc_time_registers regs;
    nerror                      error;

    regs.years   = bin_to_bcd(time->year - 2000u);
    regs.months  = bin_to_bcd(time->month);
    regs.days    = bin_to_bcd(time->day);
    regs.hours   = bin_to_bcd(time->hour);
    regs.minutes = bin_to_bcd(time->minute);
    regs.seconds = bin_to_bcd(time->second);

    error = i2c_slave_write(&g_context.i2c_slave, REG_SECONDS,
        (const uint8_t *)&regs, sizeof(regs));

    if (error) {
        goto NO_COMM_FAILURE;
    }
    memcpy(&g_context.time, time, sizeof(g_context.time));
    g_context.state.time  |= RTC_TIME_SET_AND_VALID;
    g_context.state.device = RTC_DEVICE_OK;

    return;
NO_COMM_FAILURE:
    g_context.state.time  &= ~(RTC_TIME_SET_AND_VALID);
    g_context.state.device = RTC_DEVICE_NO_COMM;

    return;
}

const struct nrtc_time * ab_rtcmc_get_time(void)
{
    return (&g_context.time);
}

void ab_rtcmc_tick(void)
{
    struct PORT_C_PACKED ab_rtcmc_time_registers regs;
    uint8_t                     reg;
    nerror                      error;

    reg   = 0;
    error = i2c_slave_write(&g_context.i2c_slave, REG_CONTROL_INT_FLAG, &reg, 1);

    if (error) {
        goto FAILURE;
    }
    error = i2c_slave_read(&g_context.i2c_slave, REG_SECONDS, (uint8_t *)&regs,
        sizeof(regs));

    if (error) {
        goto FAILURE;
    }
    g_context.state.time  |= RTC_TIME_VALID;
    g_context.state.device = RTC_DEVICE_OK;
    g_context.time.year    = (uint16_t)bcd_to_bin(regs.years) + 2000u;
    g_context.time.month   = bcd_to_bin(regs.months);
    g_context.time.day     = bcd_to_bin(regs.days);
    g_context.time.hour    = bcd_to_bin(regs.hours);
    g_context.time.minute  = bcd_to_bin(regs.minutes);
    g_context.time.second  = bcd_to_bin(regs.seconds);

    return;
FAILURE:
    g_context.state.time  &= ~RTC_TIME_VALID;
    g_context.state.device = RTC_DEVICE_NO_COMM;

    return;
}

const struct nrtc_state * ab_rtcmc_state(void)
{
    return (&g_context.state);
}
