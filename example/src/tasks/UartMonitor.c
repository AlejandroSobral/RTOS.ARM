

#include "dht11.h"
#include "UartMonitor.h"
#include "acelerometro.h"
#include "dht11.h"
#include "../main/main.h"
#include "string.h"
#include "../c-tds/port_Lpc1769.h"
#include "chip.h"
#include <stdio.h>




void UartMonitor (void)
{
	extern struct_acelerometro DataAcelerometro;
	extern struct_sensores STRUCT_SENSOR;
	char Valores[8];
//extern uint8_t buf[LEN_BUF];
 extern uint32_t enviarSerie;
	static char msg[LEN_BUF];

	Valores[0]=(int) STRUCT_SENSOR.Valor_Humedad;
	Valores[1]=(int) STRUCT_SENSOR.Valor_Temperatura;
	Valores[2]=(int) DataAcelerometro.Aceleracion [0];
	Valores[3]=(int) DataAcelerometro.Aceleracion [1];
	Valores[4]=(int) DataAcelerometro.Aceleracion [2];
	Valores[5]=(int) DataAcelerometro.Orientacion[0];
	Valores[6]=(int) DataAcelerometro.Orientacion[1];
	Valores[7]=(int) DataAcelerometro.Orientacion[2];
	Valores[8]=(int) DataAcelerometro.Orientacion[2];




		if(enviarSerie == 0)
		{
			/*Mando por el puerto serie cada 150ms*/
			memset(msg,0,LEN_BUF);
			sprintf(msg,"0x%02X%02X\t0x%02X%02X\t0x%02X%02X\t0x%02X%02X\r\n",

					Valores[0],Valores[1],
					Valores[3],Valores[4],
					Valores[5],Valores[6],
					Valores[7], Valores[8]);
		}
		if(enviarSerie<MSG_LEN)
		{
			if((Chip_UART_ReadLineStatus(LPC_UART1) & UART_LSR_THRE))
			{
				Chip_UART_SendByte(LPC_UART1,msg[enviarSerie]);
				enviarSerie++;
			}
		}



}


void UartMonitor_Init (void)
{
	/*Inicializo la UART*/
		Chip_IOCON_PinMuxSet(LPC_IOCON,UART1_PORT,UART1_TXD_PIN,IOCON_FUNC1);
		Chip_IOCON_PinMuxSet(LPC_IOCON,UART1_PORT,UART1_RXD_PIN,IOCON_FUNC1);
		Chip_UART_Init(LPC_UART1);
		Chip_UART_SetBaud(LPC_UART1,UART1_BAUDRATE);
		Chip_UART_ConfigData(LPC_UART1, (UART_LCR_WLEN8 | UART_LCR_SBS_1BIT));
		Chip_UART_SetupFIFOS(LPC_UART1, (UART_FCR_FIFO_EN | UART_FCR_TRG_LEV3));
		Chip_UART_TXEnable(LPC_UART1);
}
