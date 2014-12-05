
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

    if (config->flags & GPIO_OUTPUT_OPEN_DRAIN) {
        regs->odca |=  pin;
    } else {
        regs->odca &= ~pin;
    }
    regs->cnpu &= ~pin;
    regs->cnpd &= ~pin;

    if (config->flags & GPIO_PULL_UP) {
        regs->cnpu |= pin;
    } else if (config->flags & GPIO_PULL_DOWN) {
        regs->cnpd |= pin;
    }

    if (config->flags & GPIO_OUTPUT) {
        regs->tris &= ~pin;
    } else {
        regs->tris |=  pin;
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
