
// Project header
#include "demoramicrosegundos.h"
#include "../main/main.h"


void demoramicrosegundo(uint32_t microsegundos)
{
		// Timeout value (in microseconds)
		Chip_TIMER_SetMatch(LPC_TIMER0, 0, microsegundos);

		// Enable interrupt for Timer 0
		Chip_TIMER_ClearMatch(LPC_TIMER0,0);
		NVIC_ClearPendingIRQ(TIMER0_IRQn);
		NVIC_EnableIRQ(TIMER0_IRQn);
		// Reset & Start timer
		Chip_TIMER_Reset(LPC_TIMER0);
		Chip_TIMER_Enable(LPC_TIMER0);


	 while( microsegundos <= Chip_TIMER_ReadCount(LPC_TIMER0))
			 {
		 	 if ( microsegundos <= Chip_TIMER_ReadCount(LPC_TIMER0))
		 			 {
					Chip_TIMER_Disable(LPC_TIMER0);
					return;
		 			 }
			 }

}
