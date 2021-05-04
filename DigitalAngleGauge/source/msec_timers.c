/******************************************************************************
* @file:msec_timers.c
*
* @brief: This files consists of all the logic related to generation of approx.
* 1 msec delay.
*
* @author: Rishab Shah
* @date:  05-Mar-2021
* @version 1.0
* @references: referred lecture notes
*******************************************************************************/
/*******************************************************************************
 Header Files
*******************************************************************************/
#include "MKL25Z4.h"
#include "core_cm0plus.h" //confirm?
#include "msec_timers.h"
/*******************************************************************************
 Macros
*******************************************************************************/
#define TICKS_50MSEC							(1200000)			//24 MhZ
/*******************************************************************************
 Global Variables
*******************************************************************************/
ticktime_t g_tickcount = 0;
ticktime_t g_program_start;
ticktime_t g_timer_start;
/*******************************************************************************
 Function definition
*******************************************************************************/
/*******************************************************************************
* @Function name: time_msec
* @Description: returns the time in msec
* @input param: none
* @return: None
*******************************************************************************/
static ticktime_t time_msec()
{
	/* returns number of milliseconds elapsed */
	return g_tickcount*TICK_FREQUENCY;
}

/*******************************************************************************
* @Function name: init_systick
* @Description: delay as per the count received in msec
* @input param: none
* @return: None
*******************************************************************************/
void init_systick()
{
	/* Disable SysTick during setup */
	SysTick->CTRL = 0;

	/* The value to reload when the counter reaches 0 */
	SysTick->LOAD = TICKS_50MSEC;

	/* Set NVIC Priority for the timer */
	NVIC_SetPriority(SysTick_IRQn,3);

	/* The val register is initialized with 0 */
	SysTick->VAL = 0;

	/* Enable SysTick with core (processor) clock and using interrupt */
	SysTick->CTRL |= (SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_CLKSOURCE_Msk
					| SysTick_CTRL_TICKINT_Msk);

	g_program_start = g_timer_start = time_msec();
}

/*******************************************************************************
* @Function name: reset_timer
* @Description: reset timer gives the reference time to generate a delay
* @input param: none
* @return: None
*******************************************************************************/
void reset_timer()
{
	g_timer_start = time_msec();
}

/*******************************************************************************
* @Function name: now
* @Description: gives the current time since start of the program
* @input param: msec uint32_t type
* @return: None
*******************************************************************************/
ticktime_t now()
{
	return time_msec() - g_program_start;
}

/*******************************************************************************
* @Function name: get_timer
* @Description: fetches the time elapsed since last call to rest_timer
* @input param: None
* @return: None
*******************************************************************************/
ticktime_t get_timer()
{
	return time_msec() - g_timer_start;
}

/*******************************************************************************
* @Function name: SysTick_Handler
* @Description: systick handlerfunction
* @input param: None
* @return: None
*******************************************************************************/
void SysTick_Handler()
{
	g_tickcount++;
}

/* EOF */

