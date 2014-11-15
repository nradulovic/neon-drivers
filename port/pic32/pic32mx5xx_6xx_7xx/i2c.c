
#include <xc.h>
#include <string.h>

#include "port/cpu.h"
#include "port/isr.h"
#include "mcu/i2c.h"
#include "pic32_isr.h"
#include "pic32_clocks.h"
#include "lib/bitop.h"
#include "port/i2c.h"
#include "registers.h"
#include "lib/debug.h"
#include "port/bitop.h"
#include "lib/module.h"

#define CONFIG_PULSE_GOBBLER_NS         104

#define I2C_CON_SEN                     (0x1u << 0)
#define I2C_CON_RSEN                    (0x1u << 1)
#define I2C_CON_PEN                     (0x1u << 2)
#define I2C_CON_RCEN                    (0x1u << 3)
#define I2C_CON_ACKEN                   (0x1u << 4)
#define I2C_CON_ACKDT                   (0x1u << 5)
#define I2C_CON_DISSLW                  (0x1u << 9)
#define I2C_CON_A10M                    (0x1u << 10)
#define I2C_CON_ON                      (0x1u << 15)

#define I2C_STAT_TBF                    (0x1u << 0)
#define I2C_STAT_RBF                    (0x1u << 1)
#define I2C_STAT_P                      (0x1u << 4)
#define I2C_STAT_BCL                    (0x1u << 10)
#define I2C_STAT_TRSTAT                 (0x1u << 14)
#define I2C_STAT_ACKSTAT                (0x1u << 15)

struct i2c_hw_reg
{
    volatile ncpu_reg *         i2c_con;
    volatile ncpu_reg *         i2c_stat;
    volatile ncpu_reg *         i2c_add;
    volatile ncpu_reg *         i2c_msk;
    volatile ncpu_reg *         i2c_brg;
    volatile ncpu_reg *         i2c_trn;
    volatile ncpu_reg *         i2c_rcv;
};

struct i2c_periph
{
    struct i2c_hw_reg           regs;
    struct pic32_isr            isr_slave;
};

struct i2c_bus
{
    const struct i2c_periph * const device;
};

static NMODULE_INFO_CREATE("I2C PIC32mx5xx/6xx/7xx driver", "Nenad Radulovic");

#if defined (I2C_ID_1)
static const struct i2c_periph g_i2c1_periph =
{
    {
        &I2C1CON,
        &I2C1STAT,
        &I2C1ADD,
        &I2C1MSK,
        &I2C1BRG,
        &I2C1TRN,
        &I2C1RCV,
    },
    {
        0,
        30,
        6,
        8
    }
};
#endif

#if defined (I2C_ID_2)
static const struct i2c_periph g_i2c2_periph =
{
    {
        &I2C2CON,
        &I2C2STAT,
        &I2C2ADD,
        &I2C2MSK,
        &I2C2BRG,
        &I2C2TRN,
        &I2C2RCV,
    },
    {
        0,
        30,
        6,
        8
    }
};
#endif

#if defined (I2C_ID_3)
static const struct i2c_periph g_i2c3_periph =
{
    {
        &I2C3CON,
        &I2C3STAT,
        &I2C3ADD,
        &I2C3MSK,
        &I2C3BRG,
        &I2C3TRN,
        &I2C3RCV,
    },
    {
        0,
        27,
        6,
        0
    }
};
#endif

#if defined (I2C_ID_5)
static const struct i2c_periph g_i2c5_periph =
{
    {
        &I2C5CON,
        &I2C5STAT,
        &I2C5ADD,
        &I2C5MSK,
        &I2C5BRG,
        &I2C5TRN,
        &I2C5RCV,
    },
    {
        1,
        9,
        8,
        0
    }
};
#endif

static struct i2c_bus g_i2c_bus[] =
{
#if defined (I2C_ID_1)
    {
        &g_i2c1_periph
    },
#endif
#if defined (I2C_ID_2)
    {
        &g_i2c2_periph
    },
#endif
#if defined (I2C_ID_3)
    {
        &g_i2c3_periph
    },
#endif
#if defined (I2C_ID_4)
    {
        &g_i2c4_periph
    },
#endif
#if defined (I2C_ID_5)
    {
        &g_i2c5_periph
    },
#endif
};



void i2c_driver_init(void)
{

}



struct i2c_bus * i2c_bus_from_id(
    uint32_t                    id)
{
    NREQUIRE(NAPI_RANGE, id > NARRAY_DIMENSION(g_i2c_bus));
    
    return (&g_i2c_bus[id]);
}



nerror i2c_bus_init(
    struct i2c_bus *            bus, 
    const struct i2c_master_bus_config * config)
{
    const struct i2c_hw_reg *   regs = &bus->device->regs;
    uint32_t                    pb_clock;

    *regs->i2c_con  = 0;
    *regs->i2c_stat = 0;

    if (config->speed > 100000u) {
        REGISTER_SET(regs->i2c_con, I2C_CON_DISSLW);
    }
    pb_clock = clock_get_peripheral_clock();
    *regs->i2c_brg = (pb_clock / (2 * config->speed)) - 2 -
        NDIVISION_ROUND((CONFIG_PULSE_GOBBLER_NS * (pb_clock / 1000u)), 1000000u);

    if (config->flags & I2C_BUS_ADDRESS_10BIT) {
        REGISTER_SET(regs->i2c_con, I2C_CON_A10M);
    }

    if (config->flags & I2C_BUS_MODE_SLAVE) {
#if 0
        if (pic32_isr_request(&bus->device->isr_slave) != NERROR_NONE) {

            return (NERROR_NO_RESOURCE);
        }
        *regs->i2c_msk = 0;
        *regs->i2c_add = config->flags >> 16u;
        PIC32_ISR_CLR(&bus->device->isr_slave);
        PIC32_ISR_SET_PRIO(&bus->device->isr_slave, config->isr_prio);
        PIC32_ISR_ENABLE(&bus->device->isr_slave);
#else
        return (NERROR_NOT_IMPLEMENTED);
#endif
    }
    REGISTER_SET(regs->i2c_con, I2C_CON_ON);

    return (NERROR_NONE);
}



void i2c_bus_term(
    struct i2c_bus *            bus)
{
    const struct i2c_hw_reg *   regs = &bus->device->regs;

    while (*regs->i2c_con & (I2C_CON_SEN  | I2C_CON_RSEN | I2C_CON_PEN |
                             I2C_CON_RCEN | I2C_CON_ACKEN));
    *regs->i2c_con = 0;
}


    
void i2c_bus_ack(
    struct i2c_bus *            bus)
{
    const struct i2c_hw_reg *   regs = &bus->device->regs;

    while (*regs->i2c_con & (I2C_CON_SEN  | I2C_CON_RSEN | I2C_CON_PEN |
                             I2C_CON_RCEN | I2C_CON_ACKEN));

    REGISTER_CLR(regs->i2c_con, I2C_CON_ACKDT);
    REGISTER_SET(regs->i2c_con, I2C_CON_ACKEN);
}


    
void i2c_bus_nack(
    struct i2c_bus *            bus)
{
    const struct i2c_hw_reg *   regs = &bus->device->regs;

    while (*regs->i2c_con & (I2C_CON_SEN  | I2C_CON_RSEN | I2C_CON_PEN |
                             I2C_CON_RCEN | I2C_CON_ACKEN));
    REGISTER_SET(regs->i2c_con, I2C_CON_ACKDT);
    REGISTER_SET(regs->i2c_con, I2C_CON_ACKEN);
}



nerror i2c_bus_write(
    struct i2c_bus *            bus,
    uint8_t                     data)
{
    const struct i2c_hw_reg *   regs = &bus->device->regs;

    while (*regs->i2c_con & (I2C_CON_SEN  | I2C_CON_RSEN | I2C_CON_PEN |
                             I2C_CON_RCEN | I2C_CON_ACKEN));

    *regs->i2c_trn = data;

    while (*regs->i2c_stat & I2C_STAT_TRSTAT);

    if (*regs->i2c_stat & (I2C_STAT_ACKSTAT | I2C_STAT_BCL)) {

        return (NERROR_DEVICE_BUSY);
    } else {

        return (NERROR_NONE);
    }
}



uint8_t i2c_bus_read(
    struct i2c_bus *            bus)
{
    const struct i2c_hw_reg *   regs = &bus->device->regs;

    while (*regs->i2c_con & (I2C_CON_SEN  | I2C_CON_RSEN | I2C_CON_PEN |
                             I2C_CON_RCEN | I2C_CON_ACKEN));
    *regs->i2c_con |= I2C_CON_RCEN;

    while ((*regs->i2c_stat & I2C_STAT_RBF) == 0);

    while (*regs->i2c_con & I2C_CON_RCEN);

    return (*regs->i2c_rcv);
}



void i2c_bus_start(
    struct i2c_bus *            bus)
{
    const struct i2c_hw_reg *   regs = &bus->device->regs;

    while (*regs->i2c_con & (I2C_CON_SEN  | I2C_CON_RSEN | I2C_CON_PEN |
                             I2C_CON_RCEN | I2C_CON_ACKEN));
    REGISTER_SET(regs->i2c_con, I2C_CON_SEN);
}



void i2c_bus_restart(
    struct i2c_bus *            bus)
{
    const struct i2c_hw_reg *   regs = &bus->device->regs;

    while (*regs->i2c_con & (I2C_CON_SEN  | I2C_CON_RSEN | I2C_CON_PEN |
                             I2C_CON_RCEN | I2C_CON_ACKEN));
    REGISTER_SET(regs->i2c_con, I2C_CON_RSEN);
}



void i2c_bus_stop(
    struct i2c_bus *            bus)
{
    const struct i2c_hw_reg *   regs = &bus->device->regs;

    while (*regs->i2c_con & (I2C_CON_SEN  | I2C_CON_RSEN | I2C_CON_PEN |
                             I2C_CON_RCEN | I2C_CON_ACKEN));
    REGISTER_SET(regs->i2c_con, I2C_CON_PEN);
}



void i2c_slave_open(
    struct i2c_slave *          slave,
    const struct i2c_slave_config * config,
    struct i2c_bus *            bus,
    uint8_t                     id)
{
    slave->bus     = bus;
    slave->address = config->address | id;
    slave->flags   = config->flags;
}



void i2c_slave_close(
    struct i2c_slave *          slave)
{
    memset(slave, 0, sizeof(*slave));
}


    
nerror i2c_slave_read(
    struct i2c_slave *          slave,
    uint8_t                     address,
    void *                      data,
    size_t                      size)
{
    uint8_t *                   data_ = data;

    i2c_bus_start(slave->bus);

    if (!(slave->flags & I2C_SLAVE_RD_REPEAT)) {
        if (i2c_bus_write(slave->bus, slave->address) != NERROR_NONE) {
            goto FAILURE;
        }

        if (i2c_bus_write(slave->bus, address) != NERROR_NONE) {
            goto FAILURE;
        }

        if (slave->flags & I2C_SLAVE_RD_START_STOP) {
            i2c_bus_stop(slave->bus);
            i2c_bus_start(slave->bus);
        } else {
            i2c_bus_restart(slave->bus);
        }
    }

    if (i2c_bus_write(slave->bus, slave->address | 0x01u) == false) {
        goto FAILURE;
    }
    size--;

    while (size != 0) {
        *data_ = i2c_bus_read(slave->bus);
        i2c_bus_ack(slave->bus);
        data_++;
        size--;
    }
    *data_ = i2c_bus_read(slave->bus);

    if (slave->flags & I2C_SLAVE_RD_NACK) {
        i2c_bus_nack(slave->bus);
    }
    i2c_bus_stop(slave->bus);

    return (NERROR_NONE);
FAILURE:
    i2c_bus_nack(slave->bus);
    i2c_bus_stop(slave->bus);

    return (NERROR_DEVICE_BUSY);
}



nerror i2c_slave_write(
    struct i2c_slave *          slave,
    uint8_t                     address,
    const void *                data,
    size_t                      size)
{
    const uint8_t *             data_ = data;

    i2c_bus_start(slave->bus);

    if (i2c_bus_write(slave->bus, slave->address) != NERROR_NONE) {
        goto FAILURE;
    }

    if (i2c_bus_write(slave->bus, address) != NERROR_NONE) {
        goto FAILURE;
    }

    while (size != 0) {
        if (i2c_bus_write(slave->bus, *data_) != NERROR_NONE) {
            goto FAILURE;
        }
        data_++;
        size--;
    }
    i2c_bus_stop(slave->bus);

    return (NERROR_NONE);
FAILURE:
    i2c_bus_stop(slave->bus);

    return (NERROR_DEVICE_BUSY);
}
