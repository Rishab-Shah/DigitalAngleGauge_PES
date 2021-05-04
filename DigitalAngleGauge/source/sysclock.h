/* 
 * sysclock.h - configuration routines for KL25Z system clock
 * 
 * Author Howdy Pierce, howdy.pierce@colorado.edu
 */
#ifndef _SYSCLOCK_H_
#define _SYSCLOCK_H_
/*******************************************************************************
Macros
*******************************************************************************/
#define SYSCLOCK_FREQUENCY 									(24000000) /* 24 Mhz */
/*******************************************************************************
Function declaration
*******************************************************************************/
void init_sysclock();

#endif  // _SYSCLOCK_H_

/* EOF */
