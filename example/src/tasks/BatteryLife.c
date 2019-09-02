#include "dht11.h"
#include "UartMonitor.h"
#include "acelerometro.h"
#include "BatteryLife.h"
#include "../main/main.h"
#include "string.h"
#include "../c-tds/port_Lpc1769.h"
#include "chip.h"
#include <stdio.h>

// ------ Public variable ------------------------------------------
uint16_t bufADC[LEN_BUF_ADC];
int colaIn = 0;
int colaOut = 0;


// ------ Private variable -----------------------------------------
void ADC0_Init(void)
{
	ADC_CLOCK_SETUP_T adc_setup;
	uint16_t dummy;
	Chip_IOCON_PinMuxSet(LPC_IOCON,ADC0_PORT,ADC0_PIN,IOCON_FUNC1);
	Chip_ADC_Init(LPC_ADC,&adc_setup);
	Chip_ADC_EnableChannel(LPC_ADC,ADC_CH0,ENABLE);
	Chip_ADC_ReadValue(LPC_ADC,ADC_CH0,&dummy);
}

void EstadoBateria(void)
{
	uint16_t val=0;
	if(Chip_ADC_ReadStatus(LPC_ADC,0,ADC_DR_DONE_STAT))
	{
		Chip_ADC_ReadValue(LPC_ADC,0,&val);
		bufADC[colaIn++]=val;
		if(colaIn<LEN_BUF_ADC)
		{
			Chip_ADC_SetStartMode(LPC_ADC,ADC_START_NOW,ADC_TRIGGERMODE_RISING);
		}
	}

}
