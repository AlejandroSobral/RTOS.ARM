/*------------------------------------------------------------------*-

    port_lpc1769.h (Released 2019-04)

  ------------------------------------------------------------------

    This is the "Port Header" file: it documents usage of port pins
    in the project.

-*------------------------------------------------------------------*/

#ifndef _PORT_H
#define _PORT_H 1


// ------ Public constants -----------------------------------------
#define TIM_PRESCALE_USVAL	(100)




// Allows Active Low or Active High led to be used (or other wiring variations)
#define LED_ON	(true)
#define LED_OFF	(false)

// Allows NO or NC switch to be used (or other wiring variations)

#define ABIERTA 0
#define CERRADA 1
#define ALTO 1
#define BAJO 0
#define LEYENDO 1
#define DEMORA 2
// Heartbeat LED
// Connected to "LED2" on LPC1769 board
// Port 0, Pin 22
#define HEARTBEAT_LED_PORT (0)
#define HEARTBEAT_LED_PIN (22)
#define HEARTBEAT_LED_PIN_MODE (IOCON_MODE_INACT)
#define HEARTBEAT_LED_PIN_FUNC (IOCON_FUNC0)

// DHT11
#define GPIO_DHT11_PORT (2) // h{39}
#define GPIO_DHT11_PIN (0)
#define GPIO_DHT11_PIN_PIN_MODE (IOCON_MODE_INACT)
#define GPIO_DHT11_PIN_FUNC (IOCON_FUNC0)


// CERRADURA PUERTA SALIDA O LIMPIA
#define CERRADURA_SALIDA_PORT (0) // h{11}
#define CERRADURA_SALIDA_PIN (18)
#define CERRADURA_SALIDA_PIN_MODE (IOCON_MODE_INACT)
#define CERRADURA_SALIDA_PIN_FUNC (IOCON_FUNC0)

// Add jumper wire on baseboard to control WDT
// WDT is enabled *only* if jumper is in place.
// (Jumper is read at init phase only)
// Port 2, Pin 3 => Connected to "SW2 UP" on LPCXpresso baseboard
#define WDT_JUMPER_PORT (2)
#define WDT_JUMPER_PIN (3)
#define WDT_JUMPER_PIN_MODE (IOCON_MODE_PULLUP)
#define WDT_JUMPER_PIN_FUNC (IOCON_FUNC0)


// ------ Public data type declarations ----------------------------
/** DENTRO DE CADA ARRAY TENGO 0->4 RH INTEGRAL; RH DECIMAL; T INTEGRAL; T DECIMAL*/
typedef struct{
	uint8_t DATA_SENSOR_ARRAY[5];
	uint32_t Valor_Humedad;
	uint32_t Valor_Temperatura;
	uint8_t BufferTemperatura[4];
	uint8_t BufferHumedad[4];
} struct_sensores;

typedef struct{

	 short Aceleracion[3]; // 0 -> X, 1 -> Y, 2 -> Z.
	 short AceleracionAngular[3];  // 0 -> X, 1 -> Y, 2 -> Z.
	 float FloatAceleracion[3];
	 float FloatAceleracionAngular[3];
	 float Orientacion[2];// 0 -> X, 1 -> Y
} struct_acelerometro; //




// ------ Public function prototypes -------------------------------


#endif


/*------------------------------------------------------------------*-
  ---- END OF FILE -------------------------------------------------
-*------------------------------------------------------------------*/
