
#ifndef AB_RTCMC_32768_EOZ9_H_
#define AB_RTCMC_32768_EOZ9_H_

#include <stdint.h>
#include "port/cpu.h"
#include "shared/error.h"

#ifdef	__cplusplus
extern "C" {
#endif

#define rtc_default_init_driver     ab_rtcmc_init_driver
#define rtc_default_term_driver     ab_rtcmc_term_driver
#define rtc_default_set_time        ab_rtcmc_set_time
#define rtc_default_get_time        ab_rtcmc_get_time
#define rtc_default_tick            ab_rtcmc_tick

struct i2c_bus;
struct nrtc_time;

struct ab_rtcmc_config
{
    struct i2c_bus *            bus;
    uint8_t                     id;
};

void ab_rtcmc_init_driver(const void * config);
void ab_rtcmc_term_driver(void);
void ab_rtcmc_set_time(const struct nrtc_time * time);
void ab_rtcmc_get_time(struct nrtc_time * time);
void ab_rtcmc_tick(void);

#ifdef	__cplusplus
}
#endif

#endif /* AB_RTCMC_32768_EOZ9_H_ */