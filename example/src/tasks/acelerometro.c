// Project header
#include "acelerometro.h"
#include "../main/main.h"
#include "chip.h"
#include "string.h"
#include <stdio.h>
#include "math.h" //

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
	//Inicializo Acelerometro i2c
	Chip_I2C_Init(I2C_DEVICE_NUM_ACEL);
	Chip_I2C_SetClockRate(I2C_DEVICE_NUM_ACEL,I2C_SPEED_ACELEROMETRO);
	Chip_I2C_SetMasterEventHandler(I2C_DEVICE_1,Chip_I2C_EventHandlerPolling); // Acelerometro
	//Chip_I2C_SetMasterEventHandler(I2C_DEVICE_2,Chip_I2C_EventHandlerPolling)








	/*Enciendo el Acelerómetro*/
	/*Espero que arranque y lo configuro*/
	//for(i=0;i<1500000;i++);
	memset(buf,0,sizeof(uint8_t)*LEN_BUF);
	buf[0]=0x6B;
	Chip_I2C_MasterSend(I2C1,0x68,buf,2);
	memset(buf,0,sizeof(uint8_t)*LEN_BUF);
	buf[0]=0x1C; //Acá configuró el acelerómetro
	buf[1]=0x00; //2g maxima escala
	memset(buf,0,sizeof(uint8_t)*LEN_BUF);
		buf[0]=0x1B; //Acá configuró el giróscopo 250g/s
		buf[1]=0x00;

	Chip_I2C_MasterSend(I2C1,0x68,buf,2);




}


void Acelerometro_Update(void)
{
static uint8_t ContadorSerie;
extern uint32_t enviarSerie;
extern uint32_t FlagUmbral[6];


	if(ContadorSerie > 40)
		{enviarSerie = 0;
		ContadorSerie = 0;
		}


	if (enviarSerie > 0) {
		ContadorSerie++;
	}


		/*Leo el acelerometro*/
		memset(buf,0,LEN_BUF);

		Chip_I2C_MasterCmdRead(I2C1,0x68,0x3B,buf,6);

				DataAcelerometro.Aceleracion[0] = (((short)buf[0])<<8)|buf[1];
				DataAcelerometro.Aceleracion[1] = (((short)buf[2])<<8)|buf[3];
				DataAcelerometro.Aceleracion[2] = (((short)buf[4])<<8)|buf[5];


					// En m/s

					DataAcelerometro.FloatAceleracion[0] = DataAcelerometro.Aceleracion[0];
					DataAcelerometro.FloatAceleracion[0] = DataAcelerometro.FloatAceleracion[0]*Gravedad;
					DataAcelerometro.FloatAceleracion[0] = DataAcelerometro.FloatAceleracion[0]/16384;


					DataAcelerometro.FloatAceleracion[1] = DataAcelerometro.Aceleracion[1];
					DataAcelerometro.FloatAceleracion[1] = DataAcelerometro.FloatAceleracion[1]*Gravedad;
					DataAcelerometro.FloatAceleracion[1] = DataAcelerometro.FloatAceleracion[1]/16384;

					DataAcelerometro.FloatAceleracion[2] = DataAcelerometro.Aceleracion[2];
					DataAcelerometro.FloatAceleracion[2] = DataAcelerometro.FloatAceleracion[2]*Gravedad;
					DataAcelerometro.FloatAceleracion[2] = DataAcelerometro.FloatAceleracion[2]/16384;

					if(DataAcelerometro.FloatAceleracion[0]<0)DataAcelerometro.FloatAceleracion[0] = -DataAcelerometro.FloatAceleracion[0];
					if(DataAcelerometro.FloatAceleracion[1]<0)DataAcelerometro.FloatAceleracion[1] = -DataAcelerometro.FloatAceleracion[1];
					if(DataAcelerometro.FloatAceleracion[2]<0)DataAcelerometro.FloatAceleracion[2] = -DataAcelerometro.FloatAceleracion[2];




					memset(buf,0,LEN_BUF);

		Chip_I2C_MasterCmdRead(I2C1,0x68,0x43,buf,6);

						DataAcelerometro.AceleracionAngular[0] = (((short)buf[0])<<8)|buf[1];
						DataAcelerometro.AceleracionAngular[1] = (((short)buf[2])<<8)|buf[3];
						DataAcelerometro.AceleracionAngular[2] = (((short)buf[4])<<8)|buf[5];

					DataAcelerometro.FloatAceleracionAngular[2] = DataAcelerometro.AceleracionAngular[2];
					DataAcelerometro.FloatAceleracionAngular[2] = DataAcelerometro.FloatAceleracionAngular[2]*250; // º/s
					DataAcelerometro.FloatAceleracionAngular[2] = DataAcelerometro.FloatAceleracionAngular[2]/(16384); // º/s

					DataAcelerometro.FloatAceleracionAngular[1] = DataAcelerometro.AceleracionAngular[1];
					DataAcelerometro.FloatAceleracionAngular[1] = DataAcelerometro.FloatAceleracionAngular[1]*250; // º/s
			        DataAcelerometro.FloatAceleracionAngular[1] = DataAcelerometro.FloatAceleracionAngular[1]/(16384); // º/s

			        DataAcelerometro.FloatAceleracionAngular[0] = DataAcelerometro.AceleracionAngular[0];
			        DataAcelerometro.FloatAceleracionAngular[0] = DataAcelerometro.FloatAceleracionAngular[0]*250; // º/s
			        DataAcelerometro.FloatAceleracionAngular[0] = DataAcelerometro.FloatAceleracionAngular[0]/(16384); // º/s

			        DataAcelerometro.Orientacion[0] = 180 * atan (DataAcelerometro.FloatAceleracion[0]/1000/sqrt(DataAcelerometro.FloatAceleracion[1]/1000*DataAcelerometro.FloatAceleracion[1]/1000 + DataAcelerometro.FloatAceleracion[2]/1000*DataAcelerometro.FloatAceleracion[2]/1000))/3.1415;
			        DataAcelerometro.Orientacion[1] = 180 * atan (DataAcelerometro.FloatAceleracion[1]/1000/sqrt(DataAcelerometro.FloatAceleracion[0]/1000*DataAcelerometro.FloatAceleracion[0]/1000 + DataAcelerometro.FloatAceleracion[2]/1000*DataAcelerometro.FloatAceleracion[2]/1000))/3.1415;

			        if(DataAcelerometro.FloatAceleracionAngular[0]<0)DataAcelerometro.FloatAceleracionAngular[0] = -DataAcelerometro.FloatAceleracionAngular[0];
			        if(DataAcelerometro.FloatAceleracionAngular[1]<0)DataAcelerometro.FloatAceleracionAngular[1] = -DataAcelerometro.FloatAceleracionAngular[1];
			        if(DataAcelerometro.FloatAceleracionAngular[2]<0){
			        	DataAcelerometro.FloatAceleracionAngular[2] = -DataAcelerometro.FloatAceleracionAngular[2];
			        	FlagUmbral[2] = 1; // INVERTIDO 180º

			        }





}





