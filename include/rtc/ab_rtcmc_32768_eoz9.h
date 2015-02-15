
#ifndef AB_RTCMC_32768_EOZ9_H_
#define AB_RTCMC_32768_EOZ9_H_

#include <stdint.h>

#ifdef  __cplusplus
extern "C" {
#endif

#define rtc_default_init_driver     ab_rtcmc_init_driver
#define rtc_default_term_driver     ab_rtcmc_term_driver
#define rtc_default_set_time        ab_rtcmc_set_time
#define rtc_default_get_time        ab_rtcmc_get_time
#define rtc_default_tick            ab_rtcmc_tick
#define rtc_default_state           ab_rtcmc_state

struct nrtc_time;
struct nrtc_state;

struct ab_rtcmc_config
{
    uint8_t                     bus_id;
    uint8_t                     device_id;
};



void ab_rtcmc_init_driver(
    const void *                config);



void ab_rtcmc_term_driver(void);



void ab_rtcmc_tick(void);



void ab_rtcmc_set_time(
    const struct nrtc_time *    time);



const struct nrtc_time  * ab_rtcmc_get_time(void);



const struct nrtc_state * ab_rtcmc_state(void);

#ifdef  __cplusplus
}
#endif

#endif /* AB_RTCMC_32768_EOZ9_H_ */