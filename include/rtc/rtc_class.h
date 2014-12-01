
#ifndef RTC_CLASS_H_
#define RTC_CLASS_H_

#include <stdint.h>
#include "shared/error.h"

struct nrtc_time
{
    uint16_t                    year;
    uint8_t                     month;
    uint8_t                     day;
    uint8_t                     hour;
    uint8_t                     minute;
    uint8_t                     second;
};

struct nrtc_class
{
    void                      (* init)(void *);
    void                      (* term)(void);
    void                      (* set_time)(const struct nrtc_time *);
    void                      (* get_time)(struct nrtc_time *);
    void                      (* tick_isr)(void);
};

nerror rtc_init_driver(const void * config);
void   rtc_term_driver(void);
nerror rtc_set_time(const struct nrtc_time * time);
nerror rtc_get_time(struct nrtc_time * time);
void   rtc_tick_isr(void);

#endif /* RTC_CLASS_H_ */