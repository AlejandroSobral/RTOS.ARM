/*--------------------------------------------------------------------*-

    system_lpc1769.c (Released 2019-04)

    Controls system configuration after processor reset.
    [Two modes supported - "Normal" and "Fail Silent".]

-*--------------------------------------------------------------------*/


// Project header
#include "../main/main.h"


// Task headers
#include "../tasks/task-watchdog_lpc1769.h"
#include "../tasks/task-heartbeat_lpc1769.h"
#include "../tasks/task-e1_switch_lpc1769.h"
#include "../tasks/task-s1_switch_lpc1769.h"
#include "../tasks/task-s2_switch_lpc1769.h"
#include "../tasks/task-s3_switch_lpc1769.h"
#include "../tasks/task-motor1_led_lpc1769.h"
#include "../tasks/task-motor2_led_lpc1769.h"
#include "../tasks/task-car_wash_controller_lpc1769.h"


// ------ Public variable ------------------------------------------
// In many designs, System_mode_G will be used in other modules.
// - we therefore make this variable public.
eSystem_mode System_mode_G;


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
        LPC_SYSCTL->RSID &= ~(0x04);

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
            SYSTEM_Perform_Safe_Shutdown();

            break;
        }

        case NORMAL:
        {
        	// Set up scheduler for 1 ms ticks (tick interval in *ms*)
            SCH_Init(1);

            /* Initialize WWDT and event router */
        	Chip_WWDT_Init(LPC_WWDT);

            // Set up WDT (timeout in *microseconds*)
            WATCHDOG_Init(WatchDog_RateuS);

        	// Prepare to Read E1 SWITCH task
        	E1_SWITCH_Init();

        	// Prepare to Read S1 SWITCH task
        	S1_SWITCH_Init();

        	// Prepare to Read S2 SWITCH task
        	S2_SWITCH_Init();

        	// Prepare to Read S3 SWITCH task
        	S3_SWITCH_Init();

        	// Prepare to Write MOTOR1 LED task
        	MOTOR1_LED_Init();

        	// Prepare to Write MOTOR2 LED task
        	MOTOR2_LED_Init();

        	// Prepare to CAR_WASH_CONTROLLER task
        	CAR_WASH_CONTROLLER_Init();

        	// Prepare for Heartbeat task
        	HEARTBEAT_Init();

        	// Add tasks to schedule.
            // Parameters are:
            // 1. Task name
            // 2. Initial delay / offset (in ticks)
            // 3. Task period (in ticks): Must be > 0
            // 4. Task WCET (in microseconds)
            // 5. Task BCET (in microseconds)

            // Add watchdog task first
            SCH_Add_Task(WATCHDOG_Update, 0, 1, 10, 0);

            // Add E1_SWITCH task
            SCH_Add_Task(E1_SWITCH_Update, 1, 10, 20, 0);

            // Add S1_SWITCH task
            SCH_Add_Task(S1_SWITCH_Update, 1, 10, 20, 0);

            // Add S2_SWITCH task
            SCH_Add_Task(S2_SWITCH_Update, 1, 10, 20, 0);

            // Add S3_SWITCH task
            SCH_Add_Task(S3_SWITCH_Update, 1, 10, 20, 0);

            // Add MOTOR1_LED task
            SCH_Add_Task(MOTOR1_LED_Update, 2, 10, 10, 0);

            // Add MOTOR2_LED task
            SCH_Add_Task(MOTOR2_LED_Update, 2, 10, 10, 0);

            // Add CAR_WASH_CONTROLLER task
            SCH_Add_Task(CAR_WASH_CONTROLLER_Update, 0, 10, 40, 0);

            // Add Heartbeat task
            SCH_Add_Task(HEARTBEAT_Update, 0, 1000, 20, 0);

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


/*------------------------------------------------------------------*-
  ---- END OF FILE -------------------------------------------------
-*------------------------------------------------------------------*/
