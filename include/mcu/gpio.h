#ifndef NGPIO_H_
#define NGPIO_H_

#include <stdbool.h>
#include <stdint.h>

#include "shared/error.h"
#include "device/gpio_device.h"

#define NGPIO_DEVICE_CLASS_ID           ((uint32_t)0xdead0000u)

#define NGPIO_PIN_ID(port, pin)         ((port) * PORT_BITS_PER_GPIO + (pin))
#define NGPIO_PIN_ID_TO_PORT(id)        ((id) / PORT_BITS_PER_GPIO)
#define NGPIO_PIN_ID_TO_PIN(id)         ((id) % PORT_BITS_PER_GPIO)

#define NGPIOA                          0
#define NGPIOB                          1
#define NGPIOC                          2
#define NGPIOD                          3
#define NGPIOE                          4
#define NGPIOF                          5
#define NGPIOG                          6
#define NGPIOH                          7
#define NGPIOI                          8
#define NGPIOJ                          9

#define NGPIO_INPUT                     (0x0ul << 0)
#define NGPIO_OUTPUT_LOW                (0x1ul << 0)
#define NGPIO_OUTPUT_HIGH               (0x2ul << 0)
#define NGPIO_OUTPUT_OPEN_DRAIN_LOW     (0x3ul << 0)
#define NGPIO_OUTPUT_OPEN_DRAIN_FLOAT   (0x4ul << 0)
#define NGPIO_PULL_UP                   (0x1ul << 3)
#define NGPIO_PULL_DOWN                 (0x2ul << 3)

struct nmux;

struct gpio_pin_config 
{
    uint32_t                    flags;
};

nerror ngpio_init(uint32_t pin_id, const struct gpio_pin_config * config);
void ngpio_term(uint32_t pin_id);
bool ngpio_get(uint32_t pin_id);
void ngpio_set(uint32_t pin_id);
void ngpio_clear(uint32_t pin_id);
bool ngpio_request(uint32_t pin_id);
void ngpio_release(uint32_t pin_id);
bool ngpio_change_notice_request(uint32_t pin_id, void (* change_handler)(uint32_t pin_id));
bool ngpio_change_notice_release(uint32_t pin_id);

void ngpio_mux_init(uint32_t pin_id, const struct nmux * mux);

#endif /* NGPIO_H_ */
