


// Project header
#include "../main/main.h"

// Task header
#include "cerraduras_puertas.h"
#define TRABADO 1
#define DESTRABADO 0
#define RESET 0
#define WORK 1

// ------ Public variable ------------------------------------------

// See scheduler for definition
extern uint32_t Fault_code_G;
extern uint32_t SWITCH_PUERTA_LIMPIA, SWITCH_PUERTA_SUCIA;
void cerraduras_puertas_Init(void)
{


	//CERRADURA ENTRADA
	Chip_IOCON_PinMux(LPC_IOCON, CERRADURA_ENTRADA_PORT, CERRADURA_ENTRADA_PIN, CERRADURA_ENTRADA_PIN_MODE, CERRADURA_ENTRADA_PIN_FUNC);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, CERRADURA_ENTRADA_PORT, CERRADURA_ENTRADA_PIN);
	Chip_GPIO_WritePortBit(LPC_GPIO, CERRADURA_ENTRADA_PORT, CERRADURA_ENTRADA_PIN, false); // ARRANCA DESTRABADA
	//CERRADURA SALIDA
	Chip_IOCON_PinMux(LPC_IOCON, CERRADURA_SALIDA_PORT, CERRADURA_SALIDA_PIN, CERRADURA_SALIDA_PIN_MODE, CERRADURA_SALIDA_PIN_FUNC);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, CERRADURA_SALIDA_PORT, CERRADURA_SALIDA_PIN);
	Chip_GPIO_WritePortBit(LPC_GPIO, CERRADURA_SALIDA_PORT, CERRADURA_SALIDA_PIN, false); // ARRANCA DESTRABADA

}


uint8_t ESTADO_GENERAL = RESET;

void cerraduras_puertas_Update(void)
{
	extern uint8_t ESTADO_PUERTA_ENTRADA;
	extern uint8_t ESTADO_PUERTA_SALIDA;
	extern uint8_t ESTADO_CERRADURA_ENTRADA;
	extern uint8_t ESTADO_CERRADURA_SALIDA;
	extern uint8_t ESTADO_GENERAL;
	static uint32_t CONTADOR_TIEMPO_WORK;

	if(ESTADO_GENERAL == WORK && ESTADO_PUERTA_ENTRADA == CERRADA && ESTADO_PUERTA_SALIDA == CERRADA && ESTADO_CERRADURA_ENTRADA == TRABADO && ESTADO_CERRADURA_SALIDA == TRABADO)
	{CONTADOR_TIEMPO_WORK++;
		if(CONTADOR_TIEMPO_WORK == 200)
		{ ESTADO_GENERAL = RESET;
		Chip_GPIO_WritePortBit(LPC_GPIO, CERRADURA_SALIDA_PORT, CERRADURA_SALIDA_PIN, false); // DESTRABO SALIDA
		ESTADO_CERRADURA_SALIDA = DESTRABADO;
		Chip_GPIO_WritePortBit(LPC_GPIO, CERRADURA_ENTRADA_PORT, CERRADURA_ENTRADA_PIN, false); // DESTRABO LA ENTRADA
		ESTADO_CERRADURA_ENTRADA = DESTRABADO;
		CONTADOR_TIEMPO_WORK = 0;
		}
	}
	if(ESTADO_CERRADURA_SALIDA == DESTRABADO && ESTADO_CERRADURA_ENTRADA == DESTRABADO )
	{
		ESTADO_GENERAL = RESET;
	}
	else {
		ESTADO_GENERAL = WORK;
	}

	if( ESTADO_GENERAL == RESET){


		if( ESTADO_PUERTA_SALIDA == ABIERTA) // SI EL SWITCH SE PRESIONA, TRABO LA OTRA PUERTA
			{
			Chip_GPIO_WritePortBit(LPC_GPIO, CERRADURA_ENTRADA_PORT, CERRADURA_ENTRADA_PIN, true); // TRABO LA ENTRADA
			ESTADO_CERRADURA_ENTRADA = TRABADO;
			Chip_GPIO_WritePortBit(LPC_GPIO, CERRADURA_SALIDA_PORT, CERRADURA_SALIDA_PIN, false); // DESTRABO SALIDA
			ESTADO_CERRADURA_SALIDA = DESTRABADO;
			ESTADO_GENERAL = WORK;

				}

		if( ESTADO_PUERTA_ENTRADA == ABIERTA) // SI EL SWITCH SE PRESIONA, TRABO LA OTRA PUERTA
				{
				Chip_GPIO_WritePortBit(LPC_GPIO, CERRADURA_SALIDA_PORT, CERRADURA_SALIDA_PIN, true); // TRABO LA SALIDA
				ESTADO_CERRADURA_SALIDA = TRABADO;
				Chip_GPIO_WritePortBit(LPC_GPIO, CERRADURA_ENTRADA_PORT, CERRADURA_ENTRADA_PIN, false); // DESTRABO ENTRADA
				ESTADO_CERRADURA_ENTRADA = DESTRABADO;
				ESTADO_GENERAL = WORK;
					}

	}

	if( ESTADO_GENERAL == WORK)
	{
		if(ESTADO_PUERTA_ENTRADA == CERRADA)
		{ Chip_GPIO_WritePortBit(LPC_GPIO, CERRADURA_ENTRADA_PORT, CERRADURA_ENTRADA_PIN, true); // TRABO LA ENTRADA
		ESTADO_CERRADURA_ENTRADA = TRABADO;
		}
		if(ESTADO_PUERTA_SALIDA == CERRADA)
		{ Chip_GPIO_WritePortBit(LPC_GPIO, CERRADURA_SALIDA_PORT, CERRADURA_SALIDA_PIN, true); // TRABO LA SALIDA
			ESTADO_CERRADURA_SALIDA = TRABADO;
		}
	}
}

//if( ESTADO_PUERTA_ENTRADA == CERRADA && ESTADO_GENERAL == RESET) // SI EL SWITCH SE PRESIONA, TRABO LA OTRA PUERTA
//			{
//			Chip_GPIO_WritePortBit(LPC_GPIO, CERRADURA_ENTRADA_PORT, CERRADURA_ENTRADA_PIN, false); // DESTRABO LA ENTRADA
//			ESTADO_CERRADURA_ENTRADA = DESTRABADO;
//			Chip_GPIO_WritePortBit(LPC_GPIO, CERRADURA_SALIDA_PORT, CERRADURA_SALIDA_PIN, true); // TRABO SALIDA
//			ESTADO_CERRADURA_SALIDA = TRABADO;
//				}
//
//if( ESTADO_PUERTA_SALIDA == CERRADA && ESTADO_GENERAL == RESET) // SI EL SWITCH SE PRESIONA, TRABO LA OTRA PUERTA
//				{
//				Chip_GPIO_WritePortBit(LPC_GPIO, CERRADURA_ENTRADA_PORT, CERRADURA_ENTRADA_PIN, true); // TRABO LA ENTRADA
//				ESTADO_CERRADURA_ENTRADA = TRABADO;
//				Chip_GPIO_WritePortBit(LPC_GPIO, CERRADURA_SALIDA_PORT, CERRADURA_SALIDA_PIN, false); // DESTRABO SALIDA
//				ESTADO_CERRADURA_SALIDA = DESTRABADO;
//					}
/*------------------------------------------------------------------*-
  ---- END OF FILE -------------------------------------------------
-*------------------------------------------------------------------*/
