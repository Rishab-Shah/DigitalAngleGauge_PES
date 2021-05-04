/******************************************************************************
* @file:led_tpm_pwm.h
*
* @brief: This files consists of all the logic related to LED. Initialization and
* control of the color.
*
* @author: Rishab Shah
* @date:  01-Mar-2021
* @version 1.0
* @references: dean book - tpm chapter, lecture ppt
*******************************************************************************/
#ifndef LED_TPM_PWM_H_
#define LED_TPM_PWM_H_
/*******************************************************************************
 Header Files
*******************************************************************************/
#include "MKL25Z4.h"
#include "msec_timers.h"
/*******************************************************************************
 Macros
*******************************************************************************/
#define PWM_PERIOD										255
/*******************************************************************************
 Function Prototype
*******************************************************************************/
void init_led_pwm();
void init_tpm();
void rgb_pwm_controller(uint8_t red, uint8_t green, uint8_t blue);
void red_led_on();
void green_led_on();
void blue_led_on();
void all_led_on();
void all_led_off();


#endif /* LED_TPM_PWM_H_ */
