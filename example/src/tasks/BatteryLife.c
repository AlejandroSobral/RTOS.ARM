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
uint16_t bufADC;
int enviandoSerie = 0;


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
	static int estado = 0;
	int i;
	static char msg[LEN_BUF];
	uint32_t NivelBateria = 0;
	float PorcentajeFloat = 0;
	uint32_t PorcentajeInt = 0;

	if(Chip_ADC_ReadStatus(LPC_ADC,0,ADC_DR_DONE_STAT))
	{
		Chip_ADC_ReadValue(LPC_ADC,0,&bufADC);

		enviandoSerie = 1;
	}

	if(!enviandoSerie)
	{
		if(Chip_UART_ReadLineStatus(LPC_UART1)&UART_LSR_RDR)
		{
			if(Chip_UART_ReadByte(LPC_UART1)=='_')
			{
				Chip_ADC_SetStartMode(LPC_ADC,ADC_START_NOW,ADC_TRIGGERMODE_RISING);
			}
		}
	}

	if(enviandoSerie)
		{
		if(estado==0)
		{
			NivelBateria = bufADC * 8;
			PorcentajeFloat = (float)NivelBateria / (float)100;
			PorcentajeInt = (int)PorcentajeFloat;
			memset(msg,0,LEN_BUF);
			if (PorcentajeInt > 200)
			{
				sprintf(msg,"Estable: %03d\r\n", PorcentajeInt);

			}
			else
			{
				//SYSTEM_Perform_Safe_Shutdown();
			}
			estado++;


			enviandoSerie=0;
		}
		else
		{
		if((Chip_UART_ReadLineStatus(LPC_UART1) & UART_LSR_THRE))
		{
		for(i=0;i<LEN_BUFF;i++)
			Chip_UART_SendByte(LPC_UART1,msg[i]);
		estado=0;
		}
		}
	}
}
