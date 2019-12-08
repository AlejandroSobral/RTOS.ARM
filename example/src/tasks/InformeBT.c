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
#include "InformeBT.h"


void InformeBT(void){

extern uint32_t cantidad_golpes, primer_inicio;

uint32_t i = 0;
if(primer_inicio!=0)
{

	for(i=0; i< cantidad_golpes;i++)
	{
		Ciclo_Memoria_Reading();


	}
}





}

//
//void InformeBT(void)
//{
//	extern uint32_t cantidad_golpes, primer_inicio;
//	int i;
//	static char cadena[LEN_BUF];
//	memset(cadena,0,LEN_BUF);
//
//	if(Chip_UART_ReadLineStatus(LPC_UART1)&UART_LSR_RDR)
//	{
//		if(Chip_UART_ReadByte(LPC_UART1)=='#')
//		{
//			if(!primer_inicio)
//			{
//
//				sprintf(cadena,"No se obtuvieron datos de viaje.\r\n");
//				if((Chip_UART_ReadLineStatus(LPC_UART1) & UART_LSR_THRE))
//				{
//				for(i=0;i<LEN_BUFF;i++)
//					Chip_UART_SendByte(LPC_UART1,cadena[i]);
//				}
//			}
//			else
//			{
//				if(!cantidad_golpes)
//				{
//					sprintf(cadena,"Viaje exitoso! No hubo golpes.\r\n");
//					if((Chip_UART_ReadLineStatus(LPC_UART1) & UART_LSR_THRE))
//					{
//					for(i=0;i<LEN_BUFF;i++)
//						Chip_UART_SendByte(LPC_UART1,cadena[i]);
//					}
//				}
//				else
//				{
//
////						static int ix = 0;
////						if(estado==1)
////						{
////							Chip_UART_Send(LPC_UART1, &fecha[ix], 1);
////							ix++;
////							if(!fecha[ix])
////							{
////								ix = 0;
////								estado = 2;
////							}
////						}
//
//
//
//
//
//				}
//			}
//		}
//	}
//
//}


