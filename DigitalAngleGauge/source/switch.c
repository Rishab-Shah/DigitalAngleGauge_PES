/******************************************************************************
* @file: switch.h
*
* @brief: This files is used for handling the gpio interrupt from the switch
* for the PTD3
*
* @author: Rishab Shah
* @date:  02-May-2021
* @version 1.0
* @reference : GPIO lecture notes
* Dean's PPT (Interrupt)
*******************************************************************************/
/*******************************************************************************
 Header Files
*******************************************************************************/
#include "switch.h"
/*******************************************************************************
 Macros
*******************************************************************************/
#define SWITCH_GPIO_PORT 						GPIOD
#define SWITCH_PIN								3
#define SWITCH_PIN_CTRL_REG						PORTD->PCR[SWITCH_PIN] /* PORTD PCR 3*/
#define SWITCH_SCGC5_MASK						SIM_SCGC5_PORTD_MASK
#define SWITCH_DATA_DIR_REG 					SWITCH_GPIO_PORT->PDDR
#define SWITCH_DATA_IN_REG						SWITCH_GPIO_PORT->PDIR
#define SWITCH_IFSR_REG							PORTD->ISFR
/*******************************************************************************
 Global Variables
*******************************************************************************/
uint8_t switch_press_detected;
/*******************************************************************************
 Function definition
*******************************************************************************/
/*******************************************************************************
* @Function name: init_switch
* @Description: Initializes the switch module for interrupt based operation
* @input param: none
* @return: none
*******************************************************************************/
void init_switch()
{
	/* Enable clock source to the pin */
	SIM->SCGC5 |= SWITCH_SCGC5_MASK;

	/* Setting up port multiplexing for GPIO */
	SWITCH_PIN_CTRL_REG &= ~PORT_PCR_MUX_MASK;
	SWITCH_PIN_CTRL_REG |= PORT_PCR_MUX(1);

	/* Enable the pull select and pull enable */
	SWITCH_PIN_CTRL_REG |= PORT_PCR_PE(1) | PORT_PCR_PS(1);

	/* Enable the interrupt on any edge */
	SWITCH_PIN_CTRL_REG |= PORT_PCR_IRQC(11);

	/* Set direction to input */
	SWITCH_DATA_DIR_REG &= ~(1 << SWITCH_PIN);

	/* Set the priroity of switch */
	NVIC_EnableIRQ(PORTD_IRQn);

}
/*******************************************************************************
* @Function name: PORTD_IRQHandler
* @Description: Handles the interrupt received on the POrt D
* @input param: none
* @return: none
*******************************************************************************/
void PORTD_IRQHandler(void)
{
	switch_press_detected = 0;

	if ((SWITCH_IFSR_REG & (1 << SWITCH_PIN)))
	{
		/* clearing on purpose to create a debounce condition */
		switch_press_detected = 0;

		/* reading again to confirm that the
		 * switch was not pressed accidentally */
		if(	(SWITCH_DATA_IN_REG & (1 << SWITCH_PIN)) == 0)
		{
			/* Set the switch flag as the
			 * switch is pressed in reality */
			switch_press_detected = 1;
		}
	}

	/* Clear the switch pin flag*/
	SWITCH_IFSR_REG |= (1 << SWITCH_PIN);
}
