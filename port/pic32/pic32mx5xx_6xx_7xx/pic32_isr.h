/* 
 * File:   pic32_isr.h
 * Author: nenad
 *
 * Created on November 14, 2014, 9:44 PM
 */

#ifndef PIC32_ISR_H
#define	PIC32_ISR_H

#include <stdint.h>

#include "port/cpu.h"
#include "lib/error.h"

#ifdef	__cplusplus
extern "C" {
#endif

#define PIC32_ISR_ENABLE(isr)                                                   \
    *g_iec_reg[(isr)->isr_reg] |=  (0x1u << (isr)->isr_bit)

#define PIC32_ISR_DISABLE(isr)                                                  \
    *g_iec_reg[(isr)->isr_reg] &= ~(0x1u << (isr)->isr_bit)

#define PIC32_ISR_SET(isr)                                                      \
    *g_ifs_reg[(isr)->isr_reg] |=  (0x1u << (isr->isr_bit))

#define PIC32_ISR_CLR(isr)                                                      \
    *g_ifs_reg[(isr)->isr_reg] &= ~(0x1u << (isr)->isr_bit)

#define PIC32_ISR_SET_PRIO(isr, prio)                                           \
    do {                                                                        \
        ncpu_reg ipc = *g_ipc_reg[(isr)->prio_reg];                             \
        ipc &= ~(0x1fu << (isr)->prio_bit);                                     \
        ipc |= ((prio) << (isr)->prio_bit);                                     \
        *g_ipc_reg[(isr)->prio_reg] = ipc;                                      \
    } while (0)

#define PIC32_ISR_GET_PRIO(isr)                                                 \
    (*g_ipc_reg[(isr)->prio_reg] >> (isr)->prio_bit)
    
struct pic32_isr
{
    uint8_t                     isr_reg;
    uint8_t                     isr_bit;
    uint8_t                     prio_reg;
    uint8_t                     prio_bit;
};

extern volatile ncpu_reg * const g_iec_reg[];
extern volatile ncpu_reg * const g_ifs_reg[];
extern volatile ncpu_reg * const g_ipc_reg[];

nerror pic32_isr_request(
    const struct pic32_isr *    isr);

void pic32_isr_release(
    const struct pic32_isr *    isr);

#ifdef	__cplusplus
}
#endif

#endif	/* PIC32_ISR_H */

