/*--------------------------------------------------------------------*-

    task-motor2_led_lpc1769.c (Released 2019-04)

  --------------------------------------------------------------------

    Simple MOTOR2_LED task for LPC1769.
	[Write Output on LPCXpresso baseboard.]

    Simple motor led interface code.

-*--------------------------------------------------------------------*/


// Project header
#include "../main/main.h"


// Task header
#include "task-motor2_led_lpc1769.h"


// ------ Public variable ------------------------------------------
uint32_t motor2_led_required_G = 0;


// ------ Private constants ----------------------------------------


// ------ Private variable -----------------------------------------


/*------------------------------------------------------------------*-

    MOTOR2_Init()

    Prepare for MOTOR2_Update() function - see below.

-*------------------------------------------------------------------*/
void MOTOR2_LED_Init(void)
{
	// Set up "MOTRO2" LED as an output pin
	Chip_IOCON_PinMux(LPC_IOCON, MOTOR2_LED_PORT, MOTOR2_LED_PIN, MOTOR2_LED_PIN_MODE, MOTOR2_LED_PIN_FUNC);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, MOTOR2_LED_PORT, MOTOR2_LED_PIN);
}


/*------------------------------------------------------------------*-

    MOTOR2_Update()

    Simple motor led interface code.

    If motor2_led_required_G => MOTOR_OFF, MOTOR2_LED => LED_OFF.
    If motor2_led_required_G => MOTOR_ON,  MOTOR2_LED => LED_ON.

    Must schedule every 10 mili Second (soft deadline).

-*------------------------------------------------------------------*/
void MOTOR2_LED_Update(void)
{
	// Read motor2_led_required_G "status"
	if (motor2_led_required_G == MOTOR_ON)
	{
		// Write MOTOR2_LED => LED_ON
    	Chip_GPIO_WritePortBit(LPC_GPIO, MOTOR2_LED_PORT, MOTOR2_LED_PIN, LED_ON);
	}
	else
	{
		// Write MOTOR2_LED => LED_OFF
		Chip_GPIO_WritePortBit(LPC_GPIO, MOTOR2_LED_PORT, MOTOR2_LED_PIN, LED_OFF);

	}
}


/*------------------------------------------------------------------*-
  ---- END OF FILE -------------------------------------------------
-*------------------------------------------------------------------*/
