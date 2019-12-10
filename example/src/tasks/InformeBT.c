#include "logger.h"
#include "24LC256.h"
#include "dht11.h"
#include "UartMonitor.h"
#include "acelerometro.h"
#include "../main/main.h"
#include "string.h"
#include "../c-tds/port_Lpc1769.h"
#include <stdio.h> //
#include <math.h>
#include "InformeBT.h"
//#include "../tasks/24LC256.h"

extern struct_dataRXeeprom dataRXeeprom_Read[CantidadMaximaGolpes];

void InformeBT(void){

static int lecturaunica = 0;
extern uint32_t cantidad_golpes, primer_inicio;
int i;
uint32_t j = 0;
static char cadena[LEN_BUF];
memset(cadena,0,LEN_BUF);
//uint8_t auxiliar1, auxiliar2;

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
			if(!cantidad_golpes)
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
				if(!lecturaunica)
				{
					for(j=0; j<cantidad_golpes;j++)
					{
						Ciclo_Memoria_Reading();
					}
					lecturaunica=1;
				}

				j = 0;

				while(j<cantidad_golpes) //AcÃ¡ recorro los golpes y envio la info de cada uno
				{
					//Levanto flags, y digo que problemas hubo, y a que hora y fecha
					//FlagUmbral[i]
					//13= AceleracionAngular
					//14= Aceleracion
					//15= Orientacion
					//16= Humedad
					//17= Temperatura
					//18= Bateria
					//19= Vuelta 180Â°

					for(int k=0;k<NMROFLAGS;k++)
					{
						switch(k)
						{
							case 0:
							if (dataRXeeprom_Read[j].dataRX2[13]==1)//
							{
								sprintf(cadena,"Hubo una fuerte acel. angular.\r\n");
								if((Chip_UART_ReadLineStatus(LPC_UART1) & UART_LSR_THRE))
								{
								for(i=0;i<31;i++)
								{ if((Chip_UART_ReadLineStatus(LPC_UART1) & UART_LSR_THRE))
									Chip_UART_SendByte(LPC_UART1,cadena[i]);
								}
								}
							}
							break;
							case 1:
								if (dataRXeeprom_Read[j].dataRX2[14]==1)
								{
									sprintf(cadena,"Hubo una fuerte acel.\r\n");
									if((Chip_UART_ReadLineStatus(LPC_UART1) & UART_LSR_THRE))
									{
									for(i=0;i<36;i++)
										if((Chip_UART_ReadLineStatus(LPC_UART1) & UART_LSR_THRE))
										Chip_UART_SendByte(LPC_UART1,cadena[i]);
									}
								}
								break;
							case 2:
								if (dataRXeeprom_Read[j].dataRX2[15]==1)
								{
									sprintf(cadena,"Hubo una fuerte inclinacion.\r\n");
									if((Chip_UART_ReadLineStatus(LPC_UART1) & UART_LSR_THRE))
									{
									for(i=0;i<36;i++){
										if((Chip_UART_ReadLineStatus(LPC_UART1) & UART_LSR_THRE))
										Chip_UART_SendByte(LPC_UART1,cadena[i]);
									}
									}
								}
								break;
							case 3:
								if (dataRXeeprom_Read[j].dataRX2[16]==1)
								{
									sprintf(cadena,"Humedad fuera del rango aceptable: %%c%c.\r\n", dataRXeeprom_Read[j].dataRX2[11], dataRXeeprom_Read[j].dataRX2[12]);
									if((Chip_UART_ReadLineStatus(LPC_UART1) & UART_LSR_THRE))
									{
									for(i=0;i<40;i++){
										if((Chip_UART_ReadLineStatus(LPC_UART1) & UART_LSR_THRE))
										Chip_UART_SendByte(LPC_UART1,cadena[i]);}
									}
								}
								break;
							case 4:

								if (dataRXeeprom_Read[j].dataRX2[17]==1)
								{
									sprintf(cadena,"Temperatura fuera del rango aceptable: %c%c grados.\r\n", dataRXeeprom_Read[j].dataRX2[8], dataRXeeprom_Read[j].dataRX2[9] );
									if((Chip_UART_ReadLineStatus(LPC_UART1) & UART_LSR_THRE))
									{
									for(i=0;i<50;i++){
										if((Chip_UART_ReadLineStatus(LPC_UART1) & UART_LSR_THRE))
										Chip_UART_SendByte(LPC_UART1,cadena[i]);
									}
									}
								}
								break;
							case 5:
								if (dataRXeeprom_Read[j].dataRX2[18]==1)
								{
									sprintf(cadena,"Bateria baja.\r\n");
									if((Chip_UART_ReadLineStatus(LPC_UART1) & UART_LSR_THRE))
									{
									for(i=0;i<14;i++)
									{ if((Chip_UART_ReadLineStatus(LPC_UART1) & UART_LSR_THRE))
										Chip_UART_SendByte(LPC_UART1,cadena[i]);
									}
									}
								}
								break;
							case 6:
								if (dataRXeeprom_Read[j].dataRX2[19]==1)
								{
									sprintf(cadena,"Me dieron vuelta como una media.\r\n");
									if((Chip_UART_ReadLineStatus(LPC_UART1) & UART_LSR_THRE))
									{
									for(i=0;i<32;i++)
									{ if((Chip_UART_ReadLineStatus(LPC_UART1) & UART_LSR_THRE))
										Chip_UART_SendByte(LPC_UART1,cadena[i]);
									}
									}
								}
								break;
						}
						memset(cadena,0,LEN_BUF);
					}


					sprintf(cadena,"El hecho ocurrio a las %c%c:%c%c:%c%c el dia %c%c/%c%c/%c%c\r\n",dataRXeeprom_Read[j].dataRX3[0],dataRXeeprom_Read[j].dataRX3[1],dataRXeeprom_Read[j].dataRX3[3],dataRXeeprom_Read[j].dataRX3[4],dataRXeeprom_Read[j].dataRX3[6],dataRXeeprom_Read[j].dataRX3[7],dataRXeeprom_Read[j].dataRX4[0],dataRXeeprom_Read[j].dataRX4[1],dataRXeeprom_Read[j].dataRX4[3],dataRXeeprom_Read[j].dataRX4[4],dataRXeeprom_Read[j].dataRX4[6],dataRXeeprom_Read[j].dataRX4[7] );
					if((Chip_UART_ReadLineStatus(LPC_UART1) & UART_LSR_THRE))
					{
					for(i=0;i<48;i++){
						if((Chip_UART_ReadLineStatus(LPC_UART1) & UART_LSR_THRE))
						Chip_UART_SendByte(LPC_UART1,cadena[i]);
					}
					}

					sprintf(cadena,"en las coordenadas %c%c %c%c'%c%c%c%c%c'' %c%c %c%c'%c%c%c%c%c''\r\n", dataRXeeprom_Read[j].dataRX3[10],dataRXeeprom_Read[j].dataRX3[11],dataRXeeprom_Read[j].dataRX3[13],dataRXeeprom_Read[j].dataRX3[14],dataRXeeprom_Read[j].dataRX3[16],dataRXeeprom_Read[j].dataRX3[17],dataRXeeprom_Read[j].dataRX3[18],dataRXeeprom_Read[j].dataRX3[19],dataRXeeprom_Read[j].dataRX3[20],dataRXeeprom_Read[j].dataRX3[24],dataRXeeprom_Read[j].dataRX4[10],dataRXeeprom_Read[j].dataRX4[11],dataRXeeprom_Read[j].dataRX4[13],dataRXeeprom_Read[j].dataRX4[14],dataRXeeprom_Read[j].dataRX4[16],dataRXeeprom_Read[j].dataRX4[17],dataRXeeprom_Read[j].dataRX4[18],dataRXeeprom_Read[j].dataRX4[19],dataRXeeprom_Read[j].dataRX3[20],dataRXeeprom_Read[j].dataRX4[24]);
					if((Chip_UART_ReadLineStatus(LPC_UART1) & UART_LSR_THRE))
					{
					for(i=0;i<47;i++){
						if((Chip_UART_ReadLineStatus(LPC_UART1) & UART_LSR_THRE))
						Chip_UART_SendByte(LPC_UART1,cadena[i]);
					}
					}

					j++;
				}

			}
		}
	}
}




}


