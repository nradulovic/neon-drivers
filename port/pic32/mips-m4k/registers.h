/* 
 * File:   registers.h
 * Author: nenad
 *
 * Created on November 15, 2014, 1:17 AM
 */

#ifndef REGISTERS_H
#define	REGISTERS_H

#ifdef	__cplusplus
extern "C" {
#endif

#define REGISTER_CLR(address, mask)                                             \
    ((volatile unsigned int *)(address))[1] = mask

#define REGISTER_SET(address, mask)                                             \
    ((volatile unsigned int *)(address))[2] = mask

#define REGISTER_INV(address, mask)                                             \
    ((volatile unsigned int *)(address))[3] = mask

#ifdef	__cplusplus
}
#endif

#endif	/* REGISTERS_H */

