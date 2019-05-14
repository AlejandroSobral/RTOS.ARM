/*--------------------------------------------------------------------*-

    task-s1_switch_lpc1769.c (Released 2019-04)

  --------------------------------------------------------------------

    Simple S1_SWITCH task for LPC1769.
	[Read Input on LPCXpresso baseboard.]

    Simple switch interface code, with software debounce.

-*--------------------------------------------------------------------*/


// Project header
#include "../main/main.h"


// Task header
#include "task-s1_switch_lpc1769.h"


// ------ Public variable ------------------------------------------
uint32_t S1_switch_pressed_G = SENSOR_OFF;


// ------ Private constants ----------------------------------------
// SW_THRES must be > 1 for correct debounce behavior
#define SW_THRES (3)


// ------ Private variable -----------------------------------------
static uint8_t switch_input = 0;


/*------------------------------------------------------------------*-

    S1_SWITCH_Init()

    Prepare for S1_SWITCH_Update() function - see below.

-*------------------------------------------------------------------*/
void S1_SWITCH_Init(void)
{
	// Set up "S1" SW as an input pin
	Chip_IOCON_PinMux(LPC_IOCON, S1_SWITCH_PORT, S1_SWITCH_PIN, S1_SWITCH_PIN_MODE, S1_SWITCH_PIN_FUNC);
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, S1_SWITCH_PORT, S1_SWITCH_PIN);

	// Switch not pressed
	S1_switch_pressed_G = SENSOR_OFF;
}


/*------------------------------------------------------------------*-

    S1_SWITCH_Update()

    Simple switch interface code, with software debounce.

    If S1_SWITCH is not pressed, S1_switch_pressed_G => SENSOR_OFF.
    If S1_SWITCH is     pressed, S1_switch_pressed_G => SW_PRESSED.

    Must schedule every 10 mili Second (soft deadline).

-*------------------------------------------------------------------*/
void S1_SWITCH_Update(void)
{
	// Duration of switch press
	static uint32_t Duration = 0;

	switch_input = Chip_GPIO_ReadPortBit(LPC_GPIO, S1_SWITCH_PORT, S1_SWITCH_PIN);
	if (switch_input == SW_PRESSED)
	{
		Duration += 1;
		if (Duration > SW_THRES)
		{
			Duration = SW_THRES;
			S1_switch_pressed_G = SENSOR_ON;
		}
		else
		{
			// Switch pressed, but not yet for long enough
			S1_switch_pressed_G = SENSOR_OFF;
		}
	}
	else
	{
		// Switch not pressed - reset the count
		Duration = 0;

		// Update status
		S1_switch_pressed_G = SENSOR_OFF;
	}
}


/*------------------------------------------------------------------*-
  ---- END OF FILE -------------------------------------------------
-*------------------------------------------------------------------*/
