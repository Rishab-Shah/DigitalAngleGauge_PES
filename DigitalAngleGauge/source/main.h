/******************************************************************************
* @file: main.c
*
* @brief: This files consists of the main logic to start the application and
* initialization
*
* @author: Rishab Shah
* @date:  04-04-2021
* @references: Making Embedded Systems - White
* @references: Dean book
* @references: Lecture notes - 1st and 2nd lecture for hexdump
*******************************************************************************/
#ifndef MAIN_H_
#define MAIN_H_
/*******************************************************************************
Header files
*******************************************************************************/
#include <math.h>
#include "sysclock.h"
#include "uart.h"
#include "cbfifo.h"
#include "commandprocessor.h"
#include "msec_timers.h"
#include "switch.h"
#include "touch_slider.h"
#include "led_tpm.h"
#include "i2c.h"
#include "mma8451.h"
#include "state_machine.h"
#include "commandprocessor.h"
#include "test_modules.h"
/*******************************************************************************
Function declaration
*******************************************************************************/
int main(void);

#endif /* MAIN_H_ */
