/******************************************************************************
* @file: state_machine.c
*
* @brief: This file consist of all the required functions to handle state machine
* transition for Digital Angle gauge application (DAG)
*
* @author: Rishab Shah
* @date:  02-May-2021
* @references: Lecture notes
*******************************************************************************/
/*******************************************************************************
 Header Files
*******************************************************************************/
#include "state_machine.h"
/*******************************************************************************
 Macros
*******************************************************************************/
#define SET_INDIVIDUAL_VALUE						(15)
#define FULL_ON										(10)
#define FULL_OFF									(0)
#define UPDATE_REF_LEVEL							(1)
#define MAINTAIN_REF_LEVEL							(0)
#define RANDOM_COLOR_UPPER_LIMIT					(5)
#define MSEC_750									(15*TICK_FREQUENCY)
#define MSEC_250									(5*TICK_FREQUENCY)
/*******************************************************************************
 Structures and Enums
*******************************************************************************/
typedef struct rgb_led_color
{
	uint8_t red;
	uint8_t green;
	uint8_t blue;
}rgb_color_t;

typedef enum
{
    ANGLE_0 = 0,
    ANGLE_1 = 30,
	ANGLE_2 = 45,
	ANGLE_3 = 60,
	ANGLE_4 = 90
}eangles_detected;

/*******************************************************************************
 Global variables
*******************************************************************************/
rgb_color_t pwm_led_color = {0,0,0};
acclerometer_parameters_t mma_accl_param = {0,0,0,0,0,0};
extern uint8_t switch_press_detected;
extern int cap_tap_detected;
/*******************************************************************************
 Function Prototype
*******************************************************************************/

/*******************************************************************************
 Function definition
*******************************************************************************/
/*******************************************************************************
* @Function name: handle_calibrate_to_zero
* @Description: In this state the calibration (reference) is performed for DAG
* for reference 0 exact
* @input param: none
* @return: eDigitalAngleGaugeState - next state to be transitioned into
*******************************************************************************/
eDigitalAngleGaugeState handle_calibrate_to_zero()
{
	pwm_led_color.red = 0;
	pwm_led_color.green = 0;
	pwm_led_color.blue = SET_INDIVIDUAL_VALUE;

	rgb_pwm_controller(pwm_led_color.red,pwm_led_color.green,pwm_led_color.blue);

	read_full_xyz(&mma_accl_param);
	convert_xyz_to_roll_pitch(&mma_accl_param,MAINTAIN_REF_LEVEL);

	if(mma_accl_param.roll_val == 0.0)
	{
		convert_xyz_to_roll_pitch(&mma_accl_param,UPDATE_REF_LEVEL);

		if(mma_accl_param.ref_roll_val < 0)
		{
			mma_accl_param.ref_roll_val = mma_accl_param.ref_roll_val*(-1);
		}

		/* set the colors default to blue */
		pwm_led_color.red = 0;
		pwm_led_color.green = SET_INDIVIDUAL_VALUE;
		pwm_led_color.blue = 0;

		/* set the colors to the desired as per the angle under consideration*/
		rgb_pwm_controller(pwm_led_color.red,pwm_led_color.green,pwm_led_color.blue);
		/* wait for 750 msec for detection */
		reset_timer();
		while(get_timer() < MSEC_750);

		/* set the colors default to blue */
		pwm_led_color.red = 0;
		pwm_led_color.green = 0;
		pwm_led_color.blue = 0;
		rgb_pwm_controller(pwm_led_color.red,pwm_led_color.green,pwm_led_color.blue);

		/* wait for 250 msec for detection */
		reset_timer();
		while(get_timer() < MSEC_250);

		printf("Reference value is %d\r\n",(int)mma_accl_param.ref_roll_val);
		printf("Roll value is %d\r\n",(int)mma_accl_param.roll_val);
		printf("Calibration done for ZERO\r\n");

		return ANGLE_DISPLAY_STATE;
	}

	return INIT_STATE;

}
/*******************************************************************************
* @Function name: handle_calibrate_state
* @Description: In this state the calibration (reference) is performed for DAG
* @input param: none
* @return: eDigitalAngleGaugeState - next state to be transitioned into
*******************************************************************************/
eDigitalAngleGaugeState handle_calibrate_state()
{
	pwm_led_color.red = 0;
	pwm_led_color.green = 0;
	pwm_led_color.blue = 0;

	if(cap_tap_detected)
	{
		cap_tap_detected = 0;
		printf("CS::CALIBRATE STATE CAP TAP detected\r\n");

		read_full_xyz(&mma_accl_param);
		convert_xyz_to_roll_pitch(&mma_accl_param,UPDATE_REF_LEVEL);

		if(mma_accl_param.ref_roll_val < 0)
		{
			mma_accl_param.ref_roll_val = mma_accl_param.ref_roll_val*(-1);
		}

		return ANGLE_DISPLAY_STATE;
	}

	rgb_pwm_controller(pwm_led_color.red,pwm_led_color.green,pwm_led_color.blue);

	return CALIBRATE_STATE;
}

/*******************************************************************************
* @Function name: handle_angle_gauge_state
* @Description: In this state the desired angle value is obtained
* @input param: none
* @return: eDigitalAngleGaugeState - next state to be transitioned into
*******************************************************************************/
eDigitalAngleGaugeState handle_angle_gauge_state()
{
	int final_compare_val;

	read_full_xyz(&mma_accl_param);
	convert_xyz_to_roll_pitch(&mma_accl_param,MAINTAIN_REF_LEVEL);

	if(mma_accl_param.roll_val < 0)
	{
		mma_accl_param.roll_val = mma_accl_param.roll_val*(-1);
	}

	/* default color when a particular angle is not there */
	pwm_led_color.red = 0;
	pwm_led_color.green = 0;
	pwm_led_color.blue = 0;
	rgb_pwm_controller(pwm_led_color.red,pwm_led_color.green,pwm_led_color.blue);

	final_compare_val =  (int)(mma_accl_param.roll_val - mma_accl_param.ref_roll_val);
	angle_detect_color_assign(final_compare_val);

	if(switch_press_detected)
	{
		printf("CS::ANGLE_DISPLAY_STATE SWITCH TAP detected\r\n");
		switch_press_detected = 0;
		return CALIBRATE_STATE;
	}

	return ANGLE_DISPLAY_STATE;
}

/*******************************************************************************
* @Function name: angle_detect_color_assign
* @Description: In this state the the color for the angle obtained is displayed
* @input param: none
* @return:
*******************************************************************************/
void angle_detect_color_assign(int color_assigner)
{
	switch(color_assigner)
	{
		case ANGLE_1:
		{
			printf("30 degree detected\r\n");
			pwm_led_color.red = SET_INDIVIDUAL_VALUE;
			pwm_led_color.green = 0;
			pwm_led_color.blue = 0;
			break;
		}
		case ANGLE_2:
		{
			printf("45 degree detected\r\n");
			pwm_led_color.red = 0;
			pwm_led_color.green = SET_INDIVIDUAL_VALUE;
			pwm_led_color.blue = 0;
			break;
		}
		case ANGLE_3:
		{
			printf("60 degree detected\r\n");
			pwm_led_color.red = 0;
			pwm_led_color.green = 0;
			pwm_led_color.blue = SET_INDIVIDUAL_VALUE;
			break;
		}
		case ANGLE_4:
		{
			printf("90 degree detected\r\n");
			pwm_led_color.red = FULL_ON;
			pwm_led_color.green = FULL_ON;
			pwm_led_color.blue = FULL_ON;
			break;
		}
		default:
		{
			break;
		}
	}

	if((pwm_led_color.red > 0)
				|| (pwm_led_color.green > 0 )||
						(pwm_led_color.blue > 0))
	{
		/* set the colors to the desired as per the angle under consideration*/
		rgb_pwm_controller(pwm_led_color.red,pwm_led_color.green,pwm_led_color.blue);
		/* wait for 750 msec for detection */
		reset_timer();
		while(get_timer() < MSEC_750);

		/* set the colors default to blue */
		pwm_led_color.red = 0;
		pwm_led_color.green = 0;
		pwm_led_color.blue = 0;
		rgb_pwm_controller(pwm_led_color.red,pwm_led_color.green,pwm_led_color.blue);

		/* wait for 250 msec for detection */
		reset_timer();
		while(get_timer() < MSEC_250);
	}

}

/* EOF */
