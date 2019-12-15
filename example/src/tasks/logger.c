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

uint32_t Pausa_Golpe;
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
			FlagUmbral[AceleracionAngularExcedida] = 1; //giro brusco
		}

		if(DataAcelerometro.FloatAceleracion[i] > MaximaAceleracionGravitacional || DataAcelerometro.FloatAceleracion[i] < -MaximaAceleracionGravitacional )
		{
			if( i == 2 && DataAcelerometro.FloatAceleracion[i] > (MaximaAceleracionGravitacional+Gravedad) )
			{ // Si es en Z, hay que sumar la gravedad
			FlagUmbral[AceleracionExcedida] = 1; // Golpe
			Pausa_Golpe = 1;
			}
			else
			{
			FlagUmbral[AceleracionExcedida] = 1; // Golpe
			Pausa_Golpe = 1;
			}
		}



		if(FlagUmbral[AceleracionExcedida] == 0) Pausa_Golpe = 0; // Hasta que no detecto que el golpe frenó, no mido inclinación
	}

//	for(i = 0; i<XYBuf ; i++)
//	{
//		if(FlagUmbral[AceleracionExcedida] == 0 && Pausa_Golpe == 0) // SI ES 1, IMPLICA QUE SE GOLPEO, NO PUEDO MEDIR ANGULOS
//		{
//			DatoAcelerometro_Orientacion = DataAcelerometro.Orientacion[i];
//			if((DatoAcelerometro_Orientacion > 55 || DatoAcelerometro_Orientacion < -55))
//			{
//				FlagUmbral[AnguloExcedido] = 1; // Es inclinacion
//
//			}
//		}
//
//	  }

	if(STRUCT_SENSOR.Valor_Humedad > MaximaHumedad)
	{
		FlagUmbral[ExcesoHumedad] = 1;
	}
	if(STRUCT_SENSOR.Valor_Temperatura > MaximaTemperatura)
	{
	FlagUmbral[ExcesoTemperatura] = 1;
	}


		//Analizo si está invertido 180 grados
		if(DataAcelerometro.FloatAceleracion[2] < 0)
				{//Antirrebote de invertido 180 grados
					aux_invertido++;
					if(aux_invertido == 100){ // ASUMO QUE ESTA VOLTEADO Y NO FUE UN SOLO GOLPE; 1 SEG APROX
					FlagUmbral[Invertido180] = 1; // efectivamente esta invertido
					aux_invertido=0; //reseteo
					}

				}

		if(DataAcelerometro.FloatAceleracion[2] < -7000 && aux_invertido == 0 && FlagUmbral[Invertido180] == 0 ) //Casi 2g
		{

				FlagUmbral[AceleracionExcedida] = 1; // FUE UN GOLPE

		}



	//ACÁ ES DONDE GRABA LOS DATOS EN LA MEMORIA
if(FlagUmbral[AceleracionAngularExcedida]==1||FlagUmbral[AceleracionExcedida]==1||FlagUmbral[AnguloExcedido]==1||FlagUmbral[ExcesoHumedad]==1||FlagUmbral[ExcesoTemperatura]==1||FlagUmbral[BateriaBaja]==1||FlagUmbral[Invertido180]==1)
{//GRABA EN LA MEMORIA
extern uint32_t LeyoCantidadGolpesDeLaMemoria;
extern uint32_t GraboInfoBateriaBaja;
 		Ciclo_Memoria_Reading_CantidadGolpes(); // LEO DE MEMORIA LA ULTIMA POSICION DE MEMORIA GRABADA
		Ciclo_Memoria_Working(); // ESCRIBO LOS DATOS EN MEMORIA
		Ciclo_Memoria_Writing_CantidadGolpes(); // ESCRIBO EN MEMORIA LA ULTIMA POSICION

		if(LeyoCantidadGolpesDeLaMemoria == 0)
		{ Ciclo_Memoria_Reading_CantidadGolpes();
		}
		if(LeyoCantidadGolpesDeLaMemoria == 1)
		{	cantidad_golpes++;
			Ciclo_Memoria_Writing_CantidadGolpes();
		}
		Grabado = 1;
	 if(FlagUmbral[BateriaBaja] == 1 && Grabado == 1) // SI ESTOY EN CASO DE BATERIA BAJA
	 {												// Y YA GRABE TODO EN MEMORIA
		 GraboInfoBateriaBaja = 1;
	 }
}

	if(Grabado)
	{
		FlagUmbral[AceleracionAngularExcedida] = 0;
		FlagUmbral[AceleracionExcedida] = 0;
		FlagUmbral[AnguloExcedido] = 0;
		FlagUmbral[ExcesoHumedad] = 0;
		FlagUmbral[ExcesoTemperatura] = 0;
		FlagUmbral[BateriaBaja] = 0;
		FlagUmbral[Invertido180] = 0;
		Grabado = 0;
	}
}
