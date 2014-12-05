/* 
 * File:   clock.h
 * Author: nenad
 *
 * Created on 05 December 2014, 01:25
 */

#ifndef PERIPH_CLOCK_H
#define	PERIPH_CLOCK_H

#ifdef	__cplusplus
extern "C" {
#endif

/* FIXME: Set this value to correct one */
#define CLOCK_TYPE_PB           1
#define CLOCK_TYPE_CPU          2

struct nclock
{
    uint32_t                    frequency;
};


#ifdef	__cplusplus
}
#endif

#endif	/* PERIPH_CLOCK_H */

