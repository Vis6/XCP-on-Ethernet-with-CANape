/******************************************************************************
*
* Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

/*
 * xcp_standalone_main.c: xcp on ethernet application in bare metal system. 
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xparameters.h"
#include "xscutimer.h"
#include "xscugic.h"
#include "xil_exception.h"
#include "lwip/udp.h"
#include "netif/xadapter.h"
#include "xcpBasic.h"
#include "sleep.h"


/*
 * XCP task control.
 */
static int task_flag = 0;
static int timer_counter = 0;

int signal = 0;

/*
 * Ethernet.
 */
#define PLATFORM_EMAC_BASEADDR XPAR_XEMACPS_0_BASEADDR
#define PLATFORM_ZYNQ

// Setup netif structures
static struct netif server_netif;
struct netif *echo_netif;

// Global variables for ethernet handling
struct ip4_addr RemoteAddr;

u32 XCP_100us_Counter = 0;

int start_application();
void lwip_init();
void XcpInit(void);

/*
 * Interrupt controller.
 */
#define INTC_DEVICE_ID			XPAR_PS7_SCUGIC_0_DEVICE_ID
#define INTC_BASE_ADDR			XPAR_SCUGIC_0_CPU_BASEADDR
#define INTC_DIST_BASE_ADDR		XPAR_SCUGIC_0_DIST_BASEADDR
#define XIL_EXCEPTION_ID_INT	XIL_EXCEPTION_ID_IRQ_INT
#define eth_tmr_cnt (XPAR_CPU_CORTEXA9_0_CPU_CLK_FREQ_HZ/8/TimerLoadValue) // Counter for reading the Ethernet Status registers

static XScuGic Intc; // interrupt_test Device driver instance

void interruptSetup();
void enableInterrupt();

/*
 * Timer setup.
 */
#define TIMER_DEVICE_ID			XPAR_SCUTIMER_DEVICE_ID
#define TIMER_IRPT_INTR			XPAR_SCUTIMER_INTR
#define	TimerLoadValue			(XPAR_CPU_CORTEXA9_0_CPU_CLK_FREQ_HZ/2*0.0001) // 100us interrupt - The last number represents the period of the timer

static XScuTimer TimerInstance; // ARM private timer

void timerIntrHandler(XScuTimer *TimerInstance);


/*
 * Main function.
 */
int main()
{
    init_platform();

    /*
     * Setup Ethernet.
     */
    struct ip4_addr ipaddr, netmask, gw;

    /*
     * The mac address of the board.
     */
    unsigned char mac_ethernet_address[] = {0x00, 0x0a, 0x35, 0x00, 0x01, 0x10};

    /*
     * Use the same structure for the server and the echo server.
     */
    echo_netif = &server_netif;

    /*
     * Setup timer interrupt
     */
    interruptSetup();

    /*
     * Initialize IP address to be used.
     */
    // IP address of ZYNQ7000
	IP4_ADDR(&ipaddr, 192, 168, 1, 10);
	IP4_ADDR(&netmask, 255, 255, 255, 0);
	IP4_ADDR(&gw, 192, 168, 1, 1);

	// IP address of the PC
	IP4_ADDR(&RemoteAddr, 192, 168, 1, 11);

	/*
	 * Initialize the lwip for UDP.
	 */
	lwip_init();

  	/* Add network interface to the netif_list, and set it as default */
	if (!xemac_add(echo_netif, &ipaddr, &netmask, &gw, mac_ethernet_address, PLATFORM_EMAC_BASEADDR)) {
		return -1;
	}
	netif_set_default(echo_netif);

	/*
	 * Enable timer interrupt.
	 */
	enableInterrupt();

	/*
	 * Specify that the network if it's up.
	 */
	netif_set_up(echo_netif);

	/*
	 * Start the application (web server, rxtest, txtest, etc...).
	 */
	start_application();

	/*
	 * Initialize XCP.
	 */
	XcpInit();

	while (1) {
		if (task_flag) {
			task_flag = 0;

			/*
			 * Do something.
			 */
			XcpEvent(1); // XCP DAQ event - 2ms

			// update signal value for every 2ms
			if (++signal == 100)
				signal = 0;
		}

		/*
		 * Receive packets.
		 */
		xemacif_input(echo_netif);
	}

    cleanup_platform();
    return 0;
}

/*
 * Other functions definition.
 */
void TimerIntrHandler(XScuTimer *TimerInstance)
{
	XCP_100us_Counter++;
//	XcpEvent(0); // XCP DAQ event - 100us

	/*
	 * Call task function for every 2ms.
	 */
	if (++timer_counter == 20) {
		task_flag = 1;
		timer_counter = 0;
	}

	XScuTimer_ClearInterruptStatus(TimerInstance); // clear interrupt
}

void interruptSetup()
{
	int status;

	/* GIC Initialize */
	XScuGic_Config *IntcConfig;
	Xil_ExceptionInit();

	// interrupt controller initialization
	IntcConfig = XScuGic_LookupConfig(INTC_DEVICE_ID);
	status = XScuGic_CfgInitialize(&Intc, IntcConfig, IntcConfig->CpuBaseAddress);
	if (status != XST_SUCCESS)
		xil_printf("GIC initialization failed...\r\n");

	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
			(Xil_ExceptionHandler)XScuGic_InterruptHandler,
			&Intc);

	// Set up timer
	XScuTimer_Config *TimerConfigPtr;
	TimerConfigPtr = XScuTimer_LookupConfig(TIMER_DEVICE_ID);
	status = XScuTimer_CfgInitialize(&TimerInstance, TimerConfigPtr, TimerConfigPtr->BaseAddr);
	if (status != XST_SUCCESS)
		xil_printf("ScuTimer initialization failed...\r\n");

	status = XScuTimer_SelfTest(&TimerInstance);
	if (status != XST_SUCCESS)
		xil_printf("ScuTimer self test failed...\r\n");

	XScuTimer_EnableAutoReload(&TimerInstance);
	XScuTimer_LoadTimer(&TimerInstance, TimerLoadValue);

	/*
	 * Connect the device driver handler that will be called when an
	 * interrupt for the device occurs, the handler defined above performs
	 * the specific interrupt processing for the device.
	 */
	status = XScuGic_Connect(&Intc, TIMER_IRPT_INTR,
					  	  	 (Xil_ExceptionHandler)TimerIntrHandler,
					  	  	 (void *)&TimerInstance);
	XScuGic_Enable(&Intc, TIMER_IRPT_INTR);
}

void enableInterrupt()
{
	XScuTimer_EnableInterrupt(&TimerInstance); //enable interrupt on the timer
	Xil_ExceptionEnableMask(XIL_EXCEPTION_IRQ); // enable interrupts in the processor
	XScuTimer_Start(&TimerInstance); // start timer
}
