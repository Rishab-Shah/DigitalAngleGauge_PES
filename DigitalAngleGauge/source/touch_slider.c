/******************************************************************************
* @file: touch_slider.c
*
* @brief: This files consists of all the logic related to touch slider
* initialization and reading of data
*
* @author: Rishab Shah
* @date:  21-Feb-2021
* @version 1.0
* @references: https://github.com/alexander-g-dean/ESF/tree/master/NXP/Misc/Touch%20Sense
*******************************************************************************/
/*******************************************************************************
 Header Files
*******************************************************************************/
#include "touch_slider.h"
/*******************************************************************************
 Macros
*******************************************************************************/
int cap_tap_detected;
/*******************************************************************************
 Macros
*******************************************************************************/
/* Touch Slider configuration */
/* offset value to be subtracted */
#define TOUCH_OFFSET 						(650)
/* macro for extracting the count from data register */
#define TOUCH_DATA 							(TSI0->DATA & 0xFFFF)
#define POLL_BASED							0
#define DEBOUNCE_DURATION					(100000)
/*******************************************************************************
 Function definition
*******************************************************************************/
/*******************************************************************************
* @Function name: Touch_Init
* @Description: Initialization for touch sensor module
* @input param: none
* @return: none
* @references:https://github.com/alexander-g-dean/ESF/tree/master/NXP/Misc/Touch%20Sense
*******************************************************************************/
void init_touchsensor()
{
	SIM->SCGC5 |= SIM_SCGC5_TSI_MASK;   // enabling the clock

	TSI0->GENCS = TSI_GENCS_MODE(0u) |	// operating in non-noise mode
				TSI_GENCS_REFCHRG(0u) |	// reference oscillator charge and discharge value 500nA
				TSI_GENCS_DVOLT(0u) |	// oscillator voltage rails set to default
				TSI_GENCS_EXTCHRG(0u) |	// electrode oscillator charge and discharge value 500nA
				TSI_GENCS_PS(0u) |		// frequency clock divided by one
				TSI_GENCS_NSCN(31u) |	// scanning the electrode 32 times
				TSI_GENCS_TSIEN_MASK |	// enabling the TSI module
				TSI_GENCS_STPE_MASK |   // enabling the TSI in low power modes
				TSI_GENCS_EOSF_MASK |	// writing one to clear the end of scan flag
				TSI_GENCS_ESOR_MASK | 	// enabling interrupt using end of scan
				TSI_GENCS_TSIIEN_MASK;  // enabling the TSI interrupt

	/* selecting channel 10 */
	TSI0->DATA = 	TSI_DATA_TSICH(10u);

	/* Enabling interrupt in NVIC */
	NVIC_SetPriority(TSI0_IRQn, 2);
	NVIC_ClearPendingIRQ(TSI0_IRQn);
	NVIC_EnableIRQ(TSI0_IRQn);

	/* Allow low leakage stop mode */
	SMC->PMPROT = SMC_PMPROT_ALLS_MASK;

	/* Enable low-leakage stop mode and regular run mode */
	SMC->PMCTRL = SMC_PMCTRL_STOPM(3) | SMC_PMCTRL_RUNM(0);
	SMC->STOPCTRL = SMC_STOPCTRL_PSTOPO(0) | SMC_STOPCTRL_VLLSM(3);

	/* Enable LLWU */
	/* allow TSI0 to wake LLWU */
	LLWU->ME |= LLWU_ME_WUME4_MASK;

}

/*******************************************************************************
* @Function name: TSI0_IRQHandler
* @Description: Scan for the values on the touch slider
* @input param: none
* @return: none
* @references:https://github.com/alexander-g-dean/ESF/tree/master/NXP/Misc/Touch%20Sense
*******************************************************************************/
void TSI0_IRQHandler (void)
{
	/* Added a debounce condition for proper
	 * behavior. Determined experimentally */
	for(int i = 0 ; i < DEBOUNCE_DURATION; i++);

	int touch_value = 0;

	/* Get the touch value from the sensor */
	touch_value = TOUCH_DATA - TOUCH_OFFSET;

	cap_tap_detected = 0;

	/* Check if it crosses threshold, then only change the state */
	if(touch_value > 0)
	{
		/* change state */
		cap_tap_detected = 1;
	}

	/* writing one to clear the flag */
	TSI0->GENCS |= TSI_GENCS_EOSF_MASK ;
}

/*******************************************************************************
* @Function name: start_touch_scanning
* @Description: start the scan for the touch detection
* @input param: none
* @return: none
*******************************************************************************/
void start_touch_scanning()
{
	TSI0->DATA |= TSI_DATA_SWTS_MASK;
}
