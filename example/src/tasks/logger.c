#include "logger.h"
#include "dht11.h"
#include "UartMonitor.h"
#include "acelerometro.h"
#include "dht11.h"
#include "24LC256.h"
#include "../main/main.h"
#include "string.h"
#include "../c-tds/port_Lpc1769.h"
#include "chip.h"
#include <stdio.h>
#include <math.h>

//FlagUmbral[i]
//0 = AceleracionAngular
//1=Aceleracion
//2=Orientacion
//3= Humedad
//4= Temperatura
//5=Bateria
//6 = Dado vuelta

uint32_t FlagUmbral[NMROFLAGS];
uint32_t cantidad_golpes;
static uint32_t aux_invertido;

void Logger (void)
{
	extern struct_acelerometro DataAcelerometro;
	extern struct_sensores STRUCT_SENSOR;
	extern uint32_t Grabado;
	int DatoAcelerometro_Orientacion;
	uint32_t DatoAcelerometro_AceleracionAngular;


	uint32_t i = 0;



	for(i = 0; i<XYZBuf ; i++) // Z va por otro lado
	{	 DatoAcelerometro_AceleracionAngular = DataAcelerometro.FloatAceleracionAngular[i];
		if(DatoAcelerometro_AceleracionAngular > MaximaAceleracionAngular)
		{
			FlagUmbral[0] = 1; //giro brusco
		}

		if(DataAcelerometro.FloatAceleracion[i] > MaximaAceleracionGravitacional)//Esto es porque esta dado vuelta )
		{
			FlagUmbral[1] = 1; // Golpe
		}
	}

	for(i = 0; i<XYBuf ; i++)
	{DatoAcelerometro_Orientacion = DataAcelerometro.Orientacion[i];
		if((DatoAcelerometro_Orientacion > 55 || DatoAcelerometro_Orientacion < -55) && (DataAcelerometro.FloatAceleracion[0] > 3500 || DataAcelerometro.FloatAceleracion[1] > 3500 || DataAcelerometro.FloatAceleracion[2] < 0))
			{
				FlagUmbral[1] = 1; //Es un golpe y no una inclinacion

			}
		if((DatoAcelerometro_Orientacion > 55 || DatoAcelerometro_Orientacion < -55) && (DataAcelerometro.FloatAceleracion[0] < 2000 || DataAcelerometro.FloatAceleracion[1] < 2000 || DataAcelerometro.FloatAceleracion[2] < 3000))
		{
			FlagUmbral[2] = 1; // Es inclinacion
		}
	}
		if(STRUCT_SENSOR.Valor_Humedad > MaximaHumedad)
		{
		FlagUmbral[3] = 1;

		}
		if(STRUCT_SENSOR.Valor_Temperatura > MaximaTemperatura)
		{
		FlagUmbral[4] = 1;

		}

		//ANALISIS SI ESTA VOLTEADO completamente

		if(DataAcelerometro.FloatAceleracion[2] < 0)//Una porcion de G
				{
					aux_invertido++;
					if(aux_invertido == 100){ // ASUMO QUE ESTA VOLTEADO Y NO FUE UN SOLO GOLPE; 1 SEG APROX
					FlagUmbral[6] = 1;
					aux_invertido=0; //reseteo
					}

				}

		if(DataAcelerometro.FloatAceleracion[2] > 0 && aux_invertido > 0)
		{
			if(aux_invertido < 3){
				aux_invertido = 0;
				FlagUmbral[1] = 1; // FUE UN GOLPE
			}
		}



	//ACÁ ES DONDE GRABA LOS DATOS EN LA MEMORIA
if(FlagUmbral[0]==1||FlagUmbral[1]==1||FlagUmbral[2]==1||FlagUmbral[3]==1||FlagUmbral[4]==1||FlagUmbral[6]==1)

		{//GRABA EN LA MEMORIA
		Ciclo_Memoria_Working();
		Grabado = 1;
		cantidad_golpes++;
		}

if(Grabado){// && Enviado){

	FlagUmbral[0]=0;FlagUmbral[1]=0;FlagUmbral[2]=0;FlagUmbral[3]=0;FlagUmbral[4]=0;FlagUmbral[5]=0; FlagUmbral[6]=0;
	Grabado = 0;
	//Enviado = 0;
	}
}
