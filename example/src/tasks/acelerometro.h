
#ifndef _ACELEROMETRO_H
#define _ACELEROMETRO_H 1


#define I2C_DEVICE		(I2C1)
#define I2C_DEVICE_NUM	(1)
#define I2C_DEVICE_PORT	(0)
#define I2C_SDA_PIN		(0)
#define I2C_SCL_PIN		(1)
#define I2C_SPEED		(400000)
#define TIC_ACEL		(20)
#define LEN_BUF			(80)
#define UART1_PORT		(0)
#define UART1_TXD_PIN	(15)
#define UART1_RXD_PIN	(16)
#define UART1_BAUDRATE	(9600)
#define MSG_LEN			(80)
#define Gravedad	9827


// ------ Public constants -----------------------------------------


// ------ Public data type declarations ----------------------------


// ------ Public function prototypes -------------------------------
void Acelerometro_Init(void);
void Acelerometro_Update(void);


#endif

