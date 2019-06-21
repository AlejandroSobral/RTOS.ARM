
// Project header
#include "switch_emergencia.h"
#include "../main/main.h"

// ------ Public variable ------------------------------------------
uint32_t SWT_EMER_ENTRADA = SW_NOT_PRESSED;
uint32_t SWT_EMER_MEDIO = SW_NOT_PRESSED;
uint32_t SWT_EMER_SALIDA = SW_NOT_PRESSED;

extern uint8_t ESTADO_GENERAL;

// ------ Private variable -----------------------------------------
static uint8_t switch_input = 0;

void GPIO_SWITCH_Emergencia_Init(void)
{

	//BOTON EMERGENCIA MEDIO
	Chip_IOCON_PinMux(LPC_IOCON, GPIO_SWITCH_EMERGENCIA_MEDIO_PORT, GPIO_SWITCH_EMERGENCIA_MEDIO__PIN, GPIO_SWITCH_EMERGENCIA_MEDIO__PIN_MODE, GPIO_SWITCH_EMERGENCIA_MEDIO__PIN_FUNC);
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, GPIO_SWITCH_EMERGENCIA_MEDIO_PORT, GPIO_SWITCH_EMERGENCIA_MEDIO__PIN);

	//BOTON EMERGENCIA SALA LIMPIA
		Chip_IOCON_PinMux(LPC_IOCON, GPIO_SWITCH_EMERGENCIA_LIMPIA_PORT, GPIO_SWITCH_EMERGENCIA_LIMPIA_PIN, GPIO_SWITCH_EMERGENCIA_LIMPIA_PIN_MODE, GPIO_SWITCH_EMERGENCIA_LIMPIA_FUNC);
		Chip_GPIO_SetPinDIRInput(LPC_GPIO, GPIO_SWITCH_EMERGENCIA_LIMPIA_PORT, GPIO_SWITCH_EMERGENCIA_LIMPIA_PIN);
	//BOTON EMERGENCIA ENTRADA
		Chip_IOCON_PinMux(LPC_IOCON, GPIO_SWITCH_EMERGENCIA_ENTRADA_PORT, GPIO_SWITCH_EMERGENCIA_ENTRADA_PIN, GPIO_SWITCH_EMERGENCIA_ENTRADA_PIN_MODE, GPIO_SWITCH_EMERGENCIA_ENTRADA_PIN_FUNC);
		Chip_GPIO_SetPinDIRInput(LPC_GPIO, GPIO_SWITCH_EMERGENCIA_ENTRADA_PORT, GPIO_SWITCH_EMERGENCIA_ENTRADA_PIN);

		// Switch pressed
		SWT_EMER_ENTRADA = SW_NOT_PRESSED;
		SWT_EMER_SALIDA = SW_NOT_PRESSED;
		SWT_EMER_MEDIO = SW_NOT_PRESSED;

}

void GPIO_SWITCH_Emergencia_Update(void)
{

static uint8_t antirrebote_veces_medio;
static uint8_t antirrebote_veces_salida;
static uint8_t antirrebote_veces_entrada;



	// Read TEST_GPIO_SWITCH.
	switch_input = Chip_GPIO_ReadPortBit(LPC_GPIO, GPIO_SWITCH_EMERGENCIA_MEDIO_PORT, GPIO_SWITCH_EMERGENCIA_MEDIO__PIN);


	if (switch_input == SW_PRESSED)
	{	antirrebote_veces_medio++;
		if(antirrebote_veces_medio >= 50){ //ANTIRREBOTE OK
			antirrebote_veces_medio = 0; // REINICIO EL CONTADOR
    	SWT_EMER_MEDIO = SW_PRESSED;
		}
	}
	else
	{
		SWT_EMER_MEDIO = SW_NOT_PRESSED; //
	}

		// Read TEST_GPIO_SWITCH.
	switch_input = Chip_GPIO_ReadPortBit(LPC_GPIO, GPIO_SWITCH_EMERGENCIA_ENTRADA_PORT, GPIO_SWITCH_EMERGENCIA_ENTRADA_PIN);


		if (switch_input == SW_PRESSED)
		{	antirrebote_veces_salida++;
			if(antirrebote_veces_salida >= 50){ //ANTIRREBOTE OK
				antirrebote_veces_salida = 0; // REINICIO EL CONTADOR
	    	SWT_EMER_ENTRADA = SW_PRESSED;
			}
		}
		else
		{
			SWT_EMER_ENTRADA = SW_NOT_PRESSED; //
		}
	switch_input = Chip_GPIO_ReadPortBit(LPC_GPIO, GPIO_SWITCH_EMERGENCIA_LIMPIA_PORT, GPIO_SWITCH_EMERGENCIA_LIMPIA_PIN);


		if (switch_input == SW_PRESSED)
		{	antirrebote_veces_entrada++;
			if(antirrebote_veces_entrada >= 50){ //ANTIRREBOTE OK
				antirrebote_veces_entrada = 0; // REINICIO EL CONTADOR
	    	SWT_EMER_SALIDA = SW_PRESSED;
			}
		}
		else
		{
			SWT_EMER_SALIDA = SW_NOT_PRESSED; //
		}
		if(SWT_EMER_SALIDA == SW_PRESSED || SWT_EMER_MEDIO == SW_PRESSED || SWT_EMER_ENTRADA == SW_PRESSED)
		{
			ESTADO_GENERAL = EMERGENCIA;}
}
