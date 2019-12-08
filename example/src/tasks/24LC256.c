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
#include <stdio.h> //
#include <math.h>
//#include "C:\Users\Alejandro\git2\lpc_chip_175x_6x\inc\eeprom_17xx_40xx.h"

//FlagUmbral[i]
//0 = AceleracionAngular
//1=Aceleracion
//2=Orientacion
//3= Humedad
//4= Temperatura
//5=Bateria
//6=Invertido
uint32_t UltimaPosicionMemoria; // Global
uint8_t MSB, LSB;
static uint8_t BufferTXEeprom[TamPag]; uint8_t BufferRXEeprom[TamPag];
// BufferRXeeprom es DONDE RECIBE LA MEMORIA!!!
static uint8_t dataTX[TamPag];
I2C_STATUS_T i2c_state;
static uint32_t UltimaMemoriaGrabada;
static uint32_t UltimaMemoriaLeida;
static uint32_t UltimaMemoriaErase;
static struct_dataRXeeprom dataRXeeprom_Read[CantidadMaximaGolpes];
static uint32_t IndicePaginaRead;



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


void Ciclo_Memoria_Erase(void){

extern struct_dataRXeeprom dataRXeeprom_Read[CantidadMaximaGolpes];
uint32_t i = 0, j = 0;

	if(UltimaMemoriaErase > (MaxPos)) UltimaMemoriaErase = 0;


while(UltimaMemoriaErase < MaxPos){
	PreparaPaginaErase(); //Acelerometro angular y fuerza G
	i2c_state = Write_24LC(dataTX, UltimaMemoriaErase);


	while(i2c_state != I2C_STATUS_DONE){
		i2c_state = Write_24LC(dataTX, UltimaMemoriaErase);
	}

	if(i2c_state == I2C_STATUS_DONE ){UltimaMemoriaErase+=TamPag;}

}
UltimaMemoriaErase = 0;

for (i = 0; i<CantidadMaximaGolpes; i++)
	{	for(j = 0; j<TamPag ; j++)
		{
		dataRXeeprom_Read[i].dataRX1[j] = 0 ;
		}
		j = 0;
		for(j = 0; j<TamPag ; j++)
		{
		dataRXeeprom_Read[i].dataRX2[j] = 0 ;
		}
		j = 0;
		for(j = 0; j<TamPag ; j++)
		{
		dataRXeeprom_Read[i].dataRX3[j] = 0 ;
		}
		j = 0;
		for(j = 0; j<TamPag ; j++)
		{
		dataRXeeprom_Read[i].dataRX4[j] = 0 ;
		}

		}
}


void Ciclo_Memoria_Working (void)
{

static uint32_t IndicePaginaWrite;



	if(UltimaMemoriaGrabada > (MaxPos)) UltimaMemoriaGrabada = 0; //Roll-over de memoria


	for(IndicePaginaWrite = 0; IndicePaginaWrite<PaginasPorGolpe ; IndicePaginaWrite++)
	{

		switch(IndicePaginaWrite)
			{// Esto se hace para grabar una pagina por ciclo
			case 0:
			LimpiaBuff(dataTX);
			PreparaPaginaUno(); //Acelerometro angular y fuerza G
			i2c_state = Write_24LC(dataTX, UltimaMemoriaGrabada);
			while(i2c_state != I2C_STATUS_DONE){
				i2c_state = Write_24LC(dataTX, UltimaMemoriaGrabada);
			}

			if(i2c_state == I2C_STATUS_DONE ){UltimaMemoriaGrabada+=TamPag;}

			break;
			case 1:
			LimpiaBuff(dataTX);
			PreparaPaginaDos(); // Orientacion, H&T
			i2c_state = Write_24LC(dataTX, UltimaMemoriaGrabada);
			while(i2c_state != I2C_STATUS_DONE){
				i2c_state = Write_24LC(dataTX, UltimaMemoriaGrabada);
			}

			if(i2c_state == I2C_STATUS_DONE ){UltimaMemoriaGrabada+=TamPag;}
			break;
			case 2:
			LimpiaBuff(dataTX);
			PreparaPaginaTres(); // GPS
			i2c_state = Write_24LC(dataTX, UltimaMemoriaGrabada);
			while(i2c_state != I2C_STATUS_DONE){
				i2c_state = Write_24LC(dataTX, UltimaMemoriaGrabada);
			}

			if(i2c_state == I2C_STATUS_DONE ){UltimaMemoriaGrabada+=TamPag;}

			break;
			case 3:
			LimpiaBuff(dataTX);
			PreparaPaginaTres(); // GPS
			i2c_state = Write_24LC(dataTX, UltimaMemoriaGrabada);
			while(i2c_state != I2C_STATUS_DONE){
				i2c_state = Write_24LC(dataTX, UltimaMemoriaGrabada);
			}

			if(i2c_state == I2C_STATUS_DONE ){UltimaMemoriaGrabada+=TamPag;}
			break;
				//LeePaginaI2c(dataRX,MSB, LSB);

			}
	}



}



void Ciclo_Memoria_Reading (void){

extern struct_dataRXeeprom dataRXeeprom_Read[CantidadMaximaGolpes]; // 30 son los golpes que puede registar, 127 páginas / 4
extern uint32_t GolpesLeidos;
extern uint32_t IndicePaginaRead;


if(UltimaMemoriaLeida > (MaxPos)){ UltimaMemoriaLeida = 0; } //Roll-over de memoria


//	if(UltimaMemoriaLeida > UltimaMemoriaGrabada)
//			{
//			if(UltimaMemoriaGrabada == 0)
//					{UltimaMemoriaLeida = 0;}
////			else
////				{UltimaMemoriaLeida = UltimaMemoriaGrabada - TamPag;}
//			}
	IndicePaginaRead=0;
for(IndicePaginaRead = 0; IndicePaginaRead<PaginasPorGolpe ; IndicePaginaRead++)
{

	switch (IndicePaginaRead)
	{

	case 0:
	i2c_state = 0;
	LimpiaBuff(BufferRXEeprom);
	i2c_state = Read_24LC(dataRXeeprom_Read[GolpesLeidos].dataRX1, UltimaMemoriaLeida);

	while(i2c_state != I2C_STATUS_DONE){
		Read_24LC(dataRXeeprom_Read[GolpesLeidos].dataRX1, UltimaMemoriaLeida);
	}

	if(i2c_state == I2C_STATUS_DONE ){UltimaMemoriaLeida+=TamPag;}


	break;
	case 1:
	i2c_state = 0;
	LimpiaBuff(BufferRXEeprom);
	i2c_state = Read_24LC(dataRXeeprom_Read[GolpesLeidos].dataRX2, UltimaMemoriaLeida);

	while(i2c_state != I2C_STATUS_DONE){
		Read_24LC(dataRXeeprom_Read[GolpesLeidos].dataRX1, UltimaMemoriaLeida);
	}

	if(i2c_state == I2C_STATUS_DONE ){UltimaMemoriaLeida+=TamPag;}


	break;
	case 2:
	i2c_state = 0;
	LimpiaBuff(BufferRXEeprom);
	i2c_state = Read_24LC(dataRXeeprom_Read[GolpesLeidos].dataRX3, UltimaMemoriaLeida);

	while(i2c_state != I2C_STATUS_DONE){
		Read_24LC(dataRXeeprom_Read[GolpesLeidos].dataRX1, UltimaMemoriaLeida);
	}

	if(i2c_state == I2C_STATUS_DONE ){UltimaMemoriaLeida+=TamPag;}


	break;
	case 3:
	i2c_state = 0;
	LimpiaBuff(BufferRXEeprom);
	i2c_state = Read_24LC(dataRXeeprom_Read[GolpesLeidos].dataRX4, UltimaMemoriaLeida);

	while(i2c_state != I2C_STATUS_DONE){
		Read_24LC(dataRXeeprom_Read[GolpesLeidos].dataRX1, UltimaMemoriaLeida);
	}

	if(i2c_state == I2C_STATUS_DONE ){UltimaMemoriaLeida+=TamPag;}
	GolpesLeidos++;
	break;
		}
	}
}

void LimpiaBuff(uint8_t * Buff) {
	uint8_t i;

	/*cleaning Buffer*/
	for (i=0;i<TamPag-2;i++)
		Buff[i]=0;
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
		 vec[x]=BufferRXEeprom[x];
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
	dataTX[6] = 'Y';
	dataTX[7] = getdigit((int)DataAcelerometro.FloatAceleracionAngular [1],0);
	dataTX[8] = getdigit((int)DataAcelerometro.FloatAceleracionAngular [1],1);
	dataTX[9] = getdigit((int)DataAcelerometro.FloatAceleracionAngular [1],2);
	dataTX[10] = getdigit((int)DataAcelerometro.FloatAceleracionAngular [1],3);
	dataTX[11] = 'Z';
	dataTX[12] = getdigit((int)DataAcelerometro.FloatAceleracionAngular [2],0);
	dataTX[13] = getdigit((int)DataAcelerometro.FloatAceleracionAngular [2],1);
	dataTX[14] = getdigit((int)DataAcelerometro.FloatAceleracionAngular [2],2);
	dataTX[15] = getdigit((int)DataAcelerometro.FloatAceleracionAngular [2],3);
	dataTX[16] = 'G';
	dataTX[17] = 'X';
	dataTX[18] = getdigit((int)DataAcelerometro.FloatAceleracion [0],0);
	dataTX[19] = getdigit((int)DataAcelerometro.FloatAceleracion [0],1);
	dataTX[20] = getdigit((int)DataAcelerometro.FloatAceleracion [0],2);
	dataTX[21] = 'Y';
	dataTX[22] = getdigit((int)DataAcelerometro.FloatAceleracion [1],0);
	dataTX[23] = getdigit((int)DataAcelerometro.FloatAceleracion [1],1);
	dataTX[24] = getdigit((int)DataAcelerometro.FloatAceleracion [1],2);
	dataTX[25] = 'Z';
	dataTX[26] = getdigit((int)DataAcelerometro.FloatAceleracion [0],0);
	dataTX[27] = getdigit((int)DataAcelerometro.FloatAceleracion [2],1);
	dataTX[28] = getdigit((int)DataAcelerometro.FloatAceleracion [2],2);

}

void PreparaPaginaDos (void) // Orientacion, H&T
{uint32_t i=0;
extern uint32_t FlagUmbral[NMROFLAGS];
	extern struct_sensores STRUCT_SENSOR;
	dataTX[0] = 'O';
	dataTX[1] = 'X';
	dataTX[2] = getdigit((int)DataAcelerometro.Orientacion [0],0); // X
	dataTX[3] = getdigit((int)DataAcelerometro.Orientacion [0],1);
	dataTX[4] = 'Y';
	dataTX[5] = getdigit((int)DataAcelerometro.Orientacion [1],0); // Y
	dataTX[6] = getdigit((int)DataAcelerometro.Orientacion [1],1);
	dataTX[7] = 'T';
	dataTX[8] = getdigit((int)STRUCT_SENSOR.Valor_Temperatura,0);
	dataTX[9] = getdigit((int)STRUCT_SENSOR.Valor_Temperatura,1);
	dataTX[10] = 'H';
	dataTX[11] = getdigit((int)STRUCT_SENSOR.Valor_Humedad,1);
	dataTX[12] = getdigit((int)STRUCT_SENSOR.Valor_Humedad,0);


	for(i=0; i<NMROFLAGS; i++)
	{
		if(FlagUmbral[i] == 1)
		{

			dataTX[i+13] = 1;
		}
	}//ACA VOY GRABANDO CUALES FLAGS SON LOS QUE QUEDARON LEVANTADOS!


}


void PreparaPaginaTres (void) //Del GPS: Hora y Latitud
{
	extern char hora[10];
	extern char latitud[18];

	dataTX[0]= hora[0];
	dataTX[1]= hora[1];
	dataTX[2]= hora[2];
	dataTX[3]= hora[3];
	dataTX[4]= hora[4];
	dataTX[5]= hora[5];
	dataTX[6]= hora[6];
	dataTX[7]= hora[7];
	dataTX[8]= hora[8];
	dataTX[9]= hora[9];
	dataTX[10]= latitud[0];
	dataTX[11]= latitud[1];
	dataTX[12]= latitud[2];
	dataTX[13]= latitud[3];
	dataTX[14]= latitud[4];
//	dataTX[9]= 'A';
//	dataTX[10]= 'B';
//	dataTX[11]= 'C';
//	dataTX[12]= 'D';
//	dataTX[13]= 'E';
//	dataTX[14]= 'F';
	dataTX[15]= latitud[5];
	dataTX[16]= latitud[6];
	dataTX[17]= latitud[7];
	dataTX[18]= latitud[8];
	dataTX[19]= latitud[9];
	dataTX[20]= latitud[10];
	dataTX[21]= latitud[11];
	dataTX[22]= latitud[12];
	dataTX[23]= latitud[13];
	dataTX[24]= latitud[14];
	dataTX[25]= latitud[15];
	dataTX[26]= latitud[16];
	dataTX[27]= latitud[17];

}


void PreparaPaginaCuatro (void) //Del GPS: Fecha y Longitud
{
	extern char longitud[18];
	extern char fecha[10];

	dataTX[0]= fecha[0];
	dataTX[1]= fecha[1];
	dataTX[2]= fecha[2];
	dataTX[3]= fecha[3];
	dataTX[4]= fecha[4];
	dataTX[5]= fecha[5];
	dataTX[6]= fecha[6];
	dataTX[7]= fecha[7];
	dataTX[8]= fecha[8];
	dataTX[9]= fecha[9];
	dataTX[10]= longitud[0];
	dataTX[11]= longitud[1];
	dataTX[12]= longitud[2];
	dataTX[13]= longitud[3];
	dataTX[14]= longitud[4];
	dataTX[15]= longitud[5];
	dataTX[16]= longitud[6];
	dataTX[17]= longitud[7];
	dataTX[18]= longitud[8];
	dataTX[19]= longitud[9];
	dataTX[20]= longitud[10];
	dataTX[21]= longitud[11];
	dataTX[22]= longitud[12];
	dataTX[23]= longitud[13];
	dataTX[24]= longitud[14];
	dataTX[25]= longitud[15];
	dataTX[26]= longitud[16];
	dataTX[27]= longitud[17];
}

void PreparaPaginaErase (void) //Borro todo
{
	uint32_t i = 0;
	for (i=0;i<TamPag;i++)
	{	dataTX[i]= 'F';
	}
}


