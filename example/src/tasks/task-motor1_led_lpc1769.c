/*--------------------------------------------------------------------*-

    task-motor1_led_lpc1769.c (Released 2019-04)

  --------------------------------------------------------------------

    Simple MOTOR1_LED task for LPC1769.
	[Write Output on LPCXpresso baseboard.]

    Simple motor led interface code.

-*--------------------------------------------------------------------*/


// Project header
#include "../main/main.h"


// Task header
#include "task-motor1_led_lpc1769.h"


// ------ Public variable ------------------------------------------
uint32_t motor1_led_required_G = 0;


// ------ Private constants ----------------------------------------


// ------ Private variable -----------------------------------------


/*------------------------------------------------------------------*-

    MOTOR1_Init()

    Prepare for MOTOR1_Update() function - see below.

-*------------------------------------------------------------------*/
void MOTOR1_LED_Init(void)
{
	// Set up "GPIO" LED as an output pin
	Chip_IOCON_PinMux(LPC_IOCON, MOTOR1_LED_PORT, MOTOR1_LED_PIN, MOTOR1_LED_PIN_MODE, MOTOR1_LED_PIN_FUNC);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, MOTOR1_LED_PORT, MOTOR1_LED_PIN);
}


/*------------------------------------------------------------------*-

    MOTOR1_Update()

    Simple motor led interface code.

    If motor1_led_required_G => MOTOR_OFF, MOTOR1_LED => LED_OFF.
    If motor1_led_required_G => MOTOR_ON,  MOTOR1_LED => LED_ON.

    Must schedule every 10 mili Second (soft deadline).

-*------------------------------------------------------------------*/
void MOTOR1_LED_Update(void)
{
	// Read motor1_led_required_G "status"
	if (motor1_led_required_G == MOTOR_ON)
	{
		// Write MOTOR1_LED => LED_ON
    	Chip_GPIO_WritePortBit(LPC_GPIO, MOTOR1_LED_PORT, MOTOR1_LED_PIN, LED_ON);
	}
	else
	{
		// Write MOTOR1_LED => LED_OFF
		Chip_GPIO_WritePortBit(LPC_GPIO, MOTOR1_LED_PORT, MOTOR1_LED_PIN, LED_OFF);

	}
}


/*------------------------------------------------------------------*-
  ---- END OF FILE -------------------------------------------------
-*------------------------------------------------------------------*/
