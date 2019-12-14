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
//#include "../tasks/24LC256.h"//

extern struct_dataRXeeprom dataRXeeprom_Read[CantidadMaximaGolpes];
int lecturaunica = 0;
void InformeBT(void){

	Ciclo_Memoria_Reading_CantidadGolpes();

extern uint32_t cantidad_golpes, primer_inicio;
int i;
uint32_t j = 0;
static char cadena[LEN_BUF];
memset(cadena,0,LEN_BUF);
//uint8_t auxiliar1, auxiliar2;#

if(Chip_UART_ReadLineStatus(LPC_UART1)&UART_LSR_RDR)
{
	if(Chip_UART_ReadByte(LPC_UART1)=='#')
	{
		if(!primer_inicio)
		{

			sprintf(cadena,"No se obtuvieron datos de viaje.\r\n");
			for(i=0;i<40;i++)
			{
				while (!(Chip_UART_ReadLineStatus(LPC_UART1) & UART_LSR_THRE));
				{
					;
				}
				if((Chip_UART_ReadLineStatus(LPC_UART1) & UART_LSR_THRE))
				{
				Chip_UART_SendByte(LPC_UART1,cadena[i]);
				}
			}
		}
		else
		{
			if(!cantidad_golpes)
			{	memset(cadena,0,LEN_BUF);
				sprintf(cadena,"\r \nViaje exitoso! No hubo golpes.\r\n");
				for(i=0;i<40;i++)
				{
					while (!(Chip_UART_ReadLineStatus(LPC_UART1) & UART_LSR_THRE));
					{
						;
					}
					if((Chip_UART_ReadLineStatus(LPC_UART1) & UART_LSR_THRE))
					{
					Chip_UART_SendByte(LPC_UART1,cadena[i]);
					}
				}
				memset(cadena,0,LEN_BUF);
			}
			else
			{	memset(cadena,0,LEN_BUF);
				sprintf(cadena,"\r\n La cantidad de eventos fue de %d \r\n ", cantidad_golpes);

				for(i=0;i<45;i++)
				{
				while (!(Chip_UART_ReadLineStatus(LPC_UART1) & UART_LSR_THRE));
				{
					;
				}
				if((Chip_UART_ReadLineStatus(LPC_UART1) & UART_LSR_THRE))
				{
				Chip_UART_SendByte(LPC_UART1,cadena[i]);

				}
			}
				memset(cadena,0,LEN_BUF);
				if(!lecturaunica)
				{
					for(j=0; j<=cantidad_golpes;j++)
					{
						Ciclo_Memoria_Reading();
					}
					lecturaunica=1;
				}

				j = 0;

				for(j=0;j<cantidad_golpes;j++) //AcÃ¡ recorro los golpes y envio la info de cada uno
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
					memset(cadena,0,LEN_BUF);
					sprintf(cadena,"\r\n En el evento Nmro %d: ", j);

					for(i=0;i<35;i++)
					{
						while (!(Chip_UART_ReadLineStatus(LPC_UART1) & UART_LSR_THRE));
						{
							;
						}
						if((Chip_UART_ReadLineStatus(LPC_UART1) & UART_LSR_THRE))
						{
						Chip_UART_SendByte(LPC_UART1,cadena[i]);

						}
					}



							if (dataRXeeprom_Read[j+1].dataRX2[13]==1)
							{	memset(cadena,0,LEN_BUF);
								sprintf(cadena,"Hubo una fuerte acel. angular.\r\n");

								for(i=0;i<40;i++)
								{
									while (!(Chip_UART_ReadLineStatus(LPC_UART1) & UART_LSR_THRE));
									{
										;
									}
									if((Chip_UART_ReadLineStatus(LPC_UART1) & UART_LSR_THRE))
									{
									Chip_UART_SendByte(LPC_UART1,cadena[i]);

									}
								}
							}


								if (dataRXeeprom_Read[j+1].dataRX2[14]==1)
								{	memset(cadena,0,LEN_BUF);
									sprintf(cadena," Hubo una golpe de mas de 2g.\r\n");
									for(i=0;i<35;i++)
									{
										while (!(Chip_UART_ReadLineStatus(LPC_UART1) & UART_LSR_THRE));
										{
											;
										}
										if((Chip_UART_ReadLineStatus(LPC_UART1) & UART_LSR_THRE))
										{
										Chip_UART_SendByte(LPC_UART1,cadena[i]);

										}
									}
								}

								if (dataRXeeprom_Read[j+1].dataRX2[15]==1)
								{	memset(cadena,0,LEN_BUF);
									sprintf(cadena," Se excedieron los 45 grados inclinacion. \r\n");
									for(i=0;i<50;i++)
									{
										while (!(Chip_UART_ReadLineStatus(LPC_UART1) & UART_LSR_THRE));
										{
											;
										}
										if((Chip_UART_ReadLineStatus(LPC_UART1) & UART_LSR_THRE))
										{
										Chip_UART_SendByte(LPC_UART1,cadena[i]);

										}
									}
								}


								if (dataRXeeprom_Read[j+1].dataRX2[16]==1)
								{	memset(cadena,0,LEN_BUF);
									sprintf(cadena," Humedad fuera del rango aceptable: %d%d %. \r\n", dataRXeeprom_Read[j].dataRX2[11], dataRXeeprom_Read[j].dataRX2[12]);
									for(i=0;i<50;i++)
									{
									while (!(Chip_UART_ReadLineStatus(LPC_UART1) & UART_LSR_THRE));
									{
										;
									}
									if((Chip_UART_ReadLineStatus(LPC_UART1) & UART_LSR_THRE))
										{
										Chip_UART_SendByte(LPC_UART1,cadena[i]);

										}
									}
								}



								if (dataRXeeprom_Read[j+1].dataRX2[17]==1)
								{	memset(cadena,0,LEN_BUF);
									sprintf(cadena," Temperatura fuera del rango aceptable: %d%d grados.\r\n", dataRXeeprom_Read[j].dataRX2[8], dataRXeeprom_Read[j].dataRX2[9] );
									for(i=0;i<50;i++)
									{
										while (!(Chip_UART_ReadLineStatus(LPC_UART1) & UART_LSR_THRE));
										{
											;
										}
										if((Chip_UART_ReadLineStatus(LPC_UART1) & UART_LSR_THRE))
										{
										Chip_UART_SendByte(LPC_UART1,cadena[i]);

										}
									}
								}


								if (dataRXeeprom_Read[j+1].dataRX2[18]==1)
								{	memset(cadena,0,LEN_BUF);
									sprintf(cadena,"Bateria baja.\r\n");
									for(i=0;i<31;i++)
									{
										while (!(Chip_UART_ReadLineStatus(LPC_UART1) & UART_LSR_THRE));
										{
											;
										}
										if((Chip_UART_ReadLineStatus(LPC_UART1) & UART_LSR_THRE))
										{
										Chip_UART_SendByte(LPC_UART1,cadena[i]);

										}
									}
								}



								if (dataRXeeprom_Read[j+1].dataRX2[19]==1)
								{	memset(cadena,0,LEN_BUF);
									sprintf(cadena," Inversion de 180 grados detectada \r\n");
									for(i=0;i<39;i++)
									{
										while (!(Chip_UART_ReadLineStatus(LPC_UART1) & UART_LSR_THRE));
										{
											;
										}
										if((Chip_UART_ReadLineStatus(LPC_UART1) & UART_LSR_THRE))
											{
											Chip_UART_SendByte(LPC_UART1,cadena[i]);

											}
									}
								}





							memset(cadena,0,LEN_BUF);
							sprintf(cadena," El hecho ocurrio a las %d%d:%d%d:%d%d el dia %d%d/%d%d/%d%d \r \n ",dataRXeeprom_Read[j].dataRX3[0],dataRXeeprom_Read[j].dataRX3[1],dataRXeeprom_Read[j].dataRX3[3],dataRXeeprom_Read[j].dataRX3[4],dataRXeeprom_Read[j].dataRX3[6],dataRXeeprom_Read[j].dataRX3[7],dataRXeeprom_Read[j].dataRX4[0],dataRXeeprom_Read[j].dataRX4[1],dataRXeeprom_Read[j].dataRX4[3],dataRXeeprom_Read[j].dataRX4[4],dataRXeeprom_Read[j].dataRX4[6],dataRXeeprom_Read[j].dataRX4[7] );

							for(i=0;i<60;i++)
							{
								while (!(Chip_UART_ReadLineStatus(LPC_UART1) & UART_LSR_THRE));
									{
										;
									}
								if((Chip_UART_ReadLineStatus(LPC_UART1) & UART_LSR_THRE))
									{
									Chip_UART_SendByte(LPC_UART1,cadena[i]);

									}
							}
							memset(cadena,0,LEN_BUF);
							sprintf(cadena," en las coordenadas %d%d %d%d'%d%d%d%d%d'' %d%d %d%d'%d%d%d%d%d''\r\n", dataRXeeprom_Read[j].dataRX3[10],dataRXeeprom_Read[j].dataRX3[11],dataRXeeprom_Read[j].dataRX3[13],dataRXeeprom_Read[j].dataRX3[14],dataRXeeprom_Read[j].dataRX3[16],dataRXeeprom_Read[j].dataRX3[17],dataRXeeprom_Read[j].dataRX3[18],dataRXeeprom_Read[j].dataRX3[19],dataRXeeprom_Read[j].dataRX3[20],dataRXeeprom_Read[j].dataRX3[24],dataRXeeprom_Read[j].dataRX4[10],dataRXeeprom_Read[j].dataRX4[11],dataRXeeprom_Read[j].dataRX4[13],dataRXeeprom_Read[j].dataRX4[14],dataRXeeprom_Read[j].dataRX4[16],dataRXeeprom_Read[j].dataRX4[17],dataRXeeprom_Read[j].dataRX4[18],dataRXeeprom_Read[j].dataRX4[19],dataRXeeprom_Read[j].dataRX3[20],dataRXeeprom_Read[j].dataRX4[24]);

							for(i=0;i<60;i++)
							{
									while (!(Chip_UART_ReadLineStatus(LPC_UART1) & UART_LSR_THRE));
									{
										;;
									}
								if((Chip_UART_ReadLineStatus(LPC_UART1) & UART_LSR_THRE))
									{
									Chip_UART_SendByte(LPC_UART1,cadena[i]);

								}
							}



				}//Cierro el while de J
			}//else de cantidad de golpes
		}//else de primer incio
	}
}




}


