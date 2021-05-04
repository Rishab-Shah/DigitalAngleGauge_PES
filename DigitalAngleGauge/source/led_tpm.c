/******************************************************************************
* @file: led_tpm.c
*
* @brief: This files consists of all the logic related to LED Initialization and
* control of the color using pwm.
*
* @author: Rishab Shah
* @date:  02-Mar-2021
* @references: PPT referred for MACRO usage, data sheet and dean book - tpm chapter
*******************************************************************************/
/*******************************************************************************
 * Header files
*******************************************************************************/
#include "led_tpm.h"
/*******************************************************************************
 Macros
*******************************************************************************/
/* TPM related initialization */
#define RED_LED_CHANNEL							0
#define	TPM2_RED_CTRL_CHANNEL					TPM2->CONTROLS[RED_LED_CHANNEL]
#define GREEN_LED_CHANNEL						1
#define	TPM2_GREEN_CTRL_CHANNEL					TPM2->CONTROLS[GREEN_LED_CHANNEL]
#define BLUE_LED_CHANNEL						1
#define	TPM0_BLUE_CTRL_CHANNEL					TPM0->CONTROLS[BLUE_LED_CHANNEL]

/* RED LED configuration */
#define RED_LED_GPIO_PORT 						GPIOB
#define RED_LED_PIN								18
#define RED_LED_SCGC5_MASK						SIM_SCGC5_PORTB_MASK
#define RED_LED_PIN_CTRL_REG					PORTB->PCR[RED_LED_PIN]
#define RED_LED_DATA_DIR_REG 					RED_LED_GPIO_PORT->PDDR
#define RED_LED_ON								RED_LED_GPIO_PORT->PCOR
#define RED_LED_OFF								RED_LED_GPIO_PORT->PSOR
#define RED_LED_TOGGLE							RED_LED_GPIO_PORT->PTOR

/* GREEN LED configuration */
#define GREEN_LED_GPIO_PORT 					GPIOB
#define GREEN_LED_PIN							19
#define GREEN_LED_SCGC5_MASK					SIM_SCGC5_PORTB_MASK
#define GREEN_LED_PIN_CTRL_REG					PORTB->PCR[GREEN_LED_PIN]
#define GREEN_LED_DATA_DIR_REG 					GREEN_LED_GPIO_PORT->PDDR
#define GREEN_LED_ON							GREEN_LED_GPIO_PORT->PCOR
#define GREEN_LED_OFF							GREEN_LED_GPIO_PORT->PSOR
#define GREEN_LED_TOGGLE						GREEN_LED_GPIO_PORT->PTOR

/* BLUE LED configuration */
#define BLUE_LED_GPIO_PORT 						GPIOD
#define BLUE_LED_PIN							1
#define BLUE_LED_SCGC5_MASK						SIM_SCGC5_PORTD_MASK
#define BLUE_LED_PIN_CTRL_REG					PORTD->PCR[BLUE_LED_PIN]
#define BLUE_LED_DATA_DIR_REG 					BLUE_LED_GPIO_PORT->PDDR
#define BLUE_LED_ON								BLUE_LED_GPIO_PORT->PCOR
#define BLUE_LED_OFF							BLUE_LED_GPIO_PORT->PSOR
#define BLUE_LED_TOGGLE							BLUE_LED_GPIO_PORT->PTOR

#define RED_LED_PWM_ON()						TPM2_RED_CTRL_CHANNEL.CnV = PWM_PERIOD;
#define RED_LED_PWM_OFF()						TPM2_RED_CTRL_CHANNEL.CnV = 0;

#define GREEN_LED_PWM_ON()						TPM2_GREEN_CTRL_CHANNEL.CnV = PWM_PERIOD;
#define GREEN_LED_PWM_OFF()						TPM2_GREEN_CTRL_CHANNEL.CnV = 0;

#define BLUE_LED_PWM_ON()						TPM0_BLUE_CTRL_CHANNEL.CnV = PWM_PERIOD;
#define BLUE_LED_PWM_OFF()						TPM0_BLUE_CTRL_CHANNEL.CnV = 0;
/*******************************************************************************
 Function Prototype
*******************************************************************************/
static void init_tpm0(uint32_t period);
static void init_tpm2(uint32_t period);
static void init_led(uint8_t led_channel);
/*******************************************************************************
 * Function definition
*******************************************************************************/
/*******************************************************************************
* @Function name: init_led
* @Description: Prepared initialization for all the RGB LEDs and their default
* states for PWM
* @input param: None
* @return: None
*******************************************************************************/
void init_led_pwm()
{
	/* Send the Clock to the GPIO Port for Red, Green and Blue LED */
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTD_MASK;

	/* Set the appropriate mux for the pwm functionality */
	init_led(RED_LED_PIN);
	init_led(GREEN_LED_PIN);
	init_led(BLUE_LED_PIN);
}

/*******************************************************************************
* @Function name: init_tpm
* @Description: Initialization for the TPM channels
* @input param: None
* @return: none
*******************************************************************************/
void init_tpm()
{
	/* Configure TPM-0 and TPM-2 */
	SIM->SCGC6 |= (SIM_SCGC6_TPM0_MASK | SIM_SCGC6_TPM2_MASK);

	/* Set clock source for tpm: 24 MHz */
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);

	/* Init tpm - 0 specific initialization */
	init_tpm0(PWM_PERIOD);

	/* Init tpm - 2 specific initialization */
	init_tpm2(PWM_PERIOD);
}

/*******************************************************************************
* @Function name: init_tpm0
* @Description: Initialization for the TPM channel - 0 (BLUE)
* @input param: time preiod for the brightness adjustment
* @return: none
*******************************************************************************/
static void init_tpm0(uint32_t period)
{
	/* Load the counter and mod */
	TPM0->MOD = period;

	/* Set TPM count direction to up with a divide by 2 pre-scaler */
	TPM0->SC = TPM_SC_PS(1);

	/* Continue operation in debug mode */
	TPM0->CONF |= TPM_CONF_DBGMODE(3);

	/* Set channel 1(BLUE) to edge-aligned low-true PWM */
	TPM0_BLUE_CTRL_CHANNEL.CnSC = (TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK);

	/* Set initial duty cycle to 0  (OFF)*/
	TPM0_BLUE_CTRL_CHANNEL.CnV = 0;

	/* Start TPM-0 */
	TPM0->SC |= TPM_SC_CMOD(1);
}

/*******************************************************************************
* @Function name: init_tpm2
* @Description: Initialization for the TPM channel - 2 (RED,GREEN)
* @input param: time period for the brightness adjustment
* @return: none
*******************************************************************************/
static void init_tpm2(uint32_t period)
{
	/* Load the counter and mod */
	TPM2->MOD = period;

	/* Set TPM count direction to up with a divide by 2 pre-scaler */
	TPM2->SC =  TPM_SC_PS(1);

	/* Continue operation in debug mode */
	TPM2->CONF |= TPM_CONF_DBGMODE(3);

	/* Set channel 0 (RED) to edge-aligned low-true PWM */
	TPM2_RED_CTRL_CHANNEL.CnSC = (TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK);

	/* Set initial duty cycle to 0  (OFF)*/
	TPM2_RED_CTRL_CHANNEL.CnV = 0;

	/* Set channel 1(GREEN) to edge-aligned low-true PWM */
	TPM2_GREEN_CTRL_CHANNEL.CnSC = (TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK);

	/* Set initial duty cycle to 0 (OFF) */
	TPM2_GREEN_CTRL_CHANNEL.CnV = 0;

	/* Start TPM-2 */
	TPM2->SC |= TPM_SC_CMOD(1);
}

/*******************************************************************************
* @Function name: rgb_pwm_controller
* @Description: sets the led color value in pwn mode
* @input param: led color pwm values
* @return: none
*******************************************************************************/
void rgb_pwm_controller(uint8_t red_pwm, uint8_t green_pwm, uint8_t blue_pwm)
{
	TPM2_RED_CTRL_CHANNEL.CnV = red_pwm;
	TPM2_GREEN_CTRL_CHANNEL.CnV = green_pwm;
	TPM0_BLUE_CTRL_CHANNEL.CnV = blue_pwm;
}

/*******************************************************************************
* @Function name: init_led
* @Description: sets the appropriate mux channel for the LED
* @input param: led color pwm values
* @return: none
*******************************************************************************/
static void init_led(uint8_t led_channel)
{
	switch(led_channel)
	{
		case RED_LED_PIN:
		{
			/* Set up Port Multiplexing as it is needed
			 * for PWM feature  for RED LED */
			RED_LED_PIN_CTRL_REG &= ~PORT_PCR_MUX_MASK;
			RED_LED_PIN_CTRL_REG |= PORT_PCR_MUX(3);
			break;
		}
		case GREEN_LED_PIN:
		{
			/* Set up Port Multiplexing as it is needed
			 * for PWM feature  for GREEN LED */
			GREEN_LED_PIN_CTRL_REG &= ~PORT_PCR_MUX_MASK;
			GREEN_LED_PIN_CTRL_REG |= PORT_PCR_MUX(3);
			break;
		}
		case BLUE_LED_PIN:
		{
			/* Set up Port Multiplexing as it is needed
			 * for PWM feature  for BLUE LED */
			BLUE_LED_PIN_CTRL_REG &= ~PORT_PCR_MUX_MASK;
			BLUE_LED_PIN_CTRL_REG |= PORT_PCR_MUX(4);
			break;
		}
		default:
		{
			break;
		}
	}
}

/*******************************************************************************
* @Function name: red_led_on
* @Description: turns red led on
* @input param:  none
* @return: none
*******************************************************************************/
void red_led_on()
{
	BLUE_LED_PWM_OFF();
	GREEN_LED_PWM_OFF();

	RED_LED_PWM_ON();
}

/*******************************************************************************
* @Function name: green_led_on
* @Description: turns red led on
* @input param:  none
* @return: none
*******************************************************************************/
void green_led_on()
{
	RED_LED_PWM_OFF();
	BLUE_LED_PWM_OFF();

	GREEN_LED_PWM_ON();
}

/*******************************************************************************
* @Function name: blue_led_on
* @Description: turns red led on
* @input param:  none
* @return: none
*******************************************************************************/
void blue_led_on()
{
	GREEN_LED_PWM_OFF();
	RED_LED_PWM_OFF();

	BLUE_LED_PWM_ON();
}

/*******************************************************************************
* @Function name: all_led_on
* @Description: turns all three led's on
* @input param:  none
* @return: none
*******************************************************************************/
void all_led_on()
{
	RED_LED_PWM_ON();
	GREEN_LED_PWM_ON();
	BLUE_LED_PWM_ON();
}

/*******************************************************************************
* @Function name: all_led_off
* @Description: turns all three led's on
* @input param:  none
* @return: none
*******************************************************************************/
void all_led_off()
{
	RED_LED_PWM_OFF();
	GREEN_LED_PWM_OFF();
	BLUE_LED_PWM_OFF();
}
