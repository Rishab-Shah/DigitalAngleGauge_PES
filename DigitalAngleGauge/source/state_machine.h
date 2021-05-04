/******************************************************************************
* @file: state_machine.h
*
* @brief: This files consists of functions for the various state transitions
* for the digital angle gauge application
*
* @author: Rishab Shah
* @date:  02-May-2021
*******************************************************************************/
#ifndef STATE_MACHINE_H_
#define STATE_MACHINE_H_
/*******************************************************************************
 Header Files
*******************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include "MKL25Z4.h"
#include "mma8451.h"
#include "led_tpm.h"
#include "msec_timers.h"
#include "commandprocessor.h"
/*******************************************************************************
 Structures
*******************************************************************************/
typedef enum
{
	INIT_STATE = 0,					/* In this state the calibration for zero is performed */
    CALIBRATE_STATE,				/* In this state the calibration (reference) is performed */
    ANGLE_DISPLAY_STATE 			/* In this state the angles are displayed */
}eDigitalAngleGaugeState;
/*******************************************************************************
 Function declarations
*******************************************************************************/
eDigitalAngleGaugeState handle_calibrate_to_zero();
eDigitalAngleGaugeState handle_calibrate_state();
eDigitalAngleGaugeState handle_angle_gauge_state();
void angle_detect_color_assign(int color_assigner);
#endif /* STATE_MACHINE_H_ */
