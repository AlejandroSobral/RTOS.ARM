/*--------------------------------------------------------------------*-

    system_lpc1769.c (Released 2019-04)

    Controls system configuration after processor reset.
    [Two modes supported - "Normal" and "Fail Silent".]

-*-----//---------------------------------------------------------------*/


// Project header
#include "../main/main.h"

// Task headers
#include "../tasks/task-watchdog_lpc1769.h"
#include "../tasks/task-heartbeat_lpc1769.h"
#include "../tasks/acelerometro.h"
#include "../tasks/dht11.h"
#include "../tasks/UartMonitor.h"
#include "../tasks/demoramicrosegundos.h"
#include "../tasks/BatteryLife.h"
#include "../tasks/logger.h"
#include "../tasks/24LC256.h"
#include "../tasks/SwitchReset.h"
#include "../tasks/InformeBT.h"
#include "../tasks/SwitchModo.h"
#include "../tasks/GPS.h"
#include "../c-tds/scheduler_lpc1769.h"



// ------ Public variable ------------------------------------------
// In many designs, System_mode_G will be used in other modules.
// - we therefore make this variable public.
eSystem_mode System_mode_G;
ESTADO_GLOBAL_DEF ESTADO_GLOBAL;
uint32_t Switchea_lista_flag;
uint32_t primer_inicio;


// ------ Private function declarations ----------------------------
void SYSTEM_Identify_Required_Mode(void);
void SYSTEM_Configure_Required_Mode(void);


/*------------------------------------------------------------------*-

    SYSTEM_Init()

    Wrapper for system startup functions.

-*------------------------------------------------------------------*/
void SYSTEM_Init(void)
{
    SYSTEM_Identify_Required_Mode();
    SYSTEM_Configure_Required_Mode();
}


/*------------------------------------------------------------------*-

    SYSTEM_Identify_Required_Mode()

    Try to work out the cause of the system reset.
    Set the system mode accordingly.

-*------------------------------------------------------------------*/
void SYSTEM_Identify_Required_Mode(void)
{
	// If "1", reset was caused by WDT
    uint32_t WDT_flag = (LPC_SYSCTL->RSID >> 2) & 1;

    if (WDT_flag == 1)
    {
        // Cleared only by software or POR
        // Clear flag (or other resets may be interpreted as WDT)
        LPC_SYSCTL->RSID |= (0x04);

        // Set system mode (Fail Silent)
        System_mode_G = FAIL_SILENT;
    }
    else
    {
        // Here we treat all other forms of reset in the same way
        // Set system mode (Normal)
        System_mode_G = NORMAL;
    }
}


/*------------------------------------------------------------------*-

    SYSTEM_Configure_Required_Mode()

    Configure the system in the required mode.

-*------------------------------------------------------------------*/
void SYSTEM_Configure_Required_Mode(void)
{

	Chip_SetupXtalClocking();

	/* Setup FLASH access to 4 clocks (100MHz clock) */
	Chip_SYSCTL_SetFLASHAccess(FLASHTIM_100MHZ_CPU);

	SystemCoreClockUpdate();

	/* Initialize GPIO */
	Chip_GPIO_Init(LPC_GPIO);
	Chip_IOCON_Init(LPC_IOCON);

	switch (System_mode_G)
	{
        default: // Default to "FAIL_SILENT"
        case FAIL_SILENT:
        case FAULT_TASK_TIMING:
        {
            // Reset caused by WDT
            // Trigger "fail silent" behavior
            //SYSTEM_Perform_Safe_Shutdown();

            break;
        }

        case NORMAL:
        {
        	// Set up scheduler for 1 ms ticks (tick interval in *ms*)
            SCH_Init(10);
            /* Initialize WWDT and event router */
        	Chip_WWDT_Init(LPC_WWDT);
            // Set up WDT (timeout in *microseconds*)
            WATCHDOG_Init(WatchDog_RateuS);
        	HEARTBEAT_Init();
        	GPIO_DHT11_Init();
        	Timer1_Init();
        	Acelerometro_Init();
        	ADC0_Init();
        	UartMonitor_Init();
        	UartGPSInit();
        	Switch_Reset_Init();
        	Switch_MODO_Init();
        	Ciclo_Memoria_Reading_CantidadGolpes();
        	ESTADO_GLOBAL.Modo = SENSORES; // Por default, a modo de medicion
        	Switchea_lista_flag = 1; // Para que agregue toda la lista de tareas
        	primer_inicio = 0; //Resetea flag de inicio

        	//Agrego esta tareas para hacer una primera lectura de los botones
        	//sobre todo para el caso del reset
        	SCH_Add_Task(WATCHDOG_Update, 0, 1, 250, 0);
        	SCH_Add_Task(Switch_Reset, 0, 1, 500, 0);
        	SCH_Add_Task(Switcheo_Lista,0,1,3000,0);



        	 break;
        	}
        }
	}



/*------------------------------------------------------------------*-

    SYSTEM_Perform_Safe_Shutdown()

    Attempt to place the system into a safe state.

    Note: Does not return and may (if watchdog is operational) result
    in a processor reset, after which the function may be called again.
    [The rationale for this behavior is that - after the reset -
    the system MAY be in a better position to enter a safe state.
    To avoid the possible reset, adapt the code and feed the WDT
    in the loop.]

-*------------------------------------------------------------------*/
void SYSTEM_Perform_Safe_Shutdown(void)
{
    // Used for simple fault reporting
    uint32_t Delay, j;

    // Here we simply "fail silent" with rudimentary fault reporting
    // OTHER BEHAVIOUR IS LIKELY TO BE REQUIRED IN YOUR DESIGN
    // *************************************
    // NOTE: This function should NOT return
    // *************************************
    HEARTBEAT_Init();

    while(1)
	{
        // Flicker Heartbeat LED to indicate fault
        for (Delay = 0; Delay < 200000; Delay++) j *= 3;
        HEARTBEAT_Update();
	}
}


void SYSTEM_Change_Mode_Fault(eSystem_mode mode)
{
	System_mode_G = mode;
}


eSystem_mode SYSTEM_Get_Mode(void)
{
	return System_mode_G;
}

void Switcheo_Lista (void)
{
extern uint32_t Switchea_lista_flag;
extern uint32_t GolpesLeidos;
extern uint32_t cantidad_golpes;
extern uint32_t UltimaMemoriaLeida;
extern uint32_t MemoriaBorrada ;
extern int lecturaunica;

	if (Switchea_lista_flag) //Si me mandaron a switchear la lista de tareas
	{
		Borro_lista(); //Borro la lista entera

		Switchea_lista_flag = 0;




	       	switch(ESTADO_GLOBAL.Modo)
	       	{ // SELECCIONO LA LISTA DE TAREAS
        	case RESET: //Modo de reseteo, borra toda la memoria
        	// Enable SysTick timer
        	GolpesLeidos=0;
        	cantidad_golpes = 0;
            SysTick->CTRL |= 0x01;
            SysTick->CTRL |= 0x02;
        	SCH_Add_Task(WATCHDOG_Update, 0, 1, 250, 0);
            SCH_Add_Task(Switch_Reset, 0, 1, 500, 0);
            SCH_Add_Task(Switch_MODO, 0, 1, 4000, 0);
            SCH_Add_Task(Switcheo_Lista,0,1,3000,0);
        	SCH_Add_Task(Ciclo_Memoria_Erase,0,1,2500000,0);
        	primer_inicio = 0;


			break;
        	case BTH: //Modo envio de datos por BlueThooth
        	// Enable SysTick timer
            SysTick->CTRL |= 0x01;
            SysTick->CTRL |= 0x02;
            UltimaMemoriaLeida = Offset;
            MemoriaBorrada = 0;
            lecturaunica = 0;
        	SCH_Add_Task(WATCHDOG_Update, 0, 1, 250, 0);
            SCH_Add_Task(Switch_Reset, 0, 1, 500, 0);
            SCH_Add_Task(Switch_MODO, 0, 1, 4000, 0);
        	SCH_Add_Task(Switcheo_Lista,0,1,3000,0);
        	SCH_Add_Task(InformeBT,0,1,250000,0); //Tarea que arma los mensajes

			break;


        	case SENSORES:
        // Enable SysTick timer
           SysTick->CTRL |= 0x01;
           SysTick->CTRL |= 0x02;
           GolpesLeidos=0;
           MemoriaBorrada = 0;
           primer_inicio = 1; //Este flag me sirve para saber si alguna vez entré en este estado
           UltimaMemoriaLeida = Offset;
           SCH_Add_Task(WATCHDOG_Update, 0, 1, 250, 0);
           SCH_Add_Task(Switch_Reset, 0, 1, 500, 0);
           SCH_Add_Task(Switch_MODO, 0, 1, 4000, 0);
           SCH_Add_Task(TareaLeeGPS, 0, 2, 10000, 0);
           SCH_Add_Task(GPIO_DHT11,  2, 2, 500000, 0);
           SCH_Add_Task( HEARTBEAT_Update,  1, 1, 500, 0);
           SCH_Add_Task( Acelerometro_Update,  1, 2, 80000, 0);
           SCH_Add_Task( EstadoBateria,  1, 1, 2000, 0);
           SCH_Add_Task( Logger,  1, 2, 200000, 0);
           SCH_Add_Task(Switcheo_Lista,0,1,3000,0);

           break;
	       	}
	}

}

void Borro_lista (void)
{
extern sTask SCH_tasks_G[SCH_MAX_TASKS];

uint32_t i;

 //FRENO SYSTICK, ALIMENTO WATCHDOG, BORRO, CARGO Y PRENDO SYSTICK
    // Disable SysTick timer, interrupt.
    SysTick->CTRL |= 0x00;
    WATCHDOG_Update();


//WATCHDOG y GOGO
	for( i=0; i<SCH_MAX_TASKS ; i ++)
	{
	 SCH_tasks_G[i].pTask = 0;
	 SCH_tasks_G[i].Delay = 0;
	 SCH_tasks_G[i].Period = 0;
	 SCH_tasks_G[i].WCET = 0;
	 SCH_tasks_G[i].BCET = 0;
	}
}




/*------------------------------------------------------------------*-
  ---- END OF FILE -------------------------------------------------
-*------------------------------------------------------------------*/
