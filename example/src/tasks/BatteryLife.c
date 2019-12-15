#include "dht11.h"
#include "logger.h"
#include "UartMonitor.h"
#include "acelerometro.h"
#include "BatteryLife.h"
#include "../c-tds/system_lpc1769.h"
#include "../c-tds/scheduler_lpc1769.h"
#include "BatteryLife.h"
#include "../main/main.h"
#include "string.h"
#include "../c-tds/port_Lpc1769.h"
#include "chip.h"
#include <stdio.h>

// ------ Public variable ------------------------------------------
uint16_t bufADC;
int enviandoSerie = 0;
uint32_t GraboInfoBateriaBaja;
uint32_t BatBajaContador;
extern


// ------ Private variable -----------------------------------------
void ADC0_Init(void)
{
	ADC_CLOCK_SETUP_T adc_setup;
	uint16_t dummy;
	Chip_IOCON_PinMuxSet(LPC_IOCON,ADC0_PORT,ADC0_PIN,IOCON_FUNC1);
	Chip_ADC_Init(LPC_ADC,&adc_setup);
	Chip_ADC_EnableChannel(LPC_ADC,ADC_CH0,ENABLE);
	Chip_ADC_ReadValue(LPC_ADC,ADC_CH0,&dummy);
	Chip_ADC_SetStartMode(LPC_ADC,ADC_START_NOW,ADC_TRIGGERMODE_RISING);
}

void EstadoBateria(void)
{
	uint32_t NivelBateria = 0;
	float PorcentajeFloat = 0;
	uint32_t PorcentajeInt = 0;
	extern uint32_t FlagUmbral[NMROFLAGS];

	if(Chip_ADC_ReadStatus(LPC_ADC,0,ADC_DR_DONE_STAT))
	{
		Chip_ADC_ReadValue(LPC_ADC,0,&bufADC);

	}

	Chip_ADC_SetStartMode(LPC_ADC,ADC_START_NOW,ADC_TRIGGERMODE_RISING);


	NivelBateria = bufADC * 8;
	PorcentajeFloat = (float)NivelBateria / (float)100;
	PorcentajeInt = (int)PorcentajeFloat;
	if (PorcentajeInt < 175)
	{
		BatBajaContador++;
		if(BatBajaContador >= 100)
		{
			FlagUmbral[5] = 1; // Flag de batería baja
			if(GraboInfoBateriaBaja == 1)
			{
				SYSTEM_Perform_Safe_Shutdown();
			}
		}

	}
	else
	{
		BatBajaContador = 0;
	}

}
