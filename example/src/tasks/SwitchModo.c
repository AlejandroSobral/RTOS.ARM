#include "SwitchMODO.h"
#include "../main/main.h"
#include "string.h"
#include "SwitchReset.h"
#include "../c-tds/port_Lpc1769.h"
#include "chip.h"
#include <stdio.h>
/*------------------------------------------------------------------*/

static uint8_t Sw_switch_MODO_state;
static uint8_t switch_input_MODO;
extern ESTADO_GLOBAL_DEF ESTADO_GLOBAL;
void Switch_MODO(void)
{
extern uint32_t cantidad_golpes;
static uint8_t antirrebote_veces;
static uint8_t toggle_modo;
static uint8_t contador_toggle;
extern uint32_t Switchea_lista_flag;

if(toggle_modo)
{contador_toggle++;
	if(contador_toggle >= 1)
		{toggle_modo=0; contador_toggle=0;}
}
	// Read TEST_GPIO_SWITCH.
	switch_input_MODO = Chip_GPIO_ReadPortBit(LPC_GPIO, MODO_SWITCH_PORT, MODO_SWITCH_PIN);


	if (switch_input_MODO == SW_PRESSED && toggle_modo == 0)
	{	antirrebote_veces++;
		if(antirrebote_veces >= 1){ //ANTIRREBOTE OK
			toggle_modo = 1;
    	antirrebote_veces = 0; // REINICIO EL CONTADOR

		Sw_switch_MODO_state = SW_PRESSED;
		ESTADO_GLOBAL.ModoPrevio = ESTADO_GLOBAL.Modo;
		if(ESTADO_GLOBAL.Modo == RESET)
		{
		ESTADO_GLOBAL.Modo = ESTADO_GLOBAL.ModoPrevio; //EL RESET TIENE PRIORIDAD
		}
		else ESTADO_GLOBAL.Modo = BTH;

		if(ESTADO_GLOBAL.Modo != ESTADO_GLOBAL.ModoPrevio)
					{
					Switchea_lista_flag = 1;
					}
		}
	}
	else
	{
		Sw_switch_MODO_state = SW_NOT_PRESSED; //
		if(ESTADO_GLOBAL.Modo == BTH && Sw_switch_MODO_state == SW_NOT_PRESSED)
			{Switchea_lista_flag = 1;
			cantidad_golpes = 0;

			}
		if(ESTADO_GLOBAL.Modo!=RESET){ // si esta en RESET, este tiene prioridad
		ESTADO_GLOBAL.Modo = SENSORES;}

	}

}
void Switch_MODO_Init(void)
{



	// Set up "GPIO" SW as an input pin
	Chip_IOCON_PinMux(LPC_IOCON, MODO_SWITCH_PORT, MODO_SWITCH_PIN, SWITCH_MODO_PIN_MODE, SWITCH_MODO_PIN_FUNC);
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, MODO_SWITCH_PORT, MODO_SWITCH_PIN);

	// Switch not pressed
	Sw_switch_MODO_state = SW_NOT_PRESSED;
}

