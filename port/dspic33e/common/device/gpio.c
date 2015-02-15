
#include "mcu/peripheral.h"
#include "mcu/gpio.h"
#include "device/profile.h"

struct gpio
{
    unsigned int                tris;
    unsigned int                port;
    unsigned int                lat;
    unsigned int                odca;
    unsigned int                cnen;
    unsigned int                cnpu;
    unsigned int                cnpd;
    unsigned int                ansel;
};



void ngpio_driver_init(void)
{
    
}



nerror ngpio_init(uint32_t pin_id, const struct gpio_pin_config * config)
{
    unsigned int                gpio;
    unsigned int                pin;
    volatile struct gpio *      regs;

    gpio = pin_id >> 8;
    pin  = (unsigned int)0x1u << (pin_id & 0xfu);
    regs = (volatile struct gpio *)g_gpio[gpio].base_address;

    if (gpio != g_gpio[gpio].id) {
        return (NERROR_NOT_FOUND);
    }
    regs->cnpu &= ~pin;
    regs->cnpd &= ~pin;

    switch (config->flags & (0x7u << 0)) {
        case GPIO_INPUT: {
            regs->tris |= pin;
            break;
        }
        case GPIO_OUTPUT_LOW: {
            regs->lat  &= ~pin;
            regs->odca &= ~pin;
            regs->tris &= ~pin;
            break;
        }
        case GPIO_OUTPUT_HIGH: {
            regs->lat  |=  pin;
            regs->odca &= ~pin;
            regs->tris &= ~pin;
            break;
        }
        case GPIO_OUTPUT_OPEN_DRAIN_LOW: {
            regs->lat  &= ~pin;
            regs->odca |=  pin;
            regs->tris &= ~pin;
            break;
        }
        case GPIO_OUTPUT_OPEN_DRAIN_FLOAT: {
            regs->lat  |=  pin;
            regs->odca |=  pin;
            regs->tris &= ~pin;
            break;
        }
        default : {
            regs->tris |= pin;
            
            return (NERROR_ARG_OUT_OF_RANGE);
        }
    }

    switch (config->flags & (0x3 << 3)) {
        case GPIO_PULL_UP : {
            regs->cnpu |= pin;
            break;
        }
        case GPIO_PULL_DOWN: {
            regs->cnpd |= pin;
            break;
        }
        default : {

            return (NERROR_ARG_OUT_OF_RANGE);
        }
    }

    return (NERROR_NONE);
}



void ngpio_term(uint32_t pin_id)
{
    uint8_t                     gpio;
    uint8_t                     pin;
    volatile struct gpio *      regs;

    gpio = pin_id >> 8;
    pin  = pin_id & 0xfu;
    regs = (volatile struct gpio *)g_gpio[gpio].base_address;
    regs->tris |=  (0x1u << pin);
}



bool ngpio_get(uint32_t pin_id)
{
    uint8_t                     gpio;
    uint8_t                     pin;
    volatile struct gpio *      regs;

    gpio = pin_id >> 8;
    pin  = pin_id & 0xfu;
    regs = (volatile struct gpio *)g_gpio[gpio].base_address;

    return (regs->port & (0x1u << pin));
}



void ngpio_set(uint32_t pin_id)
{
    uint8_t                     gpio;
    uint8_t                     pin;
    volatile struct gpio *      regs;

    gpio = pin_id >> 8;
    pin  = pin_id & 0xfu;
    regs = (volatile struct gpio *)g_gpio[gpio].base_address;
    regs->lat |= (0x1u << pin);
}



void ngpio_clear(uint32_t pin_id)
{
    uint8_t                     gpio;
    uint8_t                     pin;
    volatile struct gpio *      regs;

    gpio = pin_id >> 8;
    pin  = pin_id & 0xfu;
    regs = (volatile struct gpio *)g_gpio[gpio].base_address;
    regs->lat &= ~(0x1u << pin);
}



bool ngpio_request(uint32_t pin_id);
void ngpio_release(uint32_t pin_id);
bool ngpio_change_notice_request(uint32_t pin_id, void (* change_handler)(uint32_t pin_id));
bool ngpio_change_notice_release(uint32_t pin_id);
