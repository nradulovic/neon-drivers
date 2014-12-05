#ifndef NGPIO_H_
#define NGPIO_H_

#include <stdbool.h>
#include <stdint.h>

#include "shared/error.h"
#include "periph/gpio.h"

#ifndef GPIO_PIN_ID
#define GPIO_PIN_ID(port, pin)          ((port) * PORT_BITS_PER_GPIO + (pin))
#endif

#define GPIO_INPUT                      (0x0ul << 0)
#define GPIO_OUTPUT_LOW                 (0x1ul << 0)
#define GPIO_OUTPUT_HIGH                (0x2ul << 0)
#define GPIO_OUTPUT_OPEN_DRAIN_LOW      (0x3ul << 0)
#define GPIO_OUTPUT_OPEN_DRAIN_FLOAT    (0x4ul << 0)
#define GPIO_PULL_UP                    (0x1ul << 3)
#define GPIO_PULL_DOWN                  (0x2ul << 3)

struct gpio_port;

struct gpio_pin_config 
{
    uint32_t                    flags;
};

struct ngpio_pin
{
    struct gpio_port *          port;
    uint32_t                    pin_mask;
};

void ngpio_driver_init(void);

nerror ngpio_init(uint32_t pin_id, const struct gpio_pin_config * config);
void ngpio_term(uint32_t pin_id);
bool ngpio_get(uint32_t pin_id);
void ngpio_set(uint32_t pin_id);
void ngpio_clear(uint32_t pin_id);
bool ngpio_request(uint32_t pin_id);
void ngpio_release(uint32_t pin_id);
bool ngpio_change_notice_request(uint32_t pin_id, void (* change_handler)(uint32_t pin_id));
bool ngpio_change_notice_release(uint32_t pin_id);

#endif /* NGPIO_H_ */
