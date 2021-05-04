/******************************************************************************
* @file: main.c
*
* @brief: This files consists of the main logic to start the application and
* initialization
*
* @author: Rishab Shah
* @date:  30-Apr-2021
* @references: Making Embedded Systems - White
* @references: Dean book
* @references: Lecture notes - 1st and 2nd lecture for hexdump
*******************************************************************************/
/*******************************************************************************
Header files
*******************************************************************************/
#include "main.h"
/*******************************************************************************
Macros
*******************************************************************************/
#define	ACCUM_BUFF_SIZE						50
#define ERROR								-1
/*******************************************************************************
Global variables
*******************************************************************************/
static eDigitalAngleGaugeState current_state;
/*******************************************************************************
Function prototype
*******************************************************************************/
static void system_init();
static void application_code();
void digital_angle_gauge_state_machine();
void state_change_detector(eDigitalAngleGaugeState current_state,eDigitalAngleGaugeState new_state);
/*******************************************************************************
Function definition
*******************************************************************************/
/*******************************************************************************
* @Function Name: main
* @Description: Starts the application
* @input param: None
* @return: None
*******************************************************************************/
int main(void)
{
	/* Initialize the whole system */
	system_init();

	/* test the modules */
	test_modules();

	/* Enter into application mode */
	application_code();

	return 0;
}

/*******************************************************************************
* @Function Name: system_init
* @Description: Initializes the system settings
* @input param: None
* @return: None
*******************************************************************************/
static void system_init()
{
	/* Initialize the clock */
	init_sysclock();

	/* Initialize the systick clock */
	init_systick();

	/* Init tpm settings for the RGB leds */
	init_tpm();

	/* Init pwm settings for each RGB LED */
	init_led_pwm();

	/* Initialize UART */
	init_UART0();

	/* Initialize the switch module */
	init_switch();

	/* Initializes the capacitive touch sensor module */
	init_touchsensor();

	/* Initialize the I2C module */
	init_i2c();

	/* Initialize the accelerometer */
	if (!init_mma())
	{
		/* Light red error LED */
		rgb_pwm_controller(1, 0, 0);
		while (1)
			;
		/* not able to initialize mma */
	}

	printf("Welcome to Digital Angle Gauge!\r\n");
	printf("All modules are initialized\r\n");
}

/*******************************************************************************
* @Function Name: application_code
* @Description: Starts the application as per the requirement
* 1. Digital Angle Gauge - Generates different colors for the 30,45,60,90 angle
* 2.
* @input param: None
* @return: None
*******************************************************************************/
static void application_code()
{
	int8_t ch1;
	bool firstbackspace = true;
	uint8_t accumulate[ACCUM_BUFF_SIZE];

	memset(accumulate,'\0',sizeof(accumulate));
	uint8_t *bufptr = accumulate;

	printf("Please finalize your ground reference to 0 by trying to align FRDM parallel to ground\r\n");
	printf("Once aligned the LED will change to green and then go off!\r\n");

	current_state = INIT_STATE;

	while(1)
	{
		printf("# ");

		firstbackspace = true;

		while(1)
		{
			/* get the character from the queue */
			ch1 = getchar();

			if(ch1 != ERROR)
			{
				/* loop back the current data entered
				 * by the user */
				putchar(ch1);

				/* processing for backspace detection
				 * ascii value used */
				if(ch1 != 0x08)
				{
					/* character detection condition */
					/* reset as char is detected */
					firstbackspace = false;
					/* Store the character received */
					*bufptr++ = ch1;
				}
				else
				{
					/* backspace detection condition */
					if(firstbackspace == false)
					{
						/* Replace with space and move the cursor
						 * back to the same position */
						printf(" \b");
						/* decrement the location of pointer */
						*bufptr-- = ch1;
					}
					else
					{
						putchar(' ');
					}
				}

				if(ch1 == '\r')
				{
					/* Store the character received */
					*bufptr++ = '\n';
					*bufptr++ = '\0';
					bufptr = &accumulate[0];
					ch1 = -1;
					break;
				}
			}

			/* start scanning for touch sensor
			 * detection */
			start_touch_scanning();

			/* Enter the state machine for the main functionality */
			digital_angle_gauge_state_machine();
		}

		/* Once operation is performed bring
		 * the pointer back to the start location */
		bufptr = &accumulate[0];

		/* perform lexical analysis */
		lexical_analysis(accumulate);
	}
}

/*******************************************************************************
* @Function name: traffic_light_state
* @Description: main code which handles all the state transactions
* @input param: none
* @return: none
* @references: elevator.c examples
*******************************************************************************/
void digital_angle_gauge_state_machine()
{
	eDigitalAngleGaugeState new_state;

	new_state = current_state;

	switch (current_state)
	{
		case INIT_STATE:
			new_state = handle_calibrate_to_zero();
			break;

		case CALIBRATE_STATE:
			new_state = handle_calibrate_state();
			break;

		case ANGLE_DISPLAY_STATE:
			new_state = handle_angle_gauge_state();
			break;

		default:
			break;
	}

	if (new_state != current_state)
	{
		/* Only when a new state is detected */
		state_change_detector(current_state,new_state);

		current_state = new_state;
	}
}

/*******************************************************************************
* @Function name: state_change_detector
* @Description: Logs the sequence of states in which the state machine is executed
* @input param 1: current state of the machine
* @input param 2: current state of the machine
* @return: none
* @references: elevator.c examples
*******************************************************************************/
void state_change_detector(eDigitalAngleGaugeState current_state,eDigitalAngleGaugeState new_state)
{
	char buf[300];
	uint8_t calibreate = 0;
	uint8_t angle_display = 0;

	const char *current_str;
	const char *next_str;

	/* executing through below sequence to track
	 *  the state machine flow */
	switch(current_state)
	{
		case INIT_STATE:
		{
			current_str = "INIT_STATE";
			break;
		}
		case CALIBRATE_STATE:
		{
			current_str = "CALIBRATE_STATE";
			break;
		}
		case ANGLE_DISPLAY_STATE:
		{
			current_str = "ANGLE_DISPLAY_STATE";
			break;
		}
	}

	switch (new_state)
	{
		case INIT_STATE:
		{
			next_str = "INIT_STATE";
			break;
		}
		case CALIBRATE_STATE:
		{
			calibreate = 1;
			next_str = "CALIBRATE_STATE";
			break;
		}
		case ANGLE_DISPLAY_STATE:
		{
			angle_display = 1;
			next_str = "ANGLE_DISPLAY_STATE";
			break;
		}
	}

	snprintf(buf,sizeof(buf),"CS::%s, NS::%s\r\n",current_str, next_str);
	printf(buf);

	if(calibreate == 1)
	{
		printf("Press touch slider to set new reference!\r\n");
	}

	if(angle_display == 1)
	{
		printf("Roll FRDM to detect 30,45,60,90 degree angle\r\n");
		printf("Press button between PTD3 and ground to reset the reference level\r\n");
	}

	calibreate = 0;
	angle_display = 0;

}

/* EOF */
