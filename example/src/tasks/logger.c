#include "logger.h"
#include "dht11.h"
#include "UartMonitor.h"
#include "acelerometro.h"
#include "dht11.h"
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

uint32_t FlagUmbral[6];

void Logger (void)
{
	extern struct_acelerometro DataAcelerometro;
	extern struct_sensores STRUCT_SENSOR;
	extern uint32_t Grabado, Enviado;

	uint32_t i = 0;



	for(i = 0; i<XYZBuf ; i++)
	{
		if(DataAcelerometro.FloatAceleracionAngular[i] > MaximaAceleracionGravitacional)
		{
			FlagUmbral[0] = 1;
		}

		if(DataAcelerometro.FloatAceleracion[i] > MaximaAceleracionAngular )
		{
			FlagUmbral[1] = 1;
		}
	}

	for(i = 0; i<XYBuf ; i++)
	{
		if(DataAcelerometro.Orientacion[i] > MaximaInclinacionXY)
			{
				FlagUmbral[2] = 1;

			}
		if(STRUCT_SENSOR.Valor_Humedad > MaximaHumedad)
		{
		FlagUmbral[3] = 1;

		}
		if(STRUCT_SENSOR.Valor_Temperatura > MaximaTemperatura)
		{
		FlagUmbral[4] = 1;

		}
	}


	//ACÁ ES DONDE GRABA LOS DATOS EN LA MEMORIA
if(FlagUmbral[0]==1||FlagUmbral[1]==1||FlagUmbral[2]==1||FlagUmbral[3]==1||FlagUmbral[4]==1)

		{//GRABA EN LA MEMORIA

		Grabado = 1;
		}

if(Grabado && Enviado){

	FlagUmbral[0]=0;FlagUmbral[1]=0;FlagUmbral[2]=0;FlagUmbral[3]=0;FlagUmbral[4]=0;FlagUmbral[5]=0;
	Grabado = 0;
	Enviado = 0;
	}
}
