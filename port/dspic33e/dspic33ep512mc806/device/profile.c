
#include <xc.h>
#include <stddef.h>

#include "mcu/peripheral.h"
#include "mcu/clock.h"
#include "port/isr.h"

static const struct nclock g_clock_peripheral =
{
    CLOCK_TYPE_PB
};

/*--  I2C1  ------------------------------------------------------------------*/

static const struct nisr g_i2c1_isr[] =
{
    {
        1,                              /* IFS1/IEC1                        */
        0,                              /* bit position 0                   */
        4,                              /* IPC4                             */
        0                               /* bit mask position 0              */
    }, {
        1,                              /* IFS1/IEC1                        */
        1,                              /* bit position 1                   */
        4,                              /* IPC4                             */
        4                               /* bit mask position 4              */
    }
};

const struct nperipheral g_i2c[] =
{
    {
        0,
        0,
        NULL,
        (volatile unsigned int *)0x0200,
        g_i2c1_isr,
        &g_clock_peripheral,
        NULL
    }
};

/*--  GPIO  ------------------------------------------------------------------*/
const struct nperipheral g_gpio[] =
{
    {
        UINT32_MAX, /* PORT A - not used */
        0,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    }, {
        1, /* PORT B */
        0,
        NULL,
        (volatile unsigned int *)0x0e10,
        NULL,
        NULL,
        NULL
    }, {
        2, /* PORT C */
        0,
        NULL,
        (volatile unsigned int *)0x0e20,
        NULL,
        NULL,
        NULL
    }, {
        3, /* PORT D */
        0,
        NULL,
        (volatile unsigned int *)0x0e30,
        NULL,
        NULL,
        NULL
    },{
        4, /* PORT E */
        0,
        NULL,
        (volatile unsigned int *)0x0e40,
        NULL,
        NULL,
        NULL
    }, {
        5, /* PORT F */
        0,
        NULL,
        (volatile unsigned int *)0x0e50,
        NULL,
        NULL,
        NULL
    }, {
        6, /* PORT G */
        0,
        NULL,
        (volatile unsigned int *)0x0e60,
        NULL,
        NULL,
        NULL
    }
};
