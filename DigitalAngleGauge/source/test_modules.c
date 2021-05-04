/******************************************************************************
* @file: test_led_tpm.c
*
* @brief: This files consists of test procedure for all modules
*
* @author: Rishab Shah
* @date:  03-May-2021
*******************************************************************************/
/*******************************************************************************
 * Header files
*******************************************************************************/
#include "test_modules.h"
/*******************************************************************************
 Macros
*******************************************************************************/
#define SEC_1									(20*TICK_FREQUENCY)
/*******************************************************************************
 * Function declaration
*******************************************************************************/
static void test_angle_gauge_color();
/*******************************************************************************
 * Function definition
*******************************************************************************/
/*******************************************************************************
* @Function Name: main
* @Description: Starts the application
* @input param: None
* @return: None
*******************************************************************************/
void test_modules()
{
	printf("Testing started\r\n");

	test_cbfifo();

	test_led_tpm(PWM_PERIOD);

	test_angle_gauge_color();

	printf("Testing complete\r\n");
}

/*******************************************************************************
* @Function Name: test_angle_gauge_color
* @Description: Tests the angle detection feature for the color indicated
* @input param: None
* @return: None
*******************************************************************************/
void test_angle_gauge_color()
{
	/* angle 0 test case */
	angle_detect_color_assign(0);
	reset_timer();
	while(get_timer() < SEC_1);
	printf("Angle 0 not detected as not present. No color lit.\r\n");

	/* angle 30 test case */
	angle_detect_color_assign(30);
	reset_timer();
	while(get_timer() < SEC_1);
	printf("Angle 30 detected. Red color lit.\r\n");

	/* angle 45 test case */
	angle_detect_color_assign(45);
	reset_timer();
	while(get_timer() < SEC_1);
	printf("Angle 45 detected. Green color lit.\r\n");

	/* angle 60 test case */
	angle_detect_color_assign(60);
	reset_timer();
	while(get_timer() < SEC_1);
	printf("Angle 60 detected. Blue color lit.\r\n");

	/* angle 90 test case */
	angle_detect_color_assign(90);
	reset_timer();
	while(get_timer() < SEC_1);
	printf("Angle 90 detected. White color lit.\r\n");

	/* angle -1 test case */
	angle_detect_color_assign(-1);
	reset_timer();
	while(get_timer() < SEC_1);
	printf("Invalid angle. No color lit.\r\n");

}

