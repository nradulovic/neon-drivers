
#ifndef AB_RTCMC_32768_EOZ9_H_
#define AB_RTCMC_32768_EOZ9_H_

#include <stdint.h>
#include "port/cpu.h"
#include "shared/error.h"

#ifdef	__cplusplus
extern "C" {
#endif

#define rtc_init_driver             ab_rtcmc_init_driver
#define rtc_term_driver             ab_rtcmc_term_driver
#define rtc_set_time                ab_rtcmc_set_time
#define rtc_get_time                ab_rtcmc_get_time
#define rtc_tick_isr                ab_rtcmc_tick_isr

struct i2c_bus;
struct nrtc_time;

struct ab_rtcmc_32768_eoz9_config
{
    struct i2c_bus *            bus;
    uint8_t                     id;
    ncpu_reg                    isr_prio;
};

nerror ab_rtcmc_init_driver(const void * config);
void   ab_rtcmc_term_driver(void);
nerror ab_rtcmc_set_time(const struct nrtc_time * time);
nerror ab_rtcmc_get_time(struct nrtc_time * time);
void   ab_rtcmc_tick_isr(void);

#ifdef	__cplusplus
}
#endif

#endif /* AB_RTCMC_32768_EOZ9_H_ */