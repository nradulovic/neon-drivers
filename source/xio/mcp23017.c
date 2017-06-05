/*
 * Copyright 2015 Netico Solutions d.o.o
 *  
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 *      Template version: 1.0.0 (06.04.2015)
 *
 *//***********************************************************************//**
 * @file
 * @author dejan
 * @date   Oct 29, 2015
 * @brief 
 *********************************************************************//** @{ */

/*=========================================================  INCLUDE FILES  ==*/

#include "xio/mcp23017.h"

/*=========================================================  LOCAL MACRO's  ==*/
/*======================================================  LOCAL DATA TYPES  ==*/

struct mcp_workspace
{
    struct nequeue              deferred;
    struct nevent *             deferred_queue_storage[CONFIG_NXIO_QUEUE_SIZE];
    struct nepa *               client;
    nevent *                    event;
    uint32_t                    pin;
    enum nxio_pin_state *       state;
    uint32_t *                  value;
};

/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/

static naction state_init(struct nsm * , const struct nevent * );
static naction state_open(struct nsm * , const struct nevent * );
static naction state_config(struct nsm * , const struct nevent * );
static naction state_idle(struct nsm * , const struct nevent * );
static naction state_write_pin(struct nsm * , const struct nevent * );
static naction state_write(struct nsm * , const struct nevent * );
static naction state_read_pin(struct nsm * , const struct nevent * );
static naction state_read(struct nsm * , const struct nevent * );

/*=======================================================  LOCAL VARIABLES  ==*/

static struct nevent *          g_queue_storage[CONFIG_NXIO_QUEUE_SIZE];
static struct mcp_workspace     g_workspace;

/*======================================================  GLOBAL VARIABLES  ==*/

struct nepa                     g_mcp23017_epa;
const struct nepa_define        g_mcp23017_define =
{
    NSM_DEF_INIT(&g_workspace, state_init, NSM_TYPE_FSM),
    NEQUEUE_DEF_INIT(g_queue_storage,  sizeof(g_queue_storage)),
    NTHREAD_DEF_INIT("mcp23017_epa", CONFIG_NXIO_PRIORITY),
};

/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/

static inline void mcp_config(struct nxio_event_config * config)
{
    struct mcp23017_driver *    mcp;
    uint32_t                    pin;
    uint16_t                    mode_reg = 0xFFFF;
    uint16_t                    pull_reg = 0x00;

    for (pin = 0u; pin < MCP23017_PINS_QUANTITY; pin++) {
        switch (config->modes[pin]) {
            case NXIO_MODE_INPUT_PULL_UP: {
                pull_reg |= (1u << pin);
                break;
            }
            case NXIO_MODE_OUTPUT_PUSH_PULL: {
                mode_reg &= ~(1u << pin);
                break;
            }
            default: {
                ;
            }
        }
    }
    mcp = (struct mcp23017_driver *)config->nxio;
    mcp->regs.gppu  = (uint8_t)pull_reg;
    mcp->regs.iodir = (uint8_t)mode_reg;
}



static naction state_init(
    struct nsm *                sm,
    const struct nevent *       event)
{
    struct mcp_workspace * ws = sm->wspace;

    switch (event->id) {
        case NSM_INIT: {
            struct nequeue_define deferred_def =
                NEQUEUE_DEF_INIT(ws->deferred_queue_storage, sizeof(ws->deferred_queue_storage));
            nepa_defer_init(&ws->deferred, &deferred_def);

            return (naction_transit_to(sm, state_idle));
        }
        default: {
            return (naction_ignored());
        }
    }
}



static naction state_idle(
    struct nsm *                sm,
    const struct nevent *       event)
{
    struct mcp_workspace * ws = sm->wspace;

    switch (event->id) {
        case NSM_ENTRY: {
            nepa_defer_fetch_one(&ws->deferred);

            return (naction_handled());
        }
        case EVT_NXIO_OPEN: {
            nevent_lock(event);
            ws->event   = (nevent *)event;
            ws->client  = event->producer;

            return (naction_transit_to(sm, state_open));
        }
        case EVT_NXIO_CONFIG: {
            nevent_lock(event);
            ws->event   = (nevent *)event;
            ws->client  = event->producer;

            return (naction_transit_to(sm, state_config));
        }
        case EVT_NXIO_WRITE_PIN: {
            nevent_lock(event);
            ws->event   = (nevent *)event;
            ws->client  = event->producer;

            return (naction_transit_to(sm, state_write_pin));
        }
        case EVT_NXIO_WRITE: {
            nevent_lock(event);
            ws->event   = (nevent *)event;
            ws->client  = event->producer;

            return (naction_transit_to(sm, state_write));
        }
        case EVT_NXIO_READ_PIN: {
            nevent_lock(event);
            ws->event   = (nevent *)event;
            ws->client  = event->producer;

            return (naction_transit_to(sm, state_read_pin));
        }
        case EVT_NXIO_READ: {
            nevent_lock(event);
            ws->event   = (nevent *)event;
            ws->client  = event->producer;

            return (naction_transit_to(sm, state_read));
        }
        default: {
            return (naction_ignored());
        }
    }
}



static naction state_open(
    struct nsm *                sm,
    const struct nevent *       event)
{
    struct mcp_workspace * ws = sm->wspace;

    switch (event->id) {
        case NSM_INIT: {
            struct ni2c_transfer_event *    transfer;
            struct ni2c_open_event *        i2c_open;
            struct nxio_event_open *        open;
            struct nxio_driver *            driver;
            struct mcp23017_driver *        mcp;

            open                = (struct nxio_event_open *)ws->event;

            i2c_open = (struct ni2c_open_event *)nevent_create(
                    sizeof(struct ni2c_open_event), EVT_NI2C_OPEN_SLAVE);

            transfer = (struct ni2c_transfer_event *)nevent_create(
                    sizeof(struct ni2c_transfer_event), EVT_NI2C_READ_SLAVE);

            driver              = open->nxio;
            mcp                 = (struct mcp23017_driver *)driver;

            driver->address     = open->address;
            driver->device_id   = open->device_id;
            driver->ni2c_epa    = open->ni2c_epa;

            i2c_open->slave     = &driver->slave;
            i2c_open->address   = ((driver->address << 4) & 0xF0);
            i2c_open->address   += (driver->device_id & 0x7);
            i2c_open->flags     = 0u;

            transfer->slave     = &driver->slave;
            transfer->reg       = REG16_IODIR_ADDR;
            transfer->data      = &mcp->regs;
            transfer->size      = sizeof(struct mcp23017_word_registers);

            nepa_send_event(open->ni2c_epa, (nevent *)i2c_open);
            nepa_send_event(open->ni2c_epa, (nevent *)transfer);
            nevent_unlock(ws->event);

            return (naction_transit_to(sm, state_idle));
        }
        default: {
            return (naction_ignored());
        }
    }
}



static naction state_config (
        struct nsm *                sm,
        const struct nevent *       event)
{
    struct mcp_workspace * ws = sm->wspace;

    switch (event->id) {
        case NSM_ENTRY: {
            struct nxio_event_config *      config;
            struct ni2c_transfer_event *    transfer;
            struct nxio_driver *            driver;
            struct mcp23017_driver *        mcp;

            config   = (struct nxio_event_config *)ws->event;
            transfer = (struct ni2c_transfer_event *)nevent_create(
                    sizeof(struct ni2c_transfer_event), EVT_NI2C_WRITE_SLAVE);

            mcp_config(config);

            driver          = config->nxio;
            mcp             = (struct mcp23017_driver *)driver;
            transfer->slave = &driver->slave;
            transfer->reg   = REG16_IODIR_ADDR;
            transfer->data  = &mcp->regs;
            transfer->size  = sizeof(struct mcp23017_word_registers);
            ws->client      = event->producer;

            nepa_send_event(driver->ni2c_epa, (nevent *)transfer);
            nevent_unlock(ws->event);

            return (naction_handled());
        }
        case EVT_NI2C_TRANSFER_COMPLETED: {
            nepa_send_signal(ws->client, EVT_NXIO_COMPLETED);

            return (naction_handled());
        }
        case EVT_NI2C_ERROR: {
            nepa_send_signal(ws->client, EVT_NXIO_FAILED);

            return (naction_transit_to(sm, state_idle));
        }
        default: {
            nepa_defer_event(&ws->deferred, event);

            return (naction_ignored());
        }
    }
}



static naction state_write_pin(
        struct nsm *                sm,
        const struct nevent *       event)
{
    struct mcp_workspace * ws = sm->wspace;

    switch (event->id) {
        case NSM_ENTRY: {
            struct nxio_event_write_pin *   write;
            struct ni2c_transfer_event *    transfer;
            struct nxio_driver *            driver;
            struct mcp23017_driver *        mcp;
            uint16_t                        gpio;

            write    = (struct nxio_event_write_pin *)ws->event;
            transfer = (struct ni2c_transfer_event *)nevent_create(
                    sizeof(struct ni2c_transfer_event), EVT_NI2C_WRITE_SLAVE);

            driver          = write->nxio;
            mcp             = (struct mcp23017_driver *)driver;
            gpio            = mcp->regs.gpio;

            if (write->state == NXIO_PIN_STATE_HIGH) {
                gpio |= (1u << write->pin);
            } else {
                gpio &= ~(1u << write->pin);
            }
            mcp->regs.gpio  = gpio;
            transfer->slave = &driver->slave;
            transfer->reg   = REG16_GPIO_ADDR;
            transfer->data  = &mcp->regs;
            transfer->size  = sizeof(mcp->regs.gpio);

            nepa_send_event(driver->ni2c_epa, (nevent *)transfer);
            nevent_unlock(ws->event);

            return (naction_handled());
        }
        case EVT_NI2C_TRANSFER_COMPLETED: {
            nepa_send_signal(ws->client, EVT_NXIO_COMPLETED);

            return (naction_transit_to(sm, state_idle));
        }
        case EVT_NI2C_ERROR: {
            nepa_send_signal(ws->client, EVT_NXIO_FAILED);

            return (naction_transit_to(sm, state_idle));
        }
        default: {
            nepa_defer_event(&ws->deferred, event);

            return (naction_ignored());
        }
    }
}



static naction state_write(
        struct nsm *                sm,
        const struct nevent *       event)
{
    struct mcp_workspace * ws = sm->wspace;

    switch (event->id) {
        case NSM_ENTRY: {
            struct nxio_event_write *       write;
            struct ni2c_transfer_event *    transfer;
            struct nxio_driver *            driver;
            struct mcp23017_driver *        mcp;
            uint16_t                        gpio;

            write    = (struct nxio_event_write *)ws->event;
            transfer = (struct ni2c_transfer_event *)nevent_create(
                    sizeof(struct ni2c_transfer_event), EVT_NI2C_WRITE_SLAVE);

            driver          = write->nxio;
            mcp             = (struct mcp23017_driver *)driver;
            gpio            = mcp->regs.gpio;
            mcp->regs.gpio  = gpio;
            transfer->slave = &driver->slave;
            transfer->reg   = REG16_GPIO_ADDR;
            transfer->data  = &mcp->regs;
            transfer->size  = sizeof(mcp->regs.gpio);

            nepa_send_event(driver->ni2c_epa, (nevent *)transfer);
            nevent_unlock(ws->event);

            return (naction_handled());
        }
        case EVT_NI2C_TRANSFER_COMPLETED: {
            nepa_send_signal(ws->client, EVT_NXIO_COMPLETED);

            return (naction_transit_to(sm, state_idle));
        }
        case EVT_NI2C_ERROR: {
            nepa_send_signal(ws->client, EVT_NXIO_FAILED);

            return (naction_transit_to(sm, state_idle));
        }
        default: {
            nepa_defer_event(&ws->deferred, event);

            return (naction_ignored());
        }
    }
}



static naction state_read_pin(
        struct nsm *                sm,
        const struct nevent *       event)
{
    struct mcp_workspace * ws = sm->wspace;

    switch (event->id) {
        case NSM_ENTRY: {
            struct nxio_event_read_pin *    read;
            struct ni2c_transfer_event *    transfer;
            struct nxio_driver *            driver;
            struct mcp23017_driver *        mcp;
            uint16_t                        gpio;

            read     = (struct nxio_event_read_pin *)ws->event;
            transfer = (struct ni2c_transfer_event *)nevent_create(
                    sizeof(struct ni2c_transfer_event), EVT_NI2C_READ_SLAVE);

            driver          = read->nxio;
            mcp             = (struct mcp23017_driver *)driver;
            gpio            = mcp->regs.gpio;

            mcp->regs.gpio  = gpio;
            transfer->slave = &driver->slave;
            transfer->reg   = REG16_GPIO_ADDR;
            transfer->data  = &mcp->regs;
            transfer->size  = sizeof(mcp->regs.gpio);
            ws->pin         = read->pin;
            ws->state       = read->state;

            nepa_send_event(driver->ni2c_epa, (nevent *)transfer);

            return (naction_handled());
        }
        case EVT_NI2C_TRANSFER_COMPLETED: {
            struct nxio_event_read_pin *    read;
            struct nxio_driver *            driver;
            struct mcp23017_driver *        mcp;
            uint16_t                        gpio;

            read    = (struct nxio_event_read_pin *)ws->event;
            driver  = read->nxio;
            mcp     = (struct mcp23017_driver *)driver;
            gpio    = mcp->regs.gpio;

            if (gpio & (1u << ws->pin)) {
                *ws->state = NXIO_PIN_STATE_HIGH;
            } else {
                *ws->state = NXIO_PIN_STATE_LOW;
            }
            nepa_send_signal(ws->client, EVT_NXIO_COMPLETED);
            nevent_unlock(ws->event);

            return (naction_transit_to(sm, state_idle));
        }
        case EVT_NI2C_ERROR: {
            nepa_send_signal(ws->client, EVT_NXIO_FAILED);
            nevent_unlock(ws->event);

            return (naction_transit_to(sm, state_idle));
        }
        default: {
            nepa_defer_event(&ws->deferred, event);

            return (naction_ignored());
        }
    }
}



static naction state_read(
        struct nsm *                sm,
        const struct nevent *       event)
{
    struct mcp_workspace * ws = sm->wspace;

    switch (event->id) {
        case NSM_ENTRY: {
            struct nxio_event_read *        read;
            struct ni2c_transfer_event *    transfer;
            struct nxio_driver *            driver;
            struct mcp23017_driver *        mcp;
            uint16_t                        gpio;

            read     = (struct nxio_event_read *)ws->event;
            transfer = (struct ni2c_transfer_event *)nevent_create(
                    sizeof(struct ni2c_transfer_event), EVT_NI2C_READ_SLAVE);

            driver          = read->nxio;
            mcp             = (struct mcp23017_driver *)driver;
            gpio            = mcp->regs.gpio;

            mcp->regs.gpio  = gpio;
            transfer->slave = &driver->slave;
            transfer->reg   = REG16_GPIO_ADDR;
            transfer->data  = &mcp->regs;
            transfer->size  = sizeof(mcp->regs.gpio);
            ws->value       = read->value;
            nepa_send_event(driver->ni2c_epa, (nevent *)transfer);
            nevent_unlock(ws->event);

            return (naction_handled());
        }
        case EVT_NI2C_TRANSFER_COMPLETED: {
            struct nxio_event_read_pin *    read;
            struct nxio_driver *            driver;
            struct mcp23017_driver *        mcp;
            uint16_t                        gpio;

            read        = (struct nxio_event_read_pin *)ws->event;
            driver      = read->nxio;
            mcp         = (struct mcp23017_driver *)driver;
            gpio        = mcp->regs.gpio;
            *ws->value  = gpio;
            nepa_send_signal(ws->client, EVT_NXIO_COMPLETED);

            return (naction_transit_to(sm, state_idle));
        }
        case EVT_NI2C_ERROR: {
            nepa_send_signal(ws->client, EVT_NXIO_FAILED);

            return (naction_transit_to(sm, state_idle));
        }
        default: {
            nepa_defer_event(&ws->deferred, event);

            return (naction_ignored());
        }
    }
}

/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/
/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of mcp230xx.c
 ******************************************************************************/



