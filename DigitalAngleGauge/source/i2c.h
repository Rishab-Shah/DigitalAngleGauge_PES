/******************************************************************************
* @file : i2c.h
*
* @brief : This files consists of functions related to i2c communication
*
* @author : Rishab Shah
* @date :  01-Mar-2021
* @version : 1.0
* @references : dean book - Serial communication chapter
*******************************************************************************/
#ifndef I2C_H_
#define I2C_H_
/*******************************************************************************
 Header Files
*******************************************************************************/
#include <stdint.h>
/*******************************************************************************
 Function Prototype
*******************************************************************************/
void init_i2c(void);
void i2c_start(void);
void i2c_read_setup(uint8_t dev, uint8_t address);
uint8_t i2c_repeated_read(uint8_t);
//uint8_t i2c_read_byte(uint8_t dev, uint8_t address);
void i2c_write_byte(uint8_t dev, uint8_t address, uint8_t data);

#endif /* I2C_H_ */
