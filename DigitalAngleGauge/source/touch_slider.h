/******************************************************************************
* @file:led_functions.h
*
* @brief: This files consists of all the logic related to touch sensor init and
* reading of the value
*
* @author: Rishab Shah
* @date:  21-Feb-2021
* @version 1.0
* @references: https://github.com/alexander-g-dean/ESF/tree/master/NXP/Misc/Touch%20Sense
*******************************************************************************/
#ifndef TOUCH_SLIDER_H_
#define TOUCH_SLIDER_H_
/*******************************************************************************
 Header Files
*******************************************************************************/
#include <MKL25Z4.H>
#include <stdio.h>
/*******************************************************************************
 Function Prototype
*******************************************************************************/
void init_touchsensor();
void TSI0_IRQHandler (void);
void start_touch_scanning();

#endif /* TOUCH_SLIDER_H_ */
