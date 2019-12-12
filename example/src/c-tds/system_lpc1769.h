/*------------------------------------------------------------------*-

    system_lpc1769.h (Released 2019-03)

  ------------------------------------------------------------------

    See system_lpc1769.c for details.

-*---------------//---------------------------------------------------*/
#include "../main/main.h"

#ifndef _SYSTEM_H
#define _SYSTEM_H 1


// ------ Public constants -----------------------------------------
// Required system operating frequency (in Hz)
// Will be checked in the scheduler initialization file
#define Required_SystemCoreClock (100000000)	// 100MHz

#define SysTick_RateHz	(1000)				// 1000 ticks per second

#define SysTick_RatemS	(1000 / SysTick_RateHz)		// 1mS
#define SysTick_RateuS	(1000 * SysTick_RatemS)		// 1000uS

#define WatchDog_RateuS ((11 * SysTick_RateuS)/10)	// 1100uS


//------------------------------------------------------------------
// System fault codes
//------------------------------------------------------------------

// Scheduler fault codes
#define FAULT_SCH_TOO_MANY_TASKS (1)
#define FAULT_SCH_ONE_SHOT_TASK (2)

// Other fault codes may be added here, if required

//------------------------------------------------------------------
// Project constants
//------------------------------------------------------------------

#define RETURN_NORMAL 0
#define RETURN_FAULT 1


// Three possible system modes
typedef enum {FAIL_SILENT, NORMAL, FAULT_TASK_TIMING} eSystem_mode;


// ------ Public data type declarations ----------------------------
#define RESET 0
#define BTH 1
#define SENSORES 3

typedef struct{
uint32_t Modo;
uint32_t Cantidad_Golpes;
uint32_t Primer_Inicio;
uint32_t ModoPrevio;
} ESTADO_GLOBAL_DEF;
// ------ Public function prototypes -------------------------------
void SYSTEM_Init(void);
void SYSTEM_Perform_Safe_Shutdown(void);
void SYSTEM_Change_Mode_Fault(eSystem_mode mode);
eSystem_mode SYSTEM_Get_Mode(void);
void Switcheo_Lista (void);
void Borro_lista (void);


#endif


/*------------------------------------------------------------------*-
  ---- END OF FILE -------------------------------------------------
-*------------------------------------------------------------------*/
