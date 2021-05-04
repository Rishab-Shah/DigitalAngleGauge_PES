/******************************************************************************
* @file: uart.c
*
* @brief: This files consists of the logic used to implement uart
*
* @author: Rishab Shah
* @date:  02-03-2021
* @version 1.1 : Implemented corrections suggested
* 1. Added an init function to initialize the queue
* 2. Corrected function header comments
* @references: Alexander Dean - UART code
*******************************************************************************/
#ifndef UART_H
#define UART_H
/*******************************************************************************
Header files
*******************************************************************************/
#include <MKL25Z4.H>
#include <stdint.h>
#include <stdio.h>
#include "cbfifo.h"
#include "sysclock.h"
/*******************************************************************************
Function declaration
*******************************************************************************/
void init_UART0();
int __sys_write(int handle, char *buf, int size);
int __sys_readc(void);

#endif /* UART_H */

/* EOF */
