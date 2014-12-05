
#include "lib/bitop.h"
#include "port/isr.h"
#include "mcu/peripheral.h"
#include "mcu/clock.h"
#include "mcu/gpio.h"
#include "mcu/i2c.h"
#include "i2c.h"
#include "device/profile.h"


#define CONFIG_PULSE_GOBBLER_NS         104

#define I2C_CON_SEN                     (0x1u << 0)
#define I2C_CON_RSEN                    (0x1u << 1)
#define I2C_CON_PEN                     (0x1u << 2)
#define I2C_CON_RCEN                    (0x1u << 3)
#define I2C_CON_ACKEN                   (0x1u << 4)
#define I2C_CON_ACKDT                   (0x1u << 5)
#define I2C_CON_DISSLW                  (0x1u << 9)
#define I2C_CON_ON                      (0x1u << 15)

#define I2C_STAT_TBF                    (0x1u << 0)
#define I2C_STAT_RBF                    (0x1u << 1)
#define I2C_STAT_P                      (0x1u << 4)
#define I2C_STAT_BCL                    (0x1u << 10)
#define I2C_STAT_TRSTAT                 (0x1u << 14)
#define I2C_STAT_ACKSTAT                (0x1u << 15)

static struct i2c_bus * g_i2c_buses[NPERIPH_I2C1];

struct i2c_regs
{
    volatile unsigned int *     rcv;
    volatile unsigned int *     trn;
    volatile unsigned int *     brg;
    volatile unsigned int *     con;
    volatile unsigned int *     stat;
    volatile unsigned int *     add;
    volatile unsigned int *     msk;
};


void i2c_driver_init(void)
{
    
}



struct i2c_bus * i2c_bus_from_id(
    uint8_t                     bus_id)
{
    return (NULL);
}



nerror i2c_bus_init(
    const struct nperipheral *  periph,
    struct i2c_bus *            bus,
    const struct i2c_master_bus_config * config)
{
    uint32_t                    pbclk;
    uint32_t                    brg;
    struct gpio_pin_config helper_pin;
    
    if (g_i2c_buses[periph->id] != NULL) {
        return (NERROR_NO_RESOURCE);
    }

    if (periph->flags & I2C_ERRATA_01) {
        helper_pin.flags = GPIO_OUTPUT;
        ngpio_init((uint32_t)(periph->flags >> 16), &helper_pin);
        ngpio_set((uint32_t)(periph->flags >> 16));
    }
    bus->periph = periph;
    bus->regs   = (struct i2c_regs *)periph->base_address;
    bus->regs->con = 0;
    /* FIXME: how to create micro delays? */

    if (config->flags & 100000u) {
        *bus->regs->con |= I2C_CON_DISSLW;
    }
    pbclk = clock_get_frequency(periph->clock);
    brg   = (pbclk / (2 * config->speed)) -
        NDIVISION_ROUND((CONFIG_PULSE_GOBBLER_NS * (pbclk / 1000)), 1000000) - 2;

    if (periph->flags & I2C_ERRATA_01) {
        ngpio_term((uint32_t)(periph->flags >> 16));
        *bus->regs->con  |= I2C_CON_ON;
        /* FIXME: how to create micro delays? */
        *bus->regs->brg = brg;
        i2c_bus_start(bus);
        i2c_bus_stop(bus);
    } else {
        *bus->regs->brg  = brg;
        *bus->regs->con |= I2C_CON_ON;
    }

    return (NERROR_NONE);
}



void i2c_bus_term(
    struct i2c_bus *            bus)
{
    while ((*bus->regs->con & (I2C_CON_SEN  | I2C_CON_RSEN | I2C_CON_PEN |
                              I2C_CON_RCEN | I2C_CON_ACKEN)) != 0);

    *bus->regs->con = 0;
}



void i2c_bus_ack(
    struct i2c_bus *            bus)
{
    while ((*bus->regs->con & (I2C_CON_SEN  | I2C_CON_RSEN | I2C_CON_PEN |
                              I2C_CON_RCEN | I2C_CON_ACKEN)) != 0);

    *bus->regs->con &= ~I2C_CON_ACKDT;
    *bus->regs->con |=  I2C_CON_ACKEN;
}



void i2c_bus_nack(
    struct i2c_bus *            bus)
{
    while ((*bus->regs->con & (I2C_CON_SEN  | I2C_CON_RSEN | I2C_CON_PEN |
                              I2C_CON_RCEN | I2C_CON_ACKEN)) != 0);

    *bus->regs->con |= I2C_CON_ACKDT;
    *bus->regs->con |= I2C_CON_ACKEN;
}



nerror i2c_bus_write(
    struct i2c_bus *            bus,
    uint8_t                     data)
{
    while ((*bus->regs->con & (I2C_CON_SEN | I2C_CON_RSEN | I2C_CON_PEN |
                              I2C_CON_RCEN | I2C_CON_ACKEN)) != 0);

    *bus->regs->trn  = data;

    while (*bus->regs->stat & I2C_STAT_TRSTAT);

    if (*bus->regs->stat & (I2C_STAT_ACKSTAT | I2C_STAT_BCL)) {
        return (NERROR_DEVICE_FAIL);
    } else {
        return (NERROR_NONE);
    }
}



uint8_t i2c_bus_read(
    struct i2c_bus *            bus)
{
    while ((*bus->regs->con & (I2C_CON_SEN | I2C_CON_RSEN | I2C_CON_PEN |
                              I2C_CON_RCEN | I2C_CON_ACKEN)) != 0);
    *bus->regs->con |= I2C_CON_RCEN;

    while ((*bus->regs->stat & I2C_STAT_RBF) == 0);

    while (*bus->regs->con & I2C_CON_RCEN);

    return (*bus->regs->rcv);
}



void i2c_bus_start(
    struct i2c_bus *            bus)
{
    while ((*bus->regs->con & (I2C_CON_SEN  | I2C_CON_RSEN | I2C_CON_PEN |
                              I2C_CON_RCEN | I2C_CON_ACKEN)) != 0);

    *bus->regs->con |= I2C_CON_SEN;
}



void i2c_bus_restart(
    struct i2c_bus *            bus)
{
    while ((*bus->regs->con & (I2C_CON_SEN  | I2C_CON_RSEN | I2C_CON_PEN |
                              I2C_CON_RCEN | I2C_CON_ACKEN)) != 0);

    *bus->regs->con |= I2C_CON_RSEN;
}



void i2c_bus_stop(
    struct i2c_bus *            bus)
{
    while ((*bus->regs->con & (I2C_CON_SEN  | I2C_CON_RSEN | I2C_CON_PEN |
                              I2C_CON_RCEN | I2C_CON_ACKEN)) != 0);

    *bus->regs->con |= I2C_CON_PEN;
}



void i2c_slave_open(
    struct i2c_slave *          slave,
    const struct i2c_slave_config * config,
    struct i2c_bus *            bus,
    uint8_t                     id)
{

}



void i2c_slave_close(
    struct i2c_slave *          slave)
{

}



nerror i2c_slave_read(
    struct i2c_slave *          slave,
    uint8_t                     address,
    void *                      data,
    size_t                      size)
{
    return (NERROR_NOT_IMPLEMENTED);
}



nerror i2c_slave_write(
    struct i2c_slave *          slave,
    uint8_t                     address,
    const void *                data,
    size_t                      size)
{
    return (NERROR_NOT_IMPLEMENTED);
}