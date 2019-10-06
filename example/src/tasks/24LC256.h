#include "chip.h"
#define I2C_SPEED_MMR (400000)
#define I2C_DEVICE_NUM_MEMR	(2)
#define MemoDire 0x50
/** Page Size		*/
#define TamPag 32
/** i2c port used */
#define I2C_PORT I2C1
#define TX_OK 0
#define MaxPos 4064 // 127 paginas de 32 cada uno

I2C_STATUS_T Write_24LC (uint8_t * vec,uint16_t address);
I2C_STATUS_T Read_24LC(uint8_t * vec,uint16_t address);
void ciclo_memoria_read (void);
void ciclo_memoria (void);
void init_memoriai2c (void);
void test_memoria (void);
I2C_STATUS_T GrabaPaginaI2c(uint8_t * frase,uint8_t A,uint8_t B);
I2C_STATUS_T LeePaginaI2c(uint8_t * Buff,uint8_t A,uint8_t B);
void LimpiaBuff(uint8_t * Buff);
void PreparaPaginaUno (void);


