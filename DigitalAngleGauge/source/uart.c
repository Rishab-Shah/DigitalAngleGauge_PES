/******************************************************************************
* @file: uart.c
*
* @brief: This files consists of the logic used to implement uart
*
* @author: Rishab Shah
* @date:  02-May-2021
* @references: Alexander Dean - UART code
* https://github.com/alexander-g-dean/ESF/tree/master/NXP/Code/Chapter_8/Serial-Demo
*******************************************************************************/
/*******************************************************************************
Header files
*******************************************************************************/
#include "uart.h"
/*******************************************************************************
Macros
*******************************************************************************/
#define UART_OVERSAMPLE_RATE 						(16)
#define BAUD_RATE									(38400)		/* sets the baud rate */
#define STOP_BIT									(1)			/* Ensures 2 stop bits */
#define PARITY										(0)			/* No parity */
#define DATA_SIZE									(0)			/* Data size is 8 */
/*******************************************************************************
Global variables
*******************************************************************************/
static CB_t cbfifo_rx;
static CB_t cbfifo_tx;
/*******************************************************************************
Function declaration
*******************************************************************************/
/* BEGIN - UART0 Device Driver
	Code created by Shannon Strutz
	Date : 5/7/2014
	Licensed under CC BY-NC-SA 3.0
	http://creativecommons.org/licenses/by-nc-sa/3.0/
	Modified by Alex Dean 9/13/2016
*/
/*******************************************************************************
* @Function Name: __sys_write
* @Description: enqueues the tx buffer to display data on the terminal
* @input param 1: the file handler
* @input param 2: the data to be enqueued
* @input param 3: the number of bytes to be transmitted
* @return: success/failure status
*******************************************************************************/
int __sys_write(int handle, char *buf, int size)
{
	if(buf == NULL)
	{
		return -1;
	}

	/* Check for the queue to be empty */
	while(cbfifo_full(&cbfifo_tx))
	{
		;
	}

	cbfifo_enqueue(&cbfifo_tx,buf,size);

	/* enable the transmit interrupt */
	if(!(UART0->C2 & UART0_C2_TIE_MASK))
	{
		UART0->C2 |= UART0_C2_TIE(1);
	}

	return 0;
}

/*******************************************************************************
* @Function Name: __sys_readc
* @Description: dequeues the rx buffer to fetch data from the terminal
* @input param: None
* @return: the byte which is dequeued if present in the queue, otherwise a default
* value of -1 when queue is empty
*******************************************************************************/
int __sys_readc(void)
{
	char data_byte;

	/* dequeue one byte at a time */
	if(cbfifo_dequeue(&cbfifo_rx,&data_byte,1) != 1)
	{
		return -1;
	}
	else
	{
		return data_byte;
	}
}


/*******************************************************************************
* @Function Name: init_UART0
* @Description: Initialises the UART- 0 as pe rthe macros used
* @input param: none
* @return: none
* @reference: Code listing 8.8, p. 231
*******************************************************************************/
void init_UART0()
{
	uint16_t sbr;
	
	/* Enable clock gating for UART0 and Port A */
	SIM->SCGC4 |= SIM_SCGC4_UART0_MASK; 										
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;

	/* Make sure transmitter and receiver are disabled before initialization */
	UART0->C2 &= ~UART0_C2_TE_MASK & ~UART0_C2_RE_MASK;
	
	/* Set UART clock to 24 MHz clock */
	SIM->SOPT2 |= SIM_SOPT2_UART0SRC(1);

	/* Set pins to UART0 Rx and Tx */
	PORTA->PCR[1] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(2); // Rx
	PORTA->PCR[2] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(2); // Tx
	
	/* Set baud rate and over-sampling ratio */
	sbr = (uint16_t)((SYSCLOCK_FREQUENCY)/(BAUD_RATE * UART_OVERSAMPLE_RATE));
	UART0->BDH &= ~UART0_BDH_SBR_MASK;
	UART0->BDH |= UART0_BDH_SBR(sbr>>8);
	UART0->BDL = UART0_BDL_SBR(sbr);
	UART0->C4 |= UART0_C4_OSR(UART_OVERSAMPLE_RATE-1);				

	/* select two stop bit */
	UART0->BDH |= UART0_BDH_RXEDGIE(0) | UART0_BDH_SBNS(STOP_BIT) | UART0_BDH_LBKDIE(0);

	/* Don't enable loopback mode, use 8 data bit mode, don't use parity */
	UART0->C1 = UART0_C1_LOOPS(0) | UART0_C1_M(DATA_SIZE) | UART0_C1_PE(PARITY) | UART0_C1_PT(0);

	/* Don't invert transmit data, don't enable interrupts for errors */
	UART0->C3 = UART0_C3_TXINV(0) | UART0_C3_ORIE(0)| UART0_C3_NEIE(0)
			| UART0_C3_FEIE(0) | UART0_C3_PEIE(0);

	/* Clear error flags */
	UART0->S1 = UART0_S1_OR(1) | UART0_S1_NF(1) | UART0_S1_FE(1) | UART0_S1_PF(1);

	/* Send LSB first, do not invert received data */
	UART0->S2 = UART0_S2_MSBF(0) | UART0_S2_RXINV(0);

    /* Initialize both the separate tx and rx queues */
    cbfifo_init(&cbfifo_tx,CBFIFO_SIZE);
    cbfifo_init(&cbfifo_rx,CBFIFO_SIZE);

	/* Enable UART-0 interrupt */
	NVIC_EnableIRQ(UART0_IRQn);

	/* Enable receive interrupts but not transmit interrupts yet */
	UART0->C2 |= UART_C2_RIE(1);

	/* Enable UART receiver and transmitter */
	UART0->C2 |= UART0_C2_RE(1) | UART0_C2_TE(1);

}

/* END - UART0 Device Driver 
	Code created by Shannon Strutz
	Date : 5/7/2014
	Licensed under CC BY-NC-SA 3.0
	http://creativecommons.org/licenses/by-nc-sa/3.0/
	Modified by Alex Dean 9/13/2016
*/
/*******************************************************************************
* @Function Name: UART0_IRQHandler
* @Description: Handles the UART-0 interrupt received
* @input param: none
* @return: none
* @reference: UART0 IRQ Handler. Listing 8.12 on p. 235
*******************************************************************************/
void UART0_IRQHandler(void)
{
	uint8_t ch;
	
	/* Check for any errors in the interrupt sequence */
	if (UART0->S1 & (UART_S1_OR_MASK |UART_S1_NF_MASK |
		UART_S1_FE_MASK | UART_S1_PF_MASK))
	{
		/* Clear error flags */
		UART0->S1 = UART0_S1_OR(1) | UART0_S1_NF(1) | UART0_S1_FE(1) | UART0_S1_PF(1);

		/* read the data register to clear RDRF */
		ch = UART0->D;
	}

	/* Receiver interrupt checked */
	if (UART0->S1 & UART0_S1_RDRF_MASK)
	{
		/* received a character from the interrupt */
		ch = UART0->D;

		/* Store it in the buffer */
		cbfifo_enqueue(&cbfifo_rx,&ch,1);
	}

	/* Transmitter interrupt enabled */
	if ( (UART0->C2 & UART0_C2_TIE_MASK) &&
			(UART0->S1 & UART0_S1_TDRE_MASK) )
	{
		if(cbfifo_dequeue(&cbfifo_tx,&ch,1))
		{
			/* transmit the byte */
			UART0->D = ch;
		}
		else
		{
			/* queue is empty so disable transmitter interrupt */
			UART0->C2 &= ~UART0_C2_TIE_MASK;
		}
	}
}

/* EOF */
