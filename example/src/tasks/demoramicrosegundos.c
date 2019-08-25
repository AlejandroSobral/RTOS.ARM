
// Project header
#include "demoramicrosegundos.h"
#include "../main/main.h"


int demoramicrosegundo(int microsegundos)
{
	extern uint8_t demora_lanzada;



		// Timeout value (in microseconds)
		Chip_TIMER_SetMatch(LPC_TIMER1, 0, microsegundos);

		// Enable interrupt for Timer 0
		Chip_TIMER_ClearMatch(LPC_TIMER1,0);
		NVIC_ClearPendingIRQ(TIMER1_IRQn);
		NVIC_EnableIRQ(TIMER1_IRQn);

		// Reset & Start timer
		Chip_TIMER_Reset(LPC_TIMER1);
		Chip_TIMER_Enable(LPC_TIMER1);
		demora_lanzada = 1;
}




