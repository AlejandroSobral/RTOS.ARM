

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
//0 = AceleracionAngularx
//1=Aceleracion
//2=Orientacion
//3= Humedad
//4= Temperatura
//5=Bateria

uint32_t Grabado, Enviado;

void UartMonitor (void)
{
	extern struct_acelerometro DataAcelerometro;
	extern struct_sensores STRUCT_SENSOR;
	extern uint32_t FlagUmbral[6];


	int Valores[34];
//extern uint8_t buf[LEN_BUF];
 extern uint32_t enviarSerie;
	static char msg[LEN_BUF];

if(FlagUmbral[0]==1||FlagUmbral[1]==1||FlagUmbral[2]==1||FlagUmbral[3]==1||FlagUmbral[4]==1)//Alguno se pasó de la cuenta

{
		Valores[0]=getdigit(STRUCT_SENSOR.Valor_Humedad,0);
		Valores[1]=getdigit(STRUCT_SENSOR.Valor_Humedad,1);
		Valores[2]=getdigit(STRUCT_SENSOR.Valor_Temperatura,0);
		Valores[3]=getdigit(STRUCT_SENSOR.Valor_Temperatura,1);
		Valores[4]=getdigit((int)DataAcelerometro.FloatAceleracion [2],0); //Z Accel
		Valores[5]=getdigit((int)DataAcelerometro.FloatAceleracion [2],1);
		Valores[6]=getdigit((int)DataAcelerometro.FloatAceleracion [2],2);
		Valores[7]=getdigit((int)DataAcelerometro.FloatAceleracion [2],3);
		Valores[8]=getdigit((int)DataAcelerometro.FloatAceleracion [2],4);

		Valores[9]=getdigit((int)DataAcelerometro.FloatAceleracion [1],0); //Y Accel
		Valores[10]=getdigit((int)DataAcelerometro.FloatAceleracion [1],1);
		Valores[11]=getdigit((int)DataAcelerometro.FloatAceleracion [1],2);
		Valores[12]=getdigit((int)DataAcelerometro.FloatAceleracion [1],3);
		Valores[13]=getdigit((int)DataAcelerometro.FloatAceleracion [1],4);

		Valores[14]=getdigit((int)DataAcelerometro.FloatAceleracion [0],0); //X Accel
		Valores[15]=getdigit((int)DataAcelerometro.FloatAceleracion [0],1);
		Valores[16]=getdigit((int)DataAcelerometro.FloatAceleracion [0],2);
		Valores[17]=getdigit((int)DataAcelerometro.FloatAceleracion [0],3);
		Valores[18]=getdigit((int)DataAcelerometro.FloatAceleracion [0],4);

		Valores[19]=getdigit((int)DataAcelerometro.FloatAceleracionAngular [0],0); //X Gryo
		Valores[20]=getdigit((int)DataAcelerometro.FloatAceleracionAngular [0],1);
		Valores[21]=getdigit((int)DataAcelerometro.FloatAceleracionAngular [0],2);



		Valores[23]=getdigit((int)DataAcelerometro.FloatAceleracionAngular [1],0); //Y Gryo
		Valores[24]=getdigit((int)DataAcelerometro.FloatAceleracionAngular [1],1);
		Valores[25]=getdigit((int)DataAcelerometro.FloatAceleracionAngular [1],2);



		Valores[27]=getdigit((int)DataAcelerometro.FloatAceleracionAngular [2],0); //Z Gryo
		Valores[28]=getdigit((int)DataAcelerometro.FloatAceleracionAngular [2],1);
		Valores[29]=getdigit((int)DataAcelerometro.FloatAceleracionAngular [2],2);

		Valores[30]=getdigit((int)DataAcelerometro.Orientacion [0],0);
		Valores[31]=getdigit((int)DataAcelerometro.Orientacion [0],1);

		Valores[32]=getdigit((int)DataAcelerometro.Orientacion [1],0);
		Valores[33]=getdigit((int)DataAcelerometro.Orientacion [1],1);





			if(enviarSerie == 0 && Enviado ==0)
			{
				/*Mando por el puerto serie cada 150ms*/
				memset(msg,0,LEN_BUF); //sprintf(msg,"H%2d%d\tT%2d%d\tZ%5d%d%d%d%d\tY%5d%d%d%d%d\tX%5d%d%d%d%d\r\n",
				sprintf(msg,"H%d%d\tT%d%d\tZ%d%d.%d%d%dm/s\tY%d%d.%d%d%dm/s\tX%d%d.%d%d%dm/s\tXGº%d%d%d\tYGº%d%d%d\tZGº%d%d%d\tXº%d%d\tYº%d%d\r\n",

						Valores[1],Valores[0], //Humedad
						Valores[3], Valores[2],//Temperatura
						Valores[8],Valores[7],Valores[6],Valores[5],Valores[4], //Z Accel
						Valores[13],Valores[12],Valores[11],Valores[10],Valores[9], //Y Accel
						Valores[18],Valores[17],Valores[16],Valores[15],Valores[14], //X Accel
						Valores[21],Valores[20],Valores[19], //X Gyro
						Valores[25],Valores[24],Valores[23], //Y Gyro
						Valores[29],Valores[28],Valores[27], //Z Gyro
						Valores[31],Valores[30], //ºX
						Valores[33],Valores[32] //ºY
											);
			}
			if(enviarSerie<MSG_LEN) //MSG_LENG
			{
				if((Chip_UART_ReadLineStatus(LPC_UART1) & UART_LSR_THRE))
				{
					Chip_UART_SendByte(LPC_UART1,msg[enviarSerie]);
					enviarSerie++;
				}
			}
			else Enviado=1; //Termino de enviar



	}


}
void UartMonitor_Init (void)
{
	/*Inicializo la UART*/
		Chip_IOCON_PinMuxSet(LPC_IOCON,UART1_PORT,UART1_TXD_PIN,IOCON_FUNC2);
		Chip_IOCON_PinMuxSet(LPC_IOCON,UART1_PORT,UART1_RXD_PIN,IOCON_FUNC2);
		Chip_UART_Init(LPC_UART1);
		Chip_UART_SetBaud(LPC_UART1,UART1_BAUDRATE);
		Chip_UART_ConfigData(LPC_UART1, (UART_LCR_WLEN8 | UART_LCR_SBS_1BIT));
		Chip_UART_SetupFIFOS(LPC_UART1, (UART_FCR_FIFO_EN | UART_FCR_TRG_LEV3));
		Chip_UART_TXEnable(LPC_UART1);
}

int getdigit(int num, int n)
{
    int r, t1, t2;

    t1 = pow(10, n+1);
    r = num % t1;

    if (n > 0)
    {
        t2 = pow(10, n);
        r = r / t2;
    }

    return r;
}
