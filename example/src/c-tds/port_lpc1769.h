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


// Car wash Controller
#define SENSOR_ON	(true)
#define SENSOR_OFF	(false)
#define MOTOR_ON	(true)
#define MOTOR_OFF	(false)


// Allows Active Low or Active High led to be used (or other wiring variations)
#define LED_ON	(true)
#define LED_OFF	(false)

// Allows NO or NC switch to be used (or other wiring variations)
#define SW_PRESSED		(false)
#define SW_NOT_PRESSED	(true)
#define ABIERTA 0
#define CERRADA 1

#define TRABADO 1
#define DESTRABADO 0

#define RESET 0
#define WORK 1
#define EMERGENCIA 2

// Heartbeat LED
// Connected to "LED2" on LPC1769 board
// Port 0, Pin 22
#define HEARTBEAT_LED_PORT (0)
#define HEARTBEAT_LED_PIN (22)
#define HEARTBEAT_LED_PIN_MODE (IOCON_MODE_INACT)
#define HEARTBEAT_LED_PIN_FUNC (IOCON_FUNC0)

// SWITCH PUERTA LIMPIA
#define GPIO_SWITCH_PUERTA_LIMPIA_PORT (0) // h{38}
#define GPIO_SWITCH_PUERTA_LIMPIA_PIN (4)
#define GPIO_SWITCH_PUERTA_LIMPIA_PIN_MODE (IOCON_MODE_PULLUP)
#define GPIO_SWITCH_PUERTA_LIMPIA_PIN_FUNC (IOCON_FUNC0)


// SWITCH PUERTA SUCIA
#define GPIO_SWITCH_PUERTA_SUCIA_PORT (0) // h{39}
#define GPIO_SWITCH_PUERTA_SUCIA_PIN (5)
#define GPIO_SWITCH_PUERTA_SUCIA_PIN_PIN_MODE (IOCON_MODE_PULLUP)
#define GPIO_SWITCH_PUERTA_SUCIA_PIN_FUNC (IOCON_FUNC0)

// BOTON EMERGENCIA MEDIO
#define GPIO_SWITCH_EMERGENCIA_MEDIO_PORT (2)
#define GPIO_SWITCH_EMERGENCIA_MEDIO__PIN (10) // h{51}
#define GPIO_SWITCH_EMERGENCIA_MEDIO__PIN_MODE (IOCON_MODE_PULLUP)
#define GPIO_SWITCH_EMERGENCIA_MEDIO__PIN_FUNC (IOCON_FUNC0)

// BOTON EMERGENCIA SALA LIMPIA
#define GPIO_SWITCH_EMERGENCIA_LIMPIA_PORT (2)
#define GPIO_SWITCH_EMERGENCIA_LIMPIA_PIN (11) // h{52}
#define GPIO_SWITCH_EMERGENCIA_LIMPIA_PIN_MODE (IOCON_MODE_PULLUP)
#define GPIO_SWITCH_EMERGENCIA_LIMPIA_FUNC (IOCON_FUNC0)

// BOTON EMERGENCIA ENTRADA
#define GPIO_SWITCH_EMERGENCIA_ENTRADA_PORT (2) // h{53}
#define GPIO_SWITCH_EMERGENCIA_ENTRADA_PIN (12)
#define GPIO_SWITCH_EMERGENCIA_ENTRADA_PIN_MODE (IOCON_MODE_PULLUP)
#define GPIO_SWITCH_EMERGENCIA_ENTRADA_PIN_FUNC (IOCON_FUNC0)

// LUZ VERDE PUERTA ENTRADA
#define LUZ_VERDE_ENTRADA_PORT (0) // h{40}
#define LUZ_VERDE_ENTRADA_PIN (10)
#define LUZ_VERDE_ENTRADA_PIN_MODE (IOCON_MODE_INACT)
#define LUZ_VERDE_ENTRADA_PIN_FUNC (IOCON_FUNC0)

// LUZ ROJA PUERTA ENTRADA
#define LUZ_ROJA_ENTRADA_PORT (0) // h{41}
#define LUZ_ROJA_ENTRADA_PIN (11)
#define LUZ_ROJA_ENTRADA_PIN_MODE (IOCON_MODE_INACT)
#define LUZ_ROJA_ENTRADA_PIN_FUNC (IOCON_FUNC0)

// LUZ VERDE PUERTA SALIDA
#define LUZ_VERDE_SALIDA_PORT (2) // h{42}
#define LUZ_VERDE_SALIDA_PIN (0)
#define LUZ_VERDE_SALIDA_PIN_MODE (IOCON_MODE_INACT)
#define LUZ_VERDE_SALIDA_PIN_FUNC (IOCON_FUNC0)

// LUZ ROJA PUERTA SALIDA
#define LUZ_ROJA_SALIDA_PORT (2) // h{43}
#define LUZ_ROJA_SALIDA_PIN (1)
#define LUZ_ROJA_SALIDA_PIN_MODE (IOCON_MODE_INACT)
#define LUZ_ROJA_SALIDA_PIN_FUNC (IOCON_FUNC0)

// CERRADURA PUERTA ENTRADA O SUCIA
#define CERRADURA_ENTRADA_PORT (0) // h{10}
#define CERRADURA_ENTRADA_PIN (17)
#define CERRADURA_ENTRADA_PIN_MODE (IOCON_MODE_INACT)
#define CERRADURA_ENTRADA_PIN_FUNC (IOCON_FUNC0)

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


// ------ Public function prototypes -------------------------------


#endif


/*------------------------------------------------------------------*-
  ---- END OF FILE -------------------------------------------------
-*------------------------------------------------------------------*/
