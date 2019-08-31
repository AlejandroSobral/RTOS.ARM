// Project header
#include "acelerometro.h"
#include "../main/main.h"
#include "chip.h"
#include "string.h"
#include <stdio.h>

uint32_t enviarSerie;
uint8_t buf[LEN_BUF];
uint32_t i;
struct_acelerometro DataAcelerometro;

/*Inicializo el I2C*/
void Acelerometro_Init (void){

	Chip_IOCON_PinMux(LPC_IOCON, I2C_DEVICE_PORT, I2C_SDA_PIN, IOCON_MODE_INACT, IOCON_FUNC3);
	Chip_IOCON_PinMux(LPC_IOCON, I2C_DEVICE_PORT, I2C_SCL_PIN, IOCON_MODE_INACT, IOCON_FUNC3);
	Chip_IOCON_EnableOD(LPC_IOCON,I2C_DEVICE_PORT, I2C_SDA_PIN);
	Chip_IOCON_EnableOD(LPC_IOCON,I2C_DEVICE_PORT, I2C_SDA_PIN);
	Chip_I2C_Init(I2C_DEVICE_NUM);
	Chip_I2C_SetClockRate(I2C_DEVICE_NUM,I2C_SPEED);
	Chip_I2C_SetMasterEventHandler(I2C_DEVICE,Chip_I2C_EventHandlerPolling);


	/*Enciendo el Acelerómetro*/
	/*Espero que arranque y lo configuro*/
	for(i=0;i<1500000;i++);
	memset(buf,0,sizeof(uint8_t)*LEN_BUF);
	buf[0]=0x6B;
	Chip_I2C_MasterSend(I2C1,0x68,buf,2);
	memset(buf,0,sizeof(uint8_t)*LEN_BUF);
	buf[0]=0x1C; //Acá configuró el acelerómetro
	buf[1]=0x10; //8g maxima escala
	memset(buf,0,sizeof(uint8_t)*LEN_BUF);
		buf[0]=0x1B; //Acá configuró el giróscopo 250g/s
		buf[1]=0x00;

	Chip_I2C_MasterSend(I2C1,0x68,buf,2);



}


void Acelerometro_Update(void)
{
static uint8_t ContadorSerie;
extern uint32_t enviarSerie;


	if(ContadorSerie > 50)
		{enviarSerie = 0;
		ContadorSerie = 0;
		}


	if (enviarSerie > 0) {
		ContadorSerie++;
	}


		/*Leo el acelerometro*/
		memset(buf,0,LEN_BUF);

		Chip_I2C_MasterCmdRead(I2C1,0x68,0x3B,buf,6);
					DataAcelerometro.Aceleracion[0] = ((buf[0])<<8)|buf[1];
					DataAcelerometro.Aceleracion[1] = (((buf[2])<<8)|buf[3]);
					DataAcelerometro.Aceleracion[2] = (((buf[4])<<8)|buf[5]);


					// En m/s
					DataAcelerometro.Aceleracion[0] = DataAcelerometro.Aceleracion[0]/32768;
					DataAcelerometro.Aceleracion[0] = DataAcelerometro.Aceleracion[0]*2*Gravedad;
					DataAcelerometro.Aceleracion[1] = DataAcelerometro.Aceleracion[1]/32768;
					DataAcelerometro.Aceleracion[1] = DataAcelerometro.Aceleracion[1]*2*Gravedad;
					DataAcelerometro.Aceleracion[2] = DataAcelerometro.Aceleracion[2]/32768;
					DataAcelerometro.Aceleracion[2] = DataAcelerometro.Aceleracion[2]*2*Gravedad;


					if(DataAcelerometro.Aceleracion[0]<0)DataAcelerometro.Aceleracion[0] = -DataAcelerometro.Aceleracion[0];
					if(DataAcelerometro.Aceleracion[1]<0)DataAcelerometro.Aceleracion[1] = -DataAcelerometro.Aceleracion[1];
					if(DataAcelerometro.Aceleracion[2]<0)DataAcelerometro.Aceleracion[2] = -DataAcelerometro.Aceleracion[2];

					memset(buf,0,LEN_BUF);

		Chip_I2C_MasterCmdRead(I2C1,0x68,0x43,buf,6);
					DataAcelerometro.Orientacion[0] = ((buf[0])<<8)|buf[1];
					DataAcelerometro.Orientacion[1] = ((buf[2])<<8)|buf[3];
					DataAcelerometro.Orientacion[2] = ((buf[4])<<8)|buf[5];

					DataAcelerometro.Orientacion[2] = DataAcelerometro.Orientacion[2]/(32768); // º/s
					DataAcelerometro.Orientacion[2] = DataAcelerometro.Orientacion[2]*250; // º/s
					DataAcelerometro.Orientacion[1] = DataAcelerometro.Orientacion[1]/(32768); // º/s
					DataAcelerometro.Orientacion[1] = DataAcelerometro.Orientacion[1]*250; // º/s
					DataAcelerometro.Orientacion[0] = DataAcelerometro.Orientacion[0]/(32768); // º/s
					DataAcelerometro.Orientacion[0] = DataAcelerometro.Orientacion[0]*250; // º/s



}





