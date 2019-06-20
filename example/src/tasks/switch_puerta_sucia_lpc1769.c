
// Project header
#include "switch_puerta_sucia_lpc1769.h"
#include "../main/main.h"


// ------ Public variable ------------------------------------------
uint32_t SWITCH_PUERTA_SUCIA = SW_NOT_PRESSED;

// ------ Private variable -----------------------------------------
static uint8_t switch_input = 0;

void GPIO_SWITCH_PUERTA_SUCIA_Init(void)
{ extern uint8_t ESTADO_PUERTA_ENTRADA;
	// Set up "GPIO" SW as an input pin
	Chip_IOCON_PinMux(LPC_IOCON, GPIO_SWITCH_PUERTA_SUCIA_PORT, GPIO_SWITCH_PUERTA_SUCIA_PIN, GPIO_SWITCH_PUERTA_LIMPIA_PIN_MODE, GPIO_SWITCH_PUERTA_SUCIA_PIN_FUNC);
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, GPIO_SWITCH_PUERTA_SUCIA_PORT, GPIO_SWITCH_PUERTA_SUCIA_PIN);

	// Switch pressed
	SWITCH_PUERTA_SUCIA = SW_PRESSED;
	ESTADO_PUERTA_ENTRADA = CERRADA;
}

void GPIO_SWITCH_PUERTA_SUCIA_Update(void)
{

static uint8_t antirrebote_veces;
static uint8_t toggle;
static uint8_t contador_toggle;
extern uint8_t ESTADO_PUERTA_ENTRADA;

#define ON 1
#define OFF 0

if(toggle)
{contador_toggle++;
	if(contador_toggle >= 200)
		{toggle=0; contador_toggle=0;}
}
	// Read TEST_GPIO_SWITCH.
	switch_input = Chip_GPIO_ReadPortBit(LPC_GPIO, GPIO_SWITCH_PUERTA_SUCIA_PORT, GPIO_SWITCH_PUERTA_SUCIA_PIN);


	if (switch_input == SW_PRESSED)
	{	antirrebote_veces++;
		if(antirrebote_veces >= 50){ //ANTIRREBOTE OK

    	antirrebote_veces = 0; // REINICIO EL CONTADOR

    	SWITCH_PUERTA_SUCIA = SW_PRESSED;
    	ESTADO_PUERTA_ENTRADA = CERRADA;
		}
	}
	else
	{
		SWITCH_PUERTA_SUCIA = SW_NOT_PRESSED; //
		ESTADO_PUERTA_ENTRADA = ABIERTA;
	}

}
