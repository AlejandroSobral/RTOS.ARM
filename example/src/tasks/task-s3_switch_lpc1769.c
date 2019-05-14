/*--------------------------------------------------------------------*-

    task-s3_switch_lpc1769.c (Released 2019-04)

  --------------------------------------------------------------------

    Simple S3_SWITCH task for LPC1769.
	[Read Input on LPCXpresso baseboard.]

    Simple switch interface code, with software debounce.

-*--------------------------------------------------------------------*/


// Project header
#include "../main/main.h"


// Task header
#include "task-s3_switch_lpc1769.h"


// ------ Public variable ------------------------------------------
uint32_t S3_switch_pressed_G = SENSOR_OFF;


// ------ Private constants ----------------------------------------
// SW_THRES must be > 1 for correct debounce behavior
#define SW_THRES (3)


// ------ Private variable -----------------------------------------
static uint8_t switch_input = 0;


/*------------------------------------------------------------------*-

    S3_SWITCH_Init()

    Prepare for S3_SWITCH_Update() function - see below.

-*------------------------------------------------------------------*/
void S3_SWITCH_Init(void)
{
	// Set up "S2" SW as an input pin
	Chip_IOCON_PinMux(LPC_IOCON, S3_SWITCH_PORT, S3_SWITCH_PIN, S3_SWITCH_PIN_MODE, S3_SWITCH_PIN_FUNC);
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, S3_SWITCH_PORT, S3_SWITCH_PIN);

	// Switch not pressed
	S3_switch_pressed_G = SENSOR_OFF;
}


/*------------------------------------------------------------------*-

    S3_SWITCH_Update()

    Simple switch interface code, with software debounce.

    If S3_SWITCH is not pressed, S3_switch_pressed_G => SENSOR_OFF.
    If S3_SWITCH is     pressed, S3_switch_pressed_G => SENSOR_ON.

    Must schedule every 10 mili Second (soft deadline).

-*------------------------------------------------------------------*/
void S3_SWITCH_Update(void)
{
	// Duration of switch press
	static uint32_t Duration = 0;

	// Read S3_SWITCH
	switch_input = Chip_GPIO_ReadPortBit(LPC_GPIO, S3_SWITCH_PORT, S3_SWITCH_PIN);
	if (switch_input == SW_PRESSED)
	{
		Duration += 1;
		if (Duration > SW_THRES)
		{
			Duration = SW_THRES;
			S3_switch_pressed_G = SENSOR_ON;
		}
		else
		{
			// Switch pressed, but not yet for long enough
			S3_switch_pressed_G = SENSOR_OFF;
		}
	}
	else
	{
		// Switch not pressed - reset the count
		Duration = 0;

		// Update status
		S3_switch_pressed_G = SENSOR_OFF;
	}
}


/*------------------------------------------------------------------*-
  ---- END OF FILE -------------------------------------------------
-*------------------------------------------------------------------*/
