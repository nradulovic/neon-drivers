
#include <xc.h>

#include "plat/isr.h"
#include "pic32_isr.h"
#include "lib/debug.h"
#include "port/bitop.h"
#include "lib/module.h"

static const NMODULE_INFO_CREATE("ISR PIC32mx5xx/6xx/7xx driver", "Nenad Radulovic");

static uint32_t g_isr_bitmap[3];

volatile ncpu_reg * const g_iec_reg[] =
{
#if defined(ISR_REG_0)
    &IEC0,
#endif
#if defined(ISR_REG_1)
    &IEC1,
#endif
#if defined(ISR_REG_2)
    &IEC2,
#endif
};

volatile ncpu_reg * const g_ifs_reg[] =
{
#if defined(ISR_REG_0)
    &IFS0,
#endif
#if defined(ISR_REG_1)
    &IFS1,
#endif
#if defined(ISR_REG_2)
    &IFS2,
#endif
};

volatile ncpu_reg * const g_ipc_reg[] =
{
#if defined(ISR_PRIO_0)
    &IPC0,
#endif
#if defined(ISR_PRIO_1)
    &IPC1,
#endif
#if defined(ISR_PRIO_2)
    &IPC2,
#endif
#if defined(ISR_PRIO_3)
    &IPC3,
#endif
#if defined(ISR_PRIO_4)
    &IPC4,
#endif
#if defined(ISR_PRIO_5)
    &IPC5,
#endif
#if defined(ISR_PRIO_6)
    &IPC6,
#endif
#if defined(ISR_PRIO_7)
    &IPC7,
#endif
#if defined(ISR_PRIO_8)
    &IPC8,
#endif
#if defined(ISR_PRIO_9)
    &IPC9,
#endif
#if defined(ISR_PRIO_10)
    &IPC10,
#endif
#if defined(ISR_PRIO_11)
    &IPC11,
#endif
#if defined(ISR_PRIO_12)
    &IPC12,
#endif
};

nerror pic32_isr_request(const struct pic32_isr * isr)
{
    NREQUIRE_INTERNAL(NAPI_RANGE, isr->isr_bit > NARRAY_DIMENSION(g_iec_reg));
    
    if (g_isr_bitmap[isr->isr_reg] & (0x1u << isr->isr_bit)) {
        return (NERROR_NO_RESOURCE);
    } else {
        g_isr_bitmap[isr->isr_reg] |= (0x1u << isr->isr_bit);

        return (NERROR_NONE);
    }
}

void pic32_isr_release(const struct pic32_isr * isr)
{
    g_isr_bitmap[isr->isr_reg] &= ~(0x1u << isr->isr_bit);
}