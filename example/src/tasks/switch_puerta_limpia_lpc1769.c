
// Project header
#include "switch_puerta_limpia_lpc1769.h"
#include "../main/main.h"

// ------ Public variable ------------------------------------------
uint32_t SWITCH_PUERTA_LIMPIA = SW_NOT_PRESSED;
uint8_t ESTADO_PUERTA_ENTRADA;
uint8_t ESTADO_PUERTA_SALIDA;
uint8_t ESTADO_CERRADURA_SALIDA;
uint8_t ESTADO_CERRADURA_ENTRADA;

// ------ Private variable -----------------------------------------
static uint8_t switch_input = 0;

void GPIO_SWITCH_PUERTA_LIMPIA_Init(void)
{extern uint8_t ESTADO_PUERTA_SALIDA;
	// Set up "GPIO" SW as an input pin
	Chip_IOCON_PinMux(LPC_IOCON, GPIO_SWITCH_PUERTA_LIMPIA_PORT, GPIO_SWITCH_PUERTA_LIMPIA_PIN, GPIO_SWITCH_PUERTA_LIMPIA_PIN_MODE, GPIO_SWITCH_PUERTA_LIMPIA_PIN_FUNC);
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, GPIO_SWITCH_PUERTA_LIMPIA_PORT, GPIO_SWITCH_PUERTA_LIMPIA_PIN);

	// Switch pressed
	SWITCH_PUERTA_LIMPIA = SW_PRESSED;
	ESTADO_PUERTA_SALIDA = CERRADA;

}

void GPIO_SWITCH_PUERTA_LIMPIA_Update(void)
{

static uint8_t antirrebote_veces;
static uint8_t toggle;
static uint8_t contador_toggle;

if(toggle)
{contador_toggle++;
	if(contador_toggle >= 200)
		{toggle=0; contador_toggle=0;}
}
	// Read TEST_GPIO_SWITCH.
	switch_input = Chip_GPIO_ReadPortBit(LPC_GPIO, GPIO_SWITCH_PUERTA_LIMPIA_PORT, GPIO_SWITCH_PUERTA_LIMPIA_PIN);


	if (switch_input == SW_PRESSED)
	{	antirrebote_veces++;
		if(antirrebote_veces >= 50){ //ANTIRREBOTE OK

    	antirrebote_veces = 0; // REINICIO EL CONTADOR

    	SWITCH_PUERTA_LIMPIA = SW_PRESSED;
    	ESTADO_PUERTA_SALIDA = CERRADA;
		}
	}
	else
	{
		SWITCH_PUERTA_LIMPIA = SW_NOT_PRESSED; //
		ESTADO_PUERTA_SALIDA = ABIERTA;
	}

}
