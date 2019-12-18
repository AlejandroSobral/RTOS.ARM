#include "dht11.h"
#include "UartMonitor.h"
#include "acelerometro.h"
#include "BatteryLife.h"
#include "../main/main.h"
#include "string.h"
#include "../c-tds/port_Lpc1769.h"
#include "chip.h"
#include <stdio.h>
#include "GPS.h"

#define Pidiendo	1
#define Guardando	2
#define Soltando	3


/* $GPRMC,123412.00,A,3435.71704,S,05826.52804,W,0.044,,300919,,,A*79
 * Hora: buffgps[7], buffgps[8]
 * Minutos: buffgps[9], buffgps[10]
 * Segundos: buffgps[11], buffgps[12]
 * Checksum: buffgps[17]
 * Latitud: buffgps[19], buffgps[20], buffgps[21], buffgps[22], buffgps[24], buffgps[25], buffgps[26], buffgps[27], buffgps[28], buffgps[30]
 * Longitud: buffgps[33], buffgps[34], buffgps[35], buffgps[36], buffgps[38], buffgps[39], buffgps[40], buffgps[41], buffgps[42], buffgps[44]
 * Fecha: buffgps[53], buffgps[54], buffgps[55], buffgps[56], buffgps[57], buffgps[58]
 */

// ------ Public variable ------------------------------------------
char buffgps[70]={0};
char hora[10]={0};
char checksum;
char latitud[18]={0};
char longitud[18]={0};
char fecha[10]={0};


// ------ Private variable -----------------------------------------
void UartGPSInit (void)
{
	/*Inicializo la UART*/
		Chip_IOCON_PinMuxSet(LPC_IOCON,UART2_PORT,UART2_TXD_PIN,IOCON_FUNC1);
		Chip_IOCON_PinMuxSet(LPC_IOCON,UART2_PORT,UART2_RXD_PIN,IOCON_FUNC1);
		Chip_UART_Init(LPC_UART2);
		Chip_UART_SetBaud(LPC_UART2,UART2_BAUDRATE);
		Chip_UART_ConfigData(LPC_UART2, (UART_LCR_WLEN8 | UART_LCR_SBS_1BIT));
		Chip_UART_SetupFIFOS(LPC_UART2, (UART_FCR_FIFO_EN | UART_FCR_TRG_LEV3));
		Chip_UART_TXEnable(LPC_UART2);
}

void TareaLeeGPS(void) //El GPS arranca sin mandar ningún mensaje.
{
	static char bufsal[16];
	static char bufent[128];
	static int ix=0;
	static int cadena_invalida = 0;
	char dummy;

	static int i=0;

	memset(bufsal,0,sizeof(bufsal));
	sprintf(bufsal,"$EIGPQ,RMC\r\n"); //Pide al GPS que envíe las coordenadas una sola vez
	for(i=0;i<sizeof(bufsal);i++)
	{
		while (!(Chip_UART_ReadLineStatus(LPC_UART2) & UART_LSR_THRE))
		{
			;
		}
		if((Chip_UART_ReadLineStatus(LPC_UART2) & UART_LSR_THRE))
		{
		Chip_UART_SendByte(LPC_UART2, bufsal[i]);
		}
	}

	if(Chip_UART_ReadLineStatus(LPC_UART2)&UART_LSR_RDR)
	{
		Chip_UART_Read(LPC_UART2, (void*)&dummy, 1); //El problema acá!
		if(dummy == '$' || ix == 128)
		{
			memset(bufent,0,sizeof(bufent));
			ix = 0;
			bufent[ix++]='$';
		}
		else if(dummy == '\n')
		{
			if(!strncmp(bufent,"$GPRMC",strlen("$GPRMC")) && cadena_invalida == 0)
			{
				strncpy(buffgps,bufent,128);

				sprintf(hora, "%c%c:%c%c:%c%c\r\n", buffgps[7], buffgps[8], buffgps[9], buffgps[10], buffgps[11], buffgps[12]);
				sprintf(latitud, "%c%c %c%c'%c%c%c%c%c'' %c\r\n", buffgps[19], buffgps[20], buffgps[21], buffgps[22], buffgps[24], buffgps[25], buffgps[26], buffgps[27], buffgps[28], buffgps[30]);
				sprintf(longitud, "%c%c %c%c'%c%c%c%c%c'' %c\r\n", buffgps[33], buffgps[34], buffgps[35], buffgps[36], buffgps[38], buffgps[39], buffgps[40], buffgps[41], buffgps[42], buffgps[44]);
				sprintf(fecha, "%c%c/%c%c/%c%c\r\n", buffgps[53], buffgps[54], buffgps[55], buffgps[56], buffgps[57], buffgps[58]);
			}
			else
				cadena_invalida = 0;
		}
		else
		{
			bufent[ix++]=dummy;
			if (dummy == 'V')
				cadena_invalida = 1;
		}
	}
}

