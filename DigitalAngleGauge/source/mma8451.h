/******************************************************************************
* @file : mma8451.h
*
* @brief : This files handles the parameters related to MMA accelerometer initialization
* and prototype declarations
* @author : Rishab Shah
* @date :  30-Apr-2021
* @References : https://github.com/alexander-g-dean/ESF/tree/master/NXP/Code/Chapter_8/I2C-Demo
*******************************************************************************/
#ifndef MMA8451_H_
#define MMA8451_H_
/*******************************************************************************
 Header Files
*******************************************************************************/
#include <MKL25Z4.H>
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include "i2c.h"
/*******************************************************************************
 Macros
*******************************************************************************/
#define REG_WHOAMI 						0x0D
#define WHOAMI 							0x1A
#define REG_CTRL4  						0x2D

typedef struct accl_param
{
	int x;
	int y;
	int z;
	float roll_val;
	float pitch_val;
	float ref_roll_val;
}acclerometer_parameters_t;

/*******************************************************************************
 Header Files
*******************************************************************************/
int init_mma(void);
void read_full_xyz(acclerometer_parameters_t *acc);
void convert_xyz_to_roll_pitch(acclerometer_parameters_t *accl_param,uint8_t update_ref);


#endif /* MMA8451_H_ */
