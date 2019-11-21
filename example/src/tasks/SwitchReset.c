#include "SwitchReset.h"
#include "../main/main.h"
#include "string.h"
#include "../c-tds/port_Lpc1769.h"
#include "chip.h"
#include <stdio.h>
/*------------------------------------------------------------------*/

static uint8_t Sw_switch_Reset_state;
static uint8_t switch_input_reset;
extern ESTADO_GLOBAL_DEF ESTADO_GLOBAL;
extern uint32_t cantidad_golpes;
void Switch_Reset(void)
{

static uint8_t antirrebote_veces;
static uint8_t toggle;
static uint8_t contador_toggle;
extern uint32_t Switchea_lista_flag;

if(toggle)
{contador_toggle++;
	if(contador_toggle >= 1)
		{toggle=0; contador_toggle=0;}
}
	// Read TEST_GPIO_SWITCH.
	switch_input_reset = Chip_GPIO_ReadPortBit(LPC_GPIO, RESET_SWITCH_PORT, RESET_SWITCH_PIN);


	if (switch_input_reset == SW_PRESSED && toggle == 0)
	{	antirrebote_veces++;
		if(antirrebote_veces >= 1){ //ANTIRREBOTE OK
			toggle = 1;
    	antirrebote_veces = 0; // REINICIO EL CONTADOR

		Sw_switch_Reset_state = SW_PRESSED;
		ESTADO_GLOBAL.Modo = RESET;
		Switchea_lista_flag = 1;
		cantidad_golpes = 0;

		}
	}
	else
	{
		Sw_switch_Reset_state = SW_NOT_PRESSED; //
		if(ESTADO_GLOBAL.Modo != SENSORES)
			{Switchea_lista_flag = 1;
			}


		ESTADO_GLOBAL.Modo = SENSORES;

	}

}
void Switch_Reset_Init(void)
{



	// Set up "GPIO" SW as an input pin
	Chip_IOCON_PinMux(LPC_IOCON, RESET_SWITCH_PORT, RESET_SWITCH_PIN, SWITCH_RESET_PIN_MODE, SWITCH_RESET_PIN_FUNC);
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, RESET_SWITCH_PORT, RESET_SWITCH_PIN);

	// Switch not pressed
	Sw_switch_Reset_state = SW_NOT_PRESSED;
}

