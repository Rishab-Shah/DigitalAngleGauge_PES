/******************************************************************************
* @file:msec_timers.h
*
* @brief: This files consists of all the logic related to timers -get,set,reset etc
* from systick
* @Original author: Howdy Pierce
* @author: Rishab Shah
* @date:  06-Mar-2021
* @version 1.0
*******************************************************************************/
#ifndef MSEC_TIMERS_H_
#define MSEC_TIMERS_H_
/*******************************************************************************
 Macros
*******************************************************************************/
#define TICK_FREQUENCY							(50)
/*******************************************************************************
 Global declarations
*******************************************************************************/
typedef uint32_t ticktime_t;
/*******************************************************************************
 Function Prototype
*******************************************************************************/
void delay(uint32_t time_del);
void init_systick();
ticktime_t now();
void reset_timer();
ticktime_t get_timer();

#endif /* MSEC_TIMERS_H_ */
