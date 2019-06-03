/*--------------------------------------------------------------------*-

    task-car_wash_controller_lpc1769.c (Released 2019-04)

  --------------------------------------------------------------------

    Simple CAR WASH CONTROLLER task for LPC1769.
	[Read Input on LPCXpresso baseboard.]

    Simple car wash controller code.

    - See "Car Wash Cortroller.jpg" for details.

-*--------------------------------------------------------------------*/


// Project header
#include "../main/main.h"


// Task header
#include "task-car_wash_controller_lpc1769.h"


// ------ Public variable ------------------------------------------
// Sensors
extern uint32_t E1_switch_pressed_G;
extern uint32_t S1_switch_pressed_G;
extern uint32_t S2_switch_pressed_G;
extern uint32_t S3_switch_pressed_G;


// Actuators
extern uint32_t motor1_led_required_G;
extern uint32_t motor2_led_required_G;


// ------ Private constants ----------------------------------------
// All durations are in Seconds (short times here for demo)
#define MAX_PARKING_DURATION (100)
#define MAX_WASHING_DURATION (100)


// ------ Private data type ----------------------------------------
// Possible system states
typedef enum {IDLE, COME_IN, PARKING, WASHING, COME_OUT, FAULT} eSystem_state;


// ------ Private variable -----------------------------------------
static eSystem_state System_state_G;
static uint32_t Time_in_state_G;


/*------------------------------------------------------------------*-

    CAR_WASH_CONTROLLER_Init()

    Prepare for CAR_WASH_CONTROLLER_Update() function - see below.

-*------------------------------------------------------------------*/
void CAR_WASH_CONTROLLER_Init(void)
{
	System_state_G = IDLE;

	// Motor1 is off
	motor1_led_required_G = MOTOR_OFF;

	// Motor2 is off
	motor2_led_required_G = MOTOR_OFF;
}


/*------------------------------------------------------------------*-

    CAR_WASH_CONTROLLER_Update()

    Simple car wash controller code.

	If IDLE     && E1_switch_pressed_G is sensor on  &&
	               S1_switch_pressed_G is sensor off &&
	               S2_switch_pressed_G is sensor off &&
	               S3_switch_pressed_G is sensor off    => COME_IN
	                                                       Time_in_state_G = 0
	If COME_IN  && S1_switch_pressed_G is sensor on  &&
	               S2_switch_pressed_G is sensor off &&
	               S3_switch_pressed_G is  sensor off   => PARKING
	                                                       Time_in_state_G = 0
	                                                       MOTOR2 => MOTOR_ON
	If PARKING  && S1_switch_pressed_G is sensor on &&
	               S2_switch_pressed_G is sensor on &&
	               S3_switch_pressed_G is sensor off         => WASHING
	                                                            Time_in_state_G = 0
	                                                            MOTOR1 => MOTOR_ON
	            && ++Time_in_state_G >= MAX_PARKING_DURATION => FAULT
	                                                            Time_in_state_G = 0
	                                                            MOTOR1 => MOTOR_OFF
                                                                MOTOR2 => MOTOR_OFF
	If WASHING  && S1_switch_pressed_G is sensor off &&
	               S2_switch_pressed_G is sensor off &&
	               S3_switch_pressed_G is sensor on          => COME_OUT
                                                                Time_in_state_G = 0
                                                                MOTOR1 => MOTOR_OFF
                                                                MOTOR2 => MOTOR_OFF
	            && ++Time_in_state_G >= MAX_WASHING_DURATION => FAULT
                                                                Time_in_state_G = 0
                                                                MOTOR1 => MOTOR_OFF
                                                                MOTOR2 => MOTOR_OFF
	If COME_OUT && S1_switch_pressed_G is sensor off &&
	               S2_switch_pressed_G is sensor off &&
	               S3_switch_pressed_G is sensor off    	=> IDLE
	                                                           Time_in_state_G = 0
	If FAULT                                            	=> FAULT

    Must schedule every 1 mili second (soft deadline).

-*------------------------------------------------------------------*/
void CAR_WASH_CONTROLLER_Update(void)
{
	// Call once per Second

	switch (System_state_G)
	{
		case IDLE:
			if ((E1_switch_pressed_G == SENSOR_ON) && (S1_switch_pressed_G == SENSOR_OFF) &&
				(S2_switch_pressed_G == SENSOR_OFF) && (S3_switch_pressed_G == SENSOR_OFF))
			{
				// Ready to go to next state
				System_state_G = COME_IN;
				Time_in_state_G = 0;
			}
			break;

		case COME_IN:
			if ((S1_switch_pressed_G == SENSOR_ON) && (S2_switch_pressed_G == SENSOR_OFF) &&
				(S3_switch_pressed_G == SENSOR_OFF))
			{
				// Ready to go to next state
				System_state_G = PARKING;
				Time_in_state_G = 0;

				// Motor2 is on
				motor2_led_required_G = MOTOR_ON;
			}
			break;

		case PARKING:
			// Remain in this state the necessary time
			// NOTE: Timeout facility included here
			if (++Time_in_state_G >= MAX_PARKING_DURATION)
			{
				// Should have advanced the car by now ...
				System_state_G = FAULT;
			}

			if ((S1_switch_pressed_G == SENSOR_ON) && (S2_switch_pressed_G == SENSOR_ON) &&
				(S3_switch_pressed_G == SENSOR_OFF))
			{
				// Ready to go to next state
				System_state_G = PARKING;
				Time_in_state_G = 0;

				// Motor2 is on
				motor2_led_required_G = MOTOR_ON;
			}
			break;

		case WASHING:
			// Remain in this state the necessary time
			// NOTE: Timeout facility included here
			if (++Time_in_state_G >= MAX_WASHING_DURATION)
			{
				// Should have advanced the car by now ...
				System_state_G = FAULT;
			}

			if ((S1_switch_pressed_G == SENSOR_OFF) && (S2_switch_pressed_G == SENSOR_OFF) &&
				(S3_switch_pressed_G == SENSOR_ON))
			{
				// Ready to go to next state
				System_state_G = COME_OUT;
				Time_in_state_G = 0;
			}
			break;

		case COME_OUT:
			if ((S1_switch_pressed_G == SENSOR_OFF) && (S2_switch_pressed_G == SENSOR_OFF) &&
				(S3_switch_pressed_G == SENSOR_OFF))
			{
				// Ready to go to next state
				System_state_G = IDLE;
				Time_in_state_G = 0;

				// Motor1 is off
				motor1_led_required_G = MOTOR_OFF;

				// Motor2 is off
				motor2_led_required_G = MOTOR_OFF;
			}
			break;

		case FAULT:
			// Set up safe state
			// Motor1 is off
			motor1_led_required_G = MOTOR_OFF;

			// Motor2 is off
			motor2_led_required_G = MOTOR_OFF;

			break;
	}
}


/*------------------------------------------------------------------*-
  ---- END OF FILE -------------------------------------------------
-*------------------------------------------------------------------*/
