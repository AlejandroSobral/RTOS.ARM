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

void TareaLeeGPS(void)
{
	static char bufsal[26];
	static char bufent[70];
	static int ix=0;
	char dummy;
	static int estado = 1;

	static int i=0;

	switch (estado)
	{

		case Pidiendo:
			memset(bufsal,0,26); // $PUBX,40,RMC,0,0,0,0,0,0 desactiva, $PUBX,40,RMC,0,1,0,0,0,0 activa
			sprintf(bufsal,"$PUBX,40,RMC,0,1,0,0,0,0\r\n");
			for(i=0;i<26;i++)
			{
				while (!(Chip_UART_ReadLineStatus(LPC_UART2) & UART_LSR_THRE));
				{
					;
				}
				if((Chip_UART_ReadLineStatus(LPC_UART2) & UART_LSR_THRE))
				{
				Chip_UART_SendByte(LPC_UART2,bufsal[i]);
				}
			}
			estado = Guardando;
			break;

		case Guardando:

			while (!(Chip_UART_ReadLineStatus(LPC_UART2) & UART_LSR_RDR));
			{
				;
			}
			if(Chip_UART_ReadLineStatus(LPC_UART2)&UART_LSR_RDR)
				{
					Chip_UART_Read(LPC_UART2, (void*)&dummy, 1);
					//if(dummy == '$' || ix == 70)
					if(dummy == '$')
					{
						memset(bufent,0,70);
						ix = 0;
						bufent[ix++]='$';
						break;
					}
					else if(ix == 70)
					{
						strncpy(buffgps,bufent,70);
						memset(bufent,0,70);

						sprintf(hora, "%c%c:%c%c:%c%c\r\n", buffgps[7], buffgps[8], buffgps[9], buffgps[10], buffgps[11], buffgps[12]);
						sprintf(latitud, "%c%c %c%c'%c%c%c%c%c'' %c\r\n", buffgps[19], buffgps[20], buffgps[21], buffgps[22], buffgps[24], buffgps[25], buffgps[26], buffgps[27], buffgps[28], buffgps[30]);
						sprintf(longitud, "%c%c %c%c'%c%c%c%c%c'' %c\r\n", buffgps[33], buffgps[34], buffgps[35], buffgps[36], buffgps[38], buffgps[39], buffgps[40], buffgps[41], buffgps[42], buffgps[44]);
						sprintf(fecha, "%c%c/%c%c/%c%c\r\n", buffgps[53], buffgps[54], buffgps[55], buffgps[56], buffgps[57], buffgps[58]);

						estado = Soltando;
						break;

					}
					else
					{
						bufent[ix++]=dummy;
						break;
					}
				}
			break;

		case Soltando:

			memset(bufsal,0,26); // $PUBX,40,RMC,0,0,0,0,0,0 desactiva, $PUBX,40,RMC,0,1,0,0,0,0 activa
			sprintf(bufsal,"$PUBX,40,RMC,0,0,0,0,0,0\r\n");

			for(i=0;i<26;i++)
			{
				while (!(Chip_UART_ReadLineStatus(LPC_UART2) & UART_LSR_THRE));
				{
					;
				}
				if((Chip_UART_ReadLineStatus(LPC_UART2) & UART_LSR_THRE))
				{
				Chip_UART_SendByte(LPC_UART2,bufsal[i]);
				}
			}
			estado = Pidiendo;
			break;

		default:
			break;
	}








}

//
//	if(Chip_UART_ReadLineStatus(LPC_UART2)&UART_LSR_RDR)
//	{
//		Chip_UART_Read(LPC_UART2, (void*)&dummy, 1);
//		if(dummy == '$' || ix == 128)
//		{
//			memset(buf,0,128);
//			ix = 0;
//			buf[ix++]='$';
//		}
//		else if(dummy == '\n')
//		{
//			if(!strncmp(buf,"$GPRMC",strlen("$GPRMC")))
//			{
//				strncpy(buffgps,buf,128);
//
//				sprintf(hora, "%c%c:%c%c:%c%c\r\n", buffgps[7], buffgps[8], buffgps[9], buffgps[10], buffgps[11], buffgps[12]);
//				sprintf(latitud, "%c%c %c%c'%c%c%c%c%c'' %c\r\n", buffgps[19], buffgps[20], buffgps[21], buffgps[22], buffgps[24], buffgps[25], buffgps[26], buffgps[27], buffgps[28], buffgps[30]);
//				sprintf(longitud, "%c%c %c%c'%c%c%c%c%c'' %c\r\n", buffgps[33], buffgps[34], buffgps[35], buffgps[36], buffgps[38], buffgps[39], buffgps[40], buffgps[41], buffgps[42], buffgps[44]);
//				sprintf(fecha, "%c%c/%c%c/%c%c\r\n", buffgps[53], buffgps[54], buffgps[55], buffgps[56], buffgps[57], buffgps[58]);
//			}
//		}
//		else
//		{
//			buf[ix++]=dummy;
//		}
//	}

