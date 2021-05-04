/******************************************************************************
* @file: test_led_tpm.c
*
* @brief: This files consistsof test procedure for led functionality
*
* @author: Rishab Shah
* @date:  03-May-2021
*******************************************************************************/
/*******************************************************************************
 * Header files
*******************************************************************************/
#include <led_tpm_testcase.h>
/*******************************************************************************
 * Macros
*******************************************************************************/
#define	SEC_1									(20*TICK_FREQUENCY)
#define MSEC_50									(1)
/*******************************************************************************
 * Function definition
*******************************************************************************/
void test_led_tpm(uint8_t pwm_period)
{

	/* Check maximum value of red */
	red_led_on();
	reset_timer();
	while(get_timer() < SEC_1);
	printf("LED Max Red value validated\r\n");

	/* Check maximum value of green */
	green_led_on();
	reset_timer();
	while(get_timer() < SEC_1);
	printf("LED Max Green value validated\r\n");

	/* Check maximum value of blue */
	blue_led_on();
	reset_timer();
	while(get_timer() < SEC_1);
	printf("LED Max Blue value validated\r\n");

	/* all led on */
	all_led_on();
	reset_timer();
	while(get_timer() < SEC_1);
	printf("All LED ON validated\r\n");

	/* all led off */
	all_led_off();
	reset_timer();
	while(get_timer() < SEC_1);
	printf("All LED OFF validated\r\n");

	/* white color rise from 0 to 255 and decrease */
	for(int i = 0; i <= pwm_period; i++)
	{
		rgb_pwm_controller(i,i,i);

		reset_timer();
		while(get_timer() < MSEC_50);
	}

	printf("White LED min to max validated\r\n");
	for(int i = pwm_period; i >= 0; i--)
	{

		rgb_pwm_controller(i,i,i);

		reset_timer();
		while(get_timer() < MSEC_50);
	}

	printf("White LED max to min validated\r\n");

	printf("All LED tests complete\r\n");

}
