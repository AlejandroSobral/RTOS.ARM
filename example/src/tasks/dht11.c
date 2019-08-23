
// Project header
#include "dht11.h"
#include "demoramicrosegundos.h"
#include "../main/main.h"


// ------ Public variable ------------------------------------------
struct_sensores STRUCT_SENSOR;
uint8_t DemoraInicial;

// ------ Private variable -----------------------------------------
void GPIO_DHT11_Init(void)
{

	Chip_IOCON_PinMux(LPC_IOCON, GPIO_DHT11_PORT, GPIO_DHT11_PIN, GPIO_DHT11_PIN_PIN_MODE, GPIO_DHT11_PIN_FUNC);
	//Chip_GPIO_SetPinDIRInput(LPC_GPIO, GPIO_DHT11_PORT, GPIO_DHT11_PIN);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, GPIO_DHT11_PORT, GPIO_DHT11_PIN);
	Chip_GPIO_WritePortBit(LPC_GPIO, GPIO_DHT11_PORT, GPIO_DHT11_PIN, true); // LO DEJO EN UNO POR DEFECTO
	uint8_t DemoraInicial = 0;
}

void GPIO_DHT11(void)
{

		uint8_t Estado_Anterior	= ALTO; //Por default
		uint8_t contador		= 0;
		extern uint8_t DemoraInicial ;
		uint8_t Numero_Bit		= 0, Numero_Transicion = 0;
		extern struct_sensores STRUCT_SENSOR;


		 //BAJO EL PIN Y ESPERO 20mS aprox
		Chip_GPIO_WritePortBit(LPC_GPIO, GPIO_DHT11_PORT, GPIO_DHT11_PIN, false);
		if( DemoraInicial == 3){
			DemoraInicial = 0;


//		/* Pongo el pin en 0 18mS -- START */
//		GPIO_SetDir(port_sensor, pin_sensor, SALIDA );
//		GPIO_SetPin( port_sensor, pin_sensor, BAJO );
//		delayMilisegundo( 18 );

			Chip_GPIO_WritePortBit(LPC_GPIO, GPIO_DHT11_PORT, GPIO_DHT11_PIN, true);
			Chip_GPIO_SetPinDIRInput(LPC_GPIO, GPIO_DHT11_PORT, GPIO_DHT11_PIN);

		/* Pongo el pin en 1 40uS -- WAIT */
//		GPIO_SetPin( port_sensor, pin_sensor, ALTO );
//		GPIO_SetDir(port_sensor, pin_sensor, ENTRADA );
//		//delay10Microsegundo( 1 );
		/* Me preparo para leer el pin */
		//GPIO_SetDir(SHT1, ENTRADA );

		/* Detecto cambios en el port que leo */
		for ( Numero_Transicion = 0; Numero_Transicion < 85; Numero_Transicion++ )
		{
			contador = 0;
			while (  Chip_GPIO_ReadPortBit(LPC_GPIO, GPIO_DHT11_PORT, GPIO_DHT11_PIN) == Estado_Anterior  )
			{
				contador++;
				demoramicrosegundo( 10 );

				if ( contador == 25 )
				{
					break;
				}
			}
			Estado_Anterior = Chip_GPIO_ReadPortBit(LPC_GPIO, GPIO_DHT11_PORT, GPIO_DHT11_PIN);

			if ( contador == 25 )
			{
				break;
			}

			/* Ignoro primeras 3 transiciones */
			if ( (Numero_Transicion >= 4) && (Numero_Transicion % 2 == 0) ) // Mayor a 4 (0, 1,2, 3 TRANSICIONES SIN DATA) y par (PAR=DATA)
			{
				/* Guardo el bit en el array de data */
				STRUCT_SENSOR.DATA_SENSOR_ARRAY[Numero_Bit / 8] <<= 1;
				if ( contador > 3 ) // ESTE UMBRAL SE AJUSTA A OJIMETRO!
					STRUCT_SENSOR.DATA_SENSOR_ARRAY[Numero_Bit / 8] |= 1;
				Numero_Bit++;
			}
		}

		/*
		 * Verifico 40 bits y el checksum
		 * Retorno 1 o -1 si es OK o FAIL
		 */
		if ( (Numero_Bit >= 40)
				&& (STRUCT_SENSOR.DATA_SENSOR_ARRAY[4] == ( (STRUCT_SENSOR.DATA_SENSOR_ARRAY[0] +
						STRUCT_SENSOR.DATA_SENSOR_ARRAY[1] +
						STRUCT_SENSOR.DATA_SENSOR_ARRAY[2] +
						STRUCT_SENSOR.DATA_SENSOR_ARRAY[3]) & 0xFF) ) ) // chequeo con CheckSum
		{
			//ARMO EL DATO
				STRUCT_SENSOR.Valor_Humedad = STRUCT_SENSOR.DATA_SENSOR_ARRAY[0] ; //INTEGRAL HUMEDAD
				STRUCT_SENSOR.Valor_Humedad += STRUCT_SENSOR.DATA_SENSOR_ARRAY[1] / 10; // DECIMAL HUMEDAD

				STRUCT_SENSOR.Valor_Temperatura = STRUCT_SENSOR.DATA_SENSOR_ARRAY[2] ; // INTEGRAL TEMP
				STRUCT_SENSOR.Valor_Temperatura += STRUCT_SENSOR.DATA_SENSOR_ARRAY[3] / 10; // DECIMAL TEMP
		}

	}// CIERRO EL IF DE LAS VECES
		else {DemoraInicial++;}

		// ENCIENDO LAS INTERRUPCIONES
}


