
#ifndef RTC_CLASS_H_
#define RTC_CLASS_H_

#include <stdint.h>
#include "base/shared/error.h"

enum nrtc_time_state
{
    RTC_TIME_SET                = (0x1u << 0),
    RTC_TIME_VALID              = (0x1u << 1),
    RTC_TIME_SET_AND_VALID      = RTC_TIME_SET | RTC_TIME_VALID
};

enum nrtc_device_state
{
    RTC_DEVICE_OK,
    RTC_DEVICE_INACTIVE,
    RTC_DEVICE_FAILURE,
    RTC_DEVICE_NO_COMM
};

struct nrtc_time
{
    uint16_t                    year;
    uint8_t                     month;
    uint8_t                     day;
    uint8_t                     hour;
    uint8_t                     minute;
    uint8_t                     second;
};

struct nrtc_state
{
    enum nrtc_time_state        time;
    nerror                      device;
};

struct nrtc_class
{
    void                     (* init)(void *);
    void                     (* term)(void);
    void                     (* set_time)(const struct nrtc_time *);
    void                     (* get_time)(struct nrtc_time *);
    void                     (* tick_isr)(void);
};

#endif /* RTC_CLASS_H_ */
