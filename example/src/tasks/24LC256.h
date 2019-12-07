#include "chip.h"
#define I2C_SPEED_MMR (400000)
#define I2C_DEVICE_NUM_MEMR	(2)
#define MemoDire 0x50
/** Page Size		*/
#define TamPag 32
/** i2c port used */
#define I2C_PORT I2C1 //
#define TX_OK 0
#define PaginasPorGrabado 4
#define MaxPos 4064 // 127 paginas de 32 cada uno

typedef struct{

	uint8_t dataRX1[TamPag];
	uint8_t dataRX2[TamPag];
	uint8_t dataRX3[TamPag];
	uint8_t dataRX4[TamPag];
} struct_dataRXeeprom;

I2C_STATUS_T Write_24LC (uint8_t * vec,uint16_t address);
I2C_STATUS_T Read_24LC(uint8_t * vec,uint16_t address);
void Ciclo_Memoria_Reading (void);
void Ciclo_Memoria_Working (void);
void Ciclo_Memoria_Erase (void);
void init_memoriai2c (void);
void test_memoria (void);
I2C_STATUS_T GrabaPaginaI2c(uint8_t * frase,uint8_t A,uint8_t B);
I2C_STATUS_T LeePaginaI2c(uint8_t * Buff,uint8_t A,uint8_t B);
void LimpiaBuff(uint8_t * Buff);
void PreparaPaginaUno (void);
void PreparaPaginaDos (void);
void PreparaPaginaTres (void);
void PreparaPaginaCuatro (void);
void PreparaPaginaErase (void);


