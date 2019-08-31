// Project header
#include "acelerometro.h"
#include "../main/main.h"
#include "chip.h"
#include "string.h"
#include <stdio.h>


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
	buf[1]=0x10;
	Chip_I2C_MasterSend(I2C1,0x68,buf,2);

}


void Acelerometro_Update(void)
{


		/*Leo el acelerometro*/
		memset(buf,0,LEN_BUF);
		Chip_I2C_MasterCmdRead(I2C1,0x68,0x3B,DataAcelerometro.Aceleracion[0],8); // Aceleración X
		Chip_I2C_MasterCmdRead(I2C1,0x68,0x3D,DataAcelerometro.Aceleracion[1],8); // Aceleración Y
		Chip_I2C_MasterCmdRead(I2C1,0x68,0x3F,DataAcelerometro.Aceleracion[2],8); // Aceleración Z

}
