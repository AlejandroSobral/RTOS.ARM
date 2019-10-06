#include "logger.h"
#include "24LC256.h"
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
#include "C:\Users\Alejandro\git2\lpc_chip_175x_6x\inc\eeprom_17xx_40xx.h"

//FlagUmbral[i]
//0 = AceleracionAngular
//1=Aceleracion
//2=Orientacion
//3= Humedad
//4= Temperatura
//5=Bateria
uint32_t UltimaPosicionMemoria; // Global
uint8_t MSB, LSB;
static uint8_t BufferTXEeprom[TamPag]; uint8_t BufferRXEeprom[TamPag];
static uint8_t dataRX[TamPag], dataTX[TamPag];
I2C_STATUS_T i2c_state;
static uint32_t UltimaMemoriaGrabada;
static uint32_t UltimaMemoriaLeida;

extern struct_acelerometro DataAcelerometro;

void init_memoriai2c (void)
{
	//Inicializo i2c de la memoria
	Chip_IOCON_PinMux(LPC_IOCON, I2C_DEVICE_PORT, I2C_SDA_PIN, IOCON_MODE_INACT, IOCON_FUNC3);
	Chip_IOCON_PinMux(LPC_IOCON, I2C_DEVICE_PORT, I2C_SCL_PIN, IOCON_MODE_INACT, IOCON_FUNC3);
	Chip_IOCON_EnableOD(LPC_IOCON,I2C_DEVICE_PORT, I2C_SDA_PIN);
	Chip_IOCON_EnableOD(LPC_IOCON,I2C_DEVICE_PORT, I2C_SDA_PIN);
	Chip_I2C_Init(I2C_DEVICE_NUM_MEMR);
	Chip_I2C_SetClockRate(I2C_DEVICE_NUM_MEMR,I2C_SPEED_MMR);
	Chip_I2C_SetMasterEventHandler(I2C_DEVICE_2,Chip_I2C_EventHandlerPolling);

}



void ciclo_memoria (void)
{





	uint32_t i;
	extern uint32_t FlagUmbral[NMROFLAGS];

	if(UltimaMemoriaGrabada > (MaxPos)) UltimaMemoriaGrabada = 0; //Roll-over de memoria
	FlagUmbral[0] = 1;
	for(i=0; i<NMROFLAGS; i++)
	{ if(FlagUmbral[i] == 1){

		PreparaPaginaUno();



			//if( GrabaPaginaI2c(dataTX,MSB,LSB) == TX_OK);
			//UltimaPosicionMemoria++;
			i2c_state = Write_24LC(dataTX, UltimaMemoriaGrabada);
			if(i2c_state == I2C_STATUS_DONE ){UltimaMemoriaGrabada+=TamPag;}

			//LeePaginaI2c(dataRX,MSB, LSB);
			break;
		}
	}

}

I2C_STATUS_T  Write_24LC(uint8_t * vec,uint16_t address){

	uint8_t x=0;
	I2C_XFER_T xfer;


	BufferTXEeprom[0] = (uint8_t)(address >> 8);
	BufferTXEeprom[1] = (uint8_t)(address & 0x00FF);

	for(x=2;x<TamPag;x++){
		BufferTXEeprom[x]=vec[x-2];
	}


	 xfer.rxBuff = 0;
	 xfer.rxSz = 0;
	 xfer.slaveAddr = 0x50;
	 xfer.status = 0;
	 xfer.txBuff = BufferTXEeprom;
	 xfer.txSz = TamPag;

	 Chip_I2C_MasterTransfer(I2C1, &xfer);
	// Chip_I2C_MasterSend(DEFAULT_I2C, xfer.slaveAddr, xfer.txBuff, xfer.txSz);
	 return xfer.status;

}

void ciclo_memoria_read (void){

	i2c_state = 0;
	i2c_state = Read_24LC(dataRX, 0);
	if(i2c_state == I2C_STATUS_DONE ){UltimaMemoriaLeida+=TamPag;}

}

void LimpiaBuff(uint8_t * Buff) {
	uint8_t i;

	/*cleaning Buffer*/
	for (i=0;i<TamPag-2;i++)
		Buff[i]=0;
}


I2C_STATUS_T Read_24LC(uint8_t * vec,uint16_t address)
{

	uint8_t x=0;

		I2C_XFER_T xfer;

			BufferTXEeprom[0] = (uint8_t)(address >> 8);
			BufferTXEeprom[1] = (uint8_t)(address & 0x00FF);
			 xfer.rxBuff = BufferRXEeprom;
			 xfer.rxSz = TamPag;
			 xfer.slaveAddr = 0x50;
			 xfer.status = 0;
			 xfer.txBuff = BufferTXEeprom;
			 xfer.txSz = 2;

	 Chip_I2C_MasterTransfer(I2C1, &xfer);


	 for(x=0;x<TamPag;x++){
		 dataRX[x]=BufferRXEeprom[x];
	 }
	 return xfer.status;

}

void PreparaPaginaUno (void) //Aceleraciones G y angulares
{
	//0 -> X, 1 -> Y, 2 -> Z.
	dataTX[0] = 'A';
	dataTX[1] = 'X';
	dataTX[2] = getdigit((int)DataAcelerometro.FloatAceleracionAngular [0],0);
	dataTX[3] = getdigit((int)DataAcelerometro.FloatAceleracionAngular [0],1);
	dataTX[4] = getdigit((int)DataAcelerometro.FloatAceleracionAngular [0],2);
	dataTX[5] = getdigit((int)DataAcelerometro.FloatAceleracionAngular [0],3);
	dataTX[6] = 'A';
	dataTX[7] = 'Y';
	dataTX[8] = getdigit((int)DataAcelerometro.FloatAceleracionAngular [1],0);
	dataTX[9] = getdigit((int)DataAcelerometro.FloatAceleracionAngular [1],1);
	dataTX[10] = getdigit((int)DataAcelerometro.FloatAceleracionAngular [1],2);
	dataTX[11] = getdigit((int)DataAcelerometro.FloatAceleracionAngular [1],3);
	dataTX[12] = 'A';
	dataTX[13] = 'Z';
	dataTX[14] = getdigit((int)DataAcelerometro.FloatAceleracionAngular [2],0);
	dataTX[15] = getdigit((int)DataAcelerometro.FloatAceleracionAngular [2],1);
	dataTX[16] = getdigit((int)DataAcelerometro.FloatAceleracionAngular [2],2);
	dataTX[17] = getdigit((int)DataAcelerometro.FloatAceleracionAngular [2],3);
	dataTX[18] = 'G';
	dataTX[19] = 'X';
	dataTX[20] = getdigit((int)DataAcelerometro.FloatAceleracion [0],0);
	dataTX[21] = getdigit((int)DataAcelerometro.FloatAceleracion [0],1);
	dataTX[22] = getdigit((int)DataAcelerometro.FloatAceleracion [0],2);
	dataTX[23] = 'G';
	dataTX[24] = 'Y';
	dataTX[25] = getdigit((int)DataAcelerometro.FloatAceleracion [1],0);
	dataTX[26] = getdigit((int)DataAcelerometro.FloatAceleracion [1],1);
	dataTX[27] = getdigit((int)DataAcelerometro.FloatAceleracion [1],2);
	dataTX[28] = 'G';
	dataTX[29] = 'Z';
	dataTX[30] = getdigit((int)DataAcelerometro.FloatAceleracion [0],0);
	dataTX[31] = getdigit((int)DataAcelerometro.FloatAceleracion [2],1);
	dataTX[32] = getdigit((int)DataAcelerometro.FloatAceleracion [2],2);
}







