/*
 * led_flash_task.c
 *
 *  Created on: Mar 23, 2020
 *      Author: Ximu Zhang
 *      This source code creates a task that flashes LED at the PL side.
 */

/* Standard includes. */
#include <stdio.h>
#include "xparameters.h"
#include "xgpio.h"
#include <sleep.h>
#include "XcpBasic.h"

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#define	LED_PRIORITY		( tskIDLE_PRIORITY + 1 ) // task priority
#define LED_FREQUENCY_MS	( 100UL / portTICK_PERIOD_MS ) // 100ms period

/* Function declaration */
static void ledTask();
extern void gpioSendData();

// PL GPIO
#define PL_GPIO_DEVICE_ID XPAR_AXI_GPIO_0_DEVICE_ID
#define LED_CHANNEL 1

XGpio pl_gpio; // gpio device driver instance

volatile int led; // global variables

int plLedInit()
{
	int status;

	/* GPIO Initialization */
	status = XGpio_Initialize(&pl_gpio, PL_GPIO_DEVICE_ID); // init gpio at pl side
	if (status != XST_SUCCESS)
	{
		return XST_FAILURE;
	}
	XGpio_SetDataDirection(&pl_gpio, LED_CHANNEL, 0x00); // output gpio pins

	return 0;
}

void gpioSendData()
{
	XGpio_DiscreteWrite(&pl_gpio, LED_CHANNEL, led);
	led = led+1;
}

/* Local function definition */
void mainLedFlashTask()
{
	plLedInit();
	xTaskCreate( ledTask, "PL_LED", configMINIMAL_STACK_SIZE, NULL, LED_PRIORITY, NULL );
//	ledTask();
}

static void ledTask()
{
	TickType_t xNextWakeTime;

	/* Initialise xNextWakeTime - this only needs to be done once. */
	xNextWakeTime = xTaskGetTickCount();

	for ( ;; )
	{
		/* Place this task in the blocked state until it is time to run again. */
		vTaskDelayUntil( &xNextWakeTime, LED_FREQUENCY_MS );

		gpioSendData();
	}
}
/*-----------------------------------------------------------*/
