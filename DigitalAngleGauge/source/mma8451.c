/******************************************************************************
* @file : mma8451.h
*
* @brief : This files handles the parameters related to MMA accelerometer initialization
* and prototype declarations
* @author : Rishab Shah
* @date :  30-Apr-2021
* @References : https://github.com/alexander-g-dean/ESF/tree/master/NXP/Code/Chapter_8/I2C-Demo
*******************************************************************************/
/*******************************************************************************
 Header Files
*******************************************************************************/
#include "mma8451.h"
/*******************************************************************************
 Macros
*******************************************************************************/
#define MMA_ADDR 								0x3A
#define REG_XHI 								0x01
#define REG_XLO 								0x02
#define REG_YHI 								0x03
#define REG_YLO 								0x04
#define REG_ZHI									0x05
#define REG_ZLO 								0x06
#define COUNTS_PER_G 							(4096.0)
#define M_PI 									(3.14159265)
#define REG_CTRL1  								0x2A
#define SET										1
/*******************************************************************************
 Global Variables
*******************************************************************************/
extern int cap_tap_detected;
/*******************************************************************************
 Function Definition
*******************************************************************************/
/*******************************************************************************
* @Function Name: init_mma
* @Description: initializes mma8451 sensor. Ensure I2C_Init is called before.
* @input param  : none
* @return: success/failure
*******************************************************************************/
int init_mma()
{
	/* set active mode, 14 bit sampling and 800 Hz ODR value */
	i2c_write_byte(MMA_ADDR, REG_CTRL1, 0x01);
	return 1;
}

/*******************************************************************************
* @Function Name: read_full_xyz
* @Description: reads the acceerometer's x,y,z values
* @input param : none
* @return : none
*******************************************************************************/
void read_full_xyz(acclerometer_parameters_t *accl_param)
{
	int i;
	uint8_t data[6];
	int16_t temp[3];

	i2c_start();
	i2c_read_setup(MMA_ADDR , REG_XHI);

	/* Read 5 bytes in repeated mode */
	for(i = 0 ; i < 5; i++)
	{
		data[i] = i2c_repeated_read(0);
	}

	/* Read last byte ending repeated mode */
	data[i] = i2c_repeated_read(1);

	/* calculate the final value for x,y,z
	 * and store it in the respective buffer */
	for(i = 0; i < 3; i++)
	{
		temp[i] = (int16_t) ((data[2*i]<<8) | data[2*i+1]);
	}

	/* Align the data for 14 bits */
	accl_param->x= temp[0]/4;
	accl_param->y = temp[1]/4;
	accl_param->z = temp[2]/4;

}

/*******************************************************************************
* @Function Name: convert_xyz_to_roll_pitch
* @Description: converts the x,y,z values into roll and pitch
* @input param : none
* @return : none
*******************************************************************************/
void convert_xyz_to_roll_pitch(acclerometer_parameters_t *accl_param,uint8_t update_ref)
{
	//static int count = 0;
	float ax = (accl_param->x)/COUNTS_PER_G;
	float ay = (accl_param->y)/COUNTS_PER_G;
	float az = (accl_param->z)/COUNTS_PER_G;

	accl_param->roll_val= atan2(ay, az)*180/M_PI;
	accl_param->pitch_val = atan2(ax, sqrt(ay*ay + az*az))*180/M_PI;

	if(update_ref == SET)
	{
		accl_param->ref_roll_val = accl_param->roll_val;
	}
}

/* EOF */

