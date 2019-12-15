
// Project header
#include "dht11.h"
#include "demoramicrosegundos.h"
#include "../main/main.h"


// ------ Public variable ------------------------------------------
struct_sensores STRUCT_SENSOR;
uint8_t DemoraInicial = 0, EstadoLectura = DEMORA;
uint8_t demora_lanzada;
uint32_t Flag_StartDHT = 0;


// ------ Private variable -----------------------------------------
void GPIO_DHT11_Init(void)
{

	Chip_IOCON_PinMux(LPC_IOCON, GPIO_DHT11_PORT, GPIO_DHT11_PIN, GPIO_DHT11_PIN_PIN_MODE, GPIO_DHT11_PIN_FUNC);
	//Chip_GPIO_SetPinDIRInput(LPC_GPIO, GPIO_DHT11_PORT, GPIO_DHT11_PIN);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, GPIO_DHT11_PORT, GPIO_DHT11_PIN);


	Chip_GPIO_WritePortBit(LPC_GPIO, GPIO_DHT11_PORT, GPIO_DHT11_PIN, true); // LO DEJO EN UNO POR DEFECTO

}

void Timer1_Init(void)
{

	// Power-on Timer 1
	Chip_TIMER_Init(LPC_TIMER1);

	//Change Timer PCLK.
	Chip_Clock_SetPCLKDiv(SYSCTL_PCLK_TIMER0,SYSCTL_CLKDIV_1);

	// Initialize Timer 1, prescale counter = 1 μs
	Chip_TIMER_PrescaleSet(LPC_TIMER1,TIM_PRESCALE_USVAL-1);

	// Use channel 1, MR1
	Chip_TIMER_SetMatch(LPC_TIMER1,1,0);

	// Set timer flag when MR0 matches the value in TC register
	Chip_TIMER_MatchEnableInt(LPC_TIMER1,0);

	// Enable reset on MR0: timer will reset if MR0 matches it
	Chip_TIMER_ResetOnMatchEnable(LPC_TIMER1,0);

	// Stop timer if MR0 matches it
	Chip_TIMER_StopOnMatchEnable(LPC_TIMER1,0);

	// IMPORTANT:
	// Interrupt priorities must be set in Scheduler init function
	// [Not yet started timer]
}


void GPIO_DHT11(void)
{		uint32_t Pendiente;
		uint32_t CuentaActual;
		uint8_t Estado_Anterior	= ALTO; //Por default
		uint8_t contador		= 0, ValorLeido;

		extern uint8_t DemoraInicial ;
		extern uint8_t EstadoLectura ;
		uint8_t Numero_Bit		= 0, Numero_Transicion = 0;
		extern struct_sensores STRUCT_SENSOR;
		STRUCT_SENSOR.Valor_HumedadSincro = STRUCT_SENSOR.Valor_Humedad;
		STRUCT_SENSOR.Valor_TemperaturaSincro = STRUCT_SENSOR.Valor_Temperatura;


		 //BAJO EL PIN Y ESPERO 20mS aprox
		if(EstadoLectura == DEMORA && DemoraInicial < 1){
			Chip_GPIO_SetPinDIROutput(LPC_GPIO, GPIO_DHT11_PORT, GPIO_DHT11_PIN);
			Chip_GPIO_WritePortBit(LPC_GPIO, GPIO_DHT11_PORT, GPIO_DHT11_PIN, false);


			DemoraInicial++;
		}
		else  { EstadoLectura = LEYENDO;
				DemoraInicial = 0;
		}

		if( EstadoLectura == LEYENDO){

//		/* Pongo el pin en 0 18mS -- START */
//		GPIO_SetDir(port_sensor, pin_sensor, SALIDA );
//		GPIO_SetPin( port_sensor, pin_sensor, BAJO );
//		delayMilisegundo( 18 );

			Chip_GPIO_SetPinState(LPC_GPIO, GPIO_DHT11_PORT, GPIO_DHT11_PIN, true);
			demoramicrosegundo(3);

											while(demora_lanzada == 1)
											{
												;
											}
											Chip_TIMER_Disable(LPC_TIMER1);
											// Disable interrupt for Timer 0
											NVIC_DisableIRQ(TIMER1_IRQn);



			Chip_GPIO_SetPinDIRInput(LPC_GPIO, GPIO_DHT11_PORT, GPIO_DHT11_PIN);
			Chip_IOCON_PinMux(LPC_IOCON, GPIO_DHT11_PORT, GPIO_DHT11_PIN, IOCON_MODE_PULLUP, GPIO_DHT11_PIN_FUNC);
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
			ValorLeido = Chip_GPIO_ReadPortBit(LPC_GPIO, GPIO_DHT11_PORT, GPIO_DHT11_PIN);


			while (  Estado_Anterior == ValorLeido   )
			{
				ValorLeido = Chip_GPIO_ReadPortBit(LPC_GPIO, GPIO_DHT11_PORT, GPIO_DHT11_PIN);
				contador++;

				demoramicrosegundo(1);

								while(demora_lanzada == 1)
								{
									;
								}
								Chip_TIMER_Disable(LPC_TIMER1);
								// Disable interrupt for Timer 0
								NVIC_DisableIRQ(TIMER1_IRQn);

				if ( contador == 28 )
				{
					EstadoLectura = DEMORA;
					DemoraInicial = 0;
					break;
				}
			}
			ValorLeido = Chip_GPIO_ReadPortBit(LPC_GPIO, GPIO_DHT11_PORT, GPIO_DHT11_PIN);
			Estado_Anterior = ValorLeido;

			if ( contador == 28 )
			{
				EstadoLectura = DEMORA;
				DemoraInicial = 0;
				break;
			}

			/* Ignoro primeras 3 transiciones */
			if ( (Numero_Transicion >= 4) && (Numero_Transicion % 2 == 0) ) // Mayor a 4 (0, 1,2, 3 TRANSICIONES SIN DATA) y par (PAR=DATA)
			{
				/* Guardo el bit en el array de data */
				STRUCT_SENSOR.DATA_SENSOR_ARRAY[Numero_Bit / 8] <<= 1;
				if ( contador > 2 ) // ESTE UMBRAL SE AJUSTA A OJIMETRO!
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

						DemoraInicial = 0;
						EstadoLectura = DEMORA;
		}

	}
		if( STRUCT_SENSOR.Valor_Humedad > 0 || STRUCT_SENSOR.Valor_Temperatura > 0)
		{
			Flag_StartDHT = 1;
		}

		if( Flag_StartDHT == 1 && (STRUCT_SENSOR.Valor_Humedad == 0 || STRUCT_SENSOR.Valor_Temperatura == 0))
		{
			STRUCT_SENSOR.Valor_Humedad = STRUCT_SENSOR.Valor_HumedadSincro;
			STRUCT_SENSOR.Valor_Temperatura = STRUCT_SENSOR.Valor_TemperaturaSincro;
		}

		// ENCIENDO LAS INTERRUPCIONES
}


