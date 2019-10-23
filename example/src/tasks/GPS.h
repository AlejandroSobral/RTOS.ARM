#define UART2_PORT		(0)
#define UART2_TXD_PIN	(10)
#define UART2_RXD_PIN	(11)
#define UART2_BAUDRATE	(9600) // Definir que UART usamos para BT y cual para GPS



// Pasar lo de leeADC para acá

// ------ Public constants -----------------------------------------


// ------ Public data type declarations ----------------------------


// ------ Public function prototypes -------------------------------
void ADC0_Init(void);
void EstadoBateria(void);

void UartGPSInit (void);
void TareaLeeGPS(void);
void TareaEnviaSerie(void);




