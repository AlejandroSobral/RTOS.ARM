#include "logger.h"
#include "24LC256.h"
#include "dht11.h"
#include "UartMonitor.h"
#include "acelerometro.h"
#include "dht11.h"
#include "../main/main.h"
#include "string.h"
#include "../c-tds/port_Lpc1769.h"
#include "chip.h"
#include <stdio.h> //
#include <math.h>
#include "C:\Users\Alejandro\git2\lpc_chip_175x_6x\inc\eeprom_17xx_40xx.h"


void InformeBT(void)
{
	int i;
	static char cadena[LEN_BUF];
	memset(cadena,0,LEN_BUF);

	if(Chip_UART_ReadLineStatus(LPC_UART1)&UART_LSR_RDR)
	{
		if(Chip_UART_ReadByte(LPC_UART1)=='#')
		{
			if(!primer_inicio)
			{

				sprintf(cadena,"No se obtuvieron datos de viaje.\r\n");
				if((Chip_UART_ReadLineStatus(LPC_UART1) & UART_LSR_THRE))
				{
				for(i=0;i<LEN_BUFF;i++)
					Chip_UART_SendByte(LPC_UART1,cadena[i]);
				}
			}
			else
			{
				if(!cant_golpes)
				{
					sprintf(cadena,"Viaje exitoso! No hubo golpes.\r\n");
					if((Chip_UART_ReadLineStatus(LPC_UART1) & UART_LSR_THRE))
					{
					for(i=0;i<LEN_BUFF;i++)
						Chip_UART_SendByte(LPC_UART1,cadena[i]);
					}
				}
				else
				{

						static int ix = 0;
						if(estado==1)
						{
							Chip_UART_Send(LPC_UART1, &fecha[ix], 1);
							ix++;
							if(!fecha[ix])
							{
								ix = 0;
								estado = 2;
							}
						}
						if(estado==2)
						{
							Chip_UART_Send(LPC_UART1, &hora[ix], 1);
							ix++;
							if(!hora[ix])
							{
								ix = 0;
								estado = 3;
							}
						}
						if(estado==3)
						{
							Chip_UART_Send(LPC_UART1, &latitud[ix], 1);
							ix++;
							if(!latitud[ix])
							{
								ix = 0;
								estado = 4;
							}
						}
						if(estado==4)
						{
							Chip_UART_Send(LPC_UART1, &longitud[ix], 1);
							ix++;
							if(!longitud[ix])
							{
								ix = 0;
								estado = 0;
							}
						}

					}



				}
			}
		}
	}

}


