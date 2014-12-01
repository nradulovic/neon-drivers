
#include <string.h>

#include "rtc/ab_rtcmc_32768_eoz9.h"
#include "rtc/rtc_class.h"
#include "port/i2c.h"
#include "shared/error.h"


#define CONFIG_DEFAULT_YEAR             2014
#define CONFIG_DEFAULT_MONTH            1
#define CONFIG_DEFAULT_DAY              1
#define CONFIG_DEFAULT_HOUR             0
#define CONFIG_DEFAULT_MINUTE           0
#define CONFIG_DEFAULT_SECOND           0

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
#define CONTROL_STATUS_PON              (0x1u << 5)
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
    ncpu_reg                    isr_prio;
};

struct rtc_time_registers
{
    uint8_t                     seconds;
    uint8_t                     minutes;
    uint8_t                     hours;
    uint8_t                     days;
    uint8_t                     weekdays;
    uint8_t                     months;
    uint8_t                     years;
};

static struct context g_context;

struct nrtc_time              g_rtc_time;

const struct i2c_slave_config g_rtc_i2c_config =
{
    I2C_SLAVE_RD_NACK | I2C_SLAVE_RD_REPEAT,
    RTC_SLAVE_ADDRESS
};

nerror ab_rtcmc_init_driver(const void * config)
{
    const struct ab_rtcmc_32768_eoz9_config * custom_config;
    uint8_t                     reg;
    struct rtc_time_registers   regs;
    nerror                      error;

    custom_config      = (const struct ab_rtcmc_32768_eoz9_config *)config;
    g_context.isr_prio = custom_config->isr_prio;
    i2c_slave_open(&g_context.i2c_slave, &g_rtc_i2c_config, custom_config->bus,
        custom_config->id);

    error = i2c_slave_read(&g_context.i2c_slave, REG_CONTROL_STATUS, &reg, 1);

    if (error) {
        goto FAILURE;
    }

    if (reg & CONTROL_STATUS_PON) {
        reg &= ~CONTROL_STATUS_PON;

        error = i2c_slave_write(&g_context.i2c_slave, REG_CONTROL_STATUS, &reg, 1);

        if (error) {
            goto FAILURE;
        }
        g_rtc_time.year   = CONFIG_DEFAULT_YEAR;
        g_rtc_time.month  = CONFIG_DEFAULT_MONTH;
        g_rtc_time.day    = CONFIG_DEFAULT_DAY;
        g_rtc_time.hour   = CONFIG_DEFAULT_HOUR;
        g_rtc_time.minute = CONFIG_DEFAULT_MINUTE;
        g_rtc_time.second = CONFIG_DEFAULT_SECOND;

        error = ab_rtcmc_set_time(&g_rtc_time);

        if (error) {
            goto FAILURE;
        }
    }
    memset(&regs, 0, sizeof(regs));
    reg   = 0;
    error = i2c_slave_write(&g_context.i2c_slave, REG_CONTROL_INT, &reg, 1);

    if (error) {
        goto FAILURE;
    }
    reg   = CONTROL_1_WE;
    error = i2c_slave_write(&g_context.i2c_slave, REG_CONTROL_1, &reg, 1);

    if (error) {
        goto FAILURE;
    }
    reg   = 32;
    error = i2c_slave_write(&g_context.i2c_slave, REG_TIMER_LOW, &reg, 1);

    if (error) {
        goto FAILURE;
    }
    reg   = 0;
    error = i2c_slave_write(&g_context.i2c_slave, REG_TIMER_HIGH, &reg, 1);

    if (error) {
        goto FAILURE;
    }
    reg   = CONTROL_1_TAR | CONTROL_1_WE;
    error = i2c_slave_write(&g_context.i2c_slave, REG_CONTROL_1, &reg, 1);

    if (error) {
        goto FAILURE;
    }
    reg  |= CONTROL_1_TE;
    error = i2c_slave_write(&g_context.i2c_slave, REG_CONTROL_1, &reg, 1);

    if (error) {
        goto FAILURE;
    }
    reg   = 0;
    error = i2c_slave_write(&g_context.i2c_slave, REG_CONTROL_INT_FLAG, &reg, 0);

    if (error) {
        goto FAILURE;
    }
    reg   = CONTROL_INT_TIE;
    error = i2c_slave_write(&g_context.i2c_slave, REG_CONTROL_INT, &reg, 0);
    
    if (error) {
        goto FAILURE;
    }

    return (error);
FAILURE:
    g_rtc_time.year = 0;

    return (error);
}


