/*
 * Program to set up 100us timer and timer interrupt handler.
 * Author: Ximu Zhang
 * Date: 05/12/2020
 */
/* Xilinx library */
#include <stdio.h>
#include "xil_printf.h"
#include "xparameters.h"
#include "xgpiops.h"
#include "xscutimer.h"
#include "xscugic.h"
#include "xintc.h"
#include "xil_exception.h"

#define INTC_DEVICE_ID			XPAR_PS7_SCUGIC_0_DEVICE_ID
#define TIMER_DEVICE_ID			XPAR_SCUTIMER_DEVICE_ID
#define INTC_BASE_ADDR			XPAR_SCUGIC_0_CPU_BASEADDR
#define INTC_DIST_BASE_ADDR		XPAR_SCUGIC_0_DIST_BASEADDR
#define TIMER_IRPT_INTR			XPAR_SCUTIMER_INTR
#define XIL_EXCEPTION_ID_INT	XIL_EXCEPTION_ID_IRQ_INT
#define	TimerLoadValue (XPAR_CPU_CORTEXA9_0_CPU_CLK_FREQ_HZ/2*0.1) // 100us interrupt - The last number represents the period of the timer

/* Static variables */
static XScuTimer TimerInstance;
static XScuGic Intc; // interrupt_test Device driver instance

int mem_value = 0;

/* External variables */
// PS GPIO
extern XGpioPs_Config *PS_GPIO_Config;
extern XGpioPs PS_Gpio;
extern int led_status;
extern const int OUTPUT_PORT;

/* External function */
extern void psLEDInit();

void TimerIntrHandler(XScuTimer *TimerInstance)
{
	Xil_Out32(0x201B3A8, mem_value);
	mem_value++;
	if (mem_value == 100)
		mem_value = 0;

	led_status = ((led_status == 0) ? 1:0);
	XGpioPs_WritePin(&PS_Gpio, OUTPUT_PORT, led_status);
	XScuTimer_ClearInterruptStatus(TimerInstance);
}

void interruptTaskInit()
{
	// 100us interruption
	psLEDInit();

	/* Set up 100us timer */
	XScuTimer_Config *TimerConfigPtr;
	int status;

	TimerConfigPtr = XScuTimer_LookupConfig(TIMER_DEVICE_ID);
	status = XScuTimer_CfgInitialize(&TimerInstance, TimerConfigPtr, TimerConfigPtr->BaseAddr);
	if (status != XST_SUCCESS)
		xil_printf("ScuTimer initialization failed...\r\n");

	status = XScuTimer_SelfTest(&TimerInstance);
	if (status != XST_SUCCESS)
		xil_printf("ScuTimer self test failed...\r\n");

	XScuTimer_EnableAutoReload(&TimerInstance);

	XScuTimer_LoadTimer(&TimerInstance, TimerLoadValue);

	/* Set up interruption for 100us timer*/
	XScuGic_Config *IntcConfig;
	Xil_ExceptionInit();

	// interrupt controller initialization
	IntcConfig = XScuGic_LookupConfig(INTC_DEVICE_ID);
	status = XScuGic_CfgInitialize(&Intc, IntcConfig, IntcConfig->CpuBaseAddress);

	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
			(Xil_ExceptionHandler)XScuGic_InterruptHandler,
			&Intc);

	/*
	 * Connect the device driver handler that will be called when an
	 * interrupt for the device occurs, the handler defined above performs
	 * the specific interrupt processing for the device.
	 */
	//set up the timer interrupt
	// ID: TIMER_IRPT_INTR
	status = XScuGic_Connect(&Intc, TIMER_IRPT_INTR,
					  	  	 (Xil_ExceptionHandler)TimerIntrHandler,
					  	  	 (void *)&TimerInstance);
//	XScuGic_EnableIntr(INTC_DIST_BASE_ADDR, TIMER_IRPT_INTR); //enable the interrupt for the Timer at GIC
	XScuGic_Enable(&Intc, TIMER_IRPT_INTR);
	XScuTimer_EnableInterrupt(&TimerInstance); //enable interrupt on the timer
	Xil_ExceptionEnableMask(XIL_EXCEPTION_IRQ); // enable interrupts in the processor

	XScuTimer_Start(&TimerInstance); // start timer
}
