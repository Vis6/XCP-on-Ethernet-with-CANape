/*
 * main_periodic_tasks.c
 *
 *  Created on: Mar 23, 2020
 *    Author: Ximu Zhang
 *    This source code defines periodic tasks in FreeRTOS.
 */

/* Standard includes. */
#include <stdio.h>
#include "xparameters.h"
#include "xgpiops.h"
#include <sleep.h>
#include "xqspips.h"
#include "xil_printf.h"
#include "XcpBasic.h"
#include "xil_io.h"

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/* Priorities at which the tasks are created. */
#define	task1_PRIORITY		( tskIDLE_PRIORITY + 4 )
#define	task2_PRIORITY		( tskIDLE_PRIORITY + 3 )
#define xcp_PRIORITY		( tskIDLE_PRIORITY + 2 )

/* Ticks. */
#define TASK1_FREQUENCY_MS			( 2UL / portTICK_PERIOD_MS ) // 2ms period
#define TASK2_FREQUENCY_MS			( 10UL / portTICK_PERIOD_MS ) // 10ms period
#define XCP_FREQUENCY_MS			( 100UL / portTICK_PERIOD_MS ) // period 100ms
/*-----------------------------------------------------------*/

/*
 * The tasks as described in the comments at the top of this file.
 */
static void prvTask1( void *pvParameters );
static void prvTask2( void *pvParameters );
static void xcpTask();

// global variables
static u32 task1_value = 0;
static u32 task2_value = 0;
static u32 calibration_value = 0;
static u32 signal = 0;

extern u32 XCP_100us_Counter; // XCP

extern XGpioPs PS_Gpio;
extern volatile int led_status;
extern const int OUTPUT_PORT;
/*-----------------------------------------------------------*/

void mainPeriodicTasks()
{
	/* Start the four tasks. */
	xTaskCreate( prvTask1, "Task1", configMINIMAL_STACK_SIZE, NULL, task1_PRIORITY, NULL );
	xTaskCreate( prvTask2, "Task2", configMINIMAL_STACK_SIZE, NULL, task2_PRIORITY, NULL );
	xTaskCreate( xcpTask, "xcp", configMINIMAL_STACK_SIZE, NULL, xcp_PRIORITY, NULL );
}

/*-----------------------------------------------------------*/

static void prvTask1( void *pvParameters)
{
	TickType_t xNextWakeTime;

	( void ) pvParameters;

	/* Initialize xNextWakeTime - this only needs to be done once. */
	xNextWakeTime = xTaskGetTickCount();

	for ( ;; )
	{
		/* Place this task in the blocked state until it is time to run again. */
		vTaskDelayUntil( &xNextWakeTime, TASK1_FREQUENCY_MS );

		Xil_Out32(0x201B3A0, task1_value); // output task1_value to CANape

		task1_value++;
		if (task1_value >= 100)
			task1_value = (u32)0;

		XCP_100us_Counter += 20;
		XcpEvent(1);

		if (++signal == 100)
			signal = 0;
	}
}

/*-----------------------------------------------------------*/

static void prvTask2( void *pvParameters )
{
	TickType_t xNextWakeTime;

	( void ) pvParameters;

	/* Initialize xNextWakeTime - this only needs to be done once. */
	xNextWakeTime = xTaskGetTickCount();

	for ( ;; )
	{
		/* Place this task in the blocked state until it is time to run again. */
		vTaskDelayUntil( &xNextWakeTime, TASK2_FREQUENCY_MS );

		Xil_Out32(0x201B3A4, task2_value); // output task2_value to CANape

		task2_value++;
		if (task2_value >= 100) {
			task2_value = (u32)0;

			led_status = led_status == 0 ? 1 : 0;
			XGpioPs_WritePin(&PS_Gpio, OUTPUT_PORT, led_status);
		}

//		XcpEvent(2);
	}
}
/*-----------------------------------------------------------*/

static void xcpTask()
{
	TickType_t xNextWakeTime;
	xNextWakeTime = xTaskGetTickCount();

	// init
	Xil_Out32(0x1C0034, 0x42480000); // store float, need use IEEE 754 standard

	for ( ;; )
	{
		vTaskDelayUntil( &xNextWakeTime, XCP_FREQUENCY_MS );

		calibration_value = Xil_In32(0x1C0034);
		Xil_Out32(0x1BE234, calibration_value);

//		XcpEvent(0);
	}
}
