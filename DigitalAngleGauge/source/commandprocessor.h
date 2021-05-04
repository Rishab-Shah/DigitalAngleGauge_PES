/******************************************************************************
* @file: commandprocessor.h
*
* @brief: This files consists of the various commands received from the user
*
* @author: Rishab Shah
* @date:  02-04-2021
* @references: Howdy lecture notes
*******************************************************************************/
#ifndef COMMANDPROCESSOR_H_
#define COMMANDPROCESSOR_H_
/*******************************************************************************
Header files
*******************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
/*******************************************************************************
Function declaration
*******************************************************************************/
void acceptinput();
void lexical_analysis(uint8_t *cmdProessor);

#endif /* COMMANDPROCESSOR_H_ */
