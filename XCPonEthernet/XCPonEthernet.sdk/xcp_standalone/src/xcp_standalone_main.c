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
 * helloworld.c: simple test application
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

#ifndef _MATH_H_
#define _MATH_H_
#include <math.h>
#endif

#ifndef _XCP_MEASURE_CALIBRATE_H_
#define _XCP_MEASURE_CALIBRATE_H_
#include "xcp_measure_calibrate.h"
#endif


/*
 * Ethernet.
 */
#define PLATFORM_EMAC_BASEADDR XPAR_XEMACPS_0_BASEADDR
#define PLATFORM_ZYNQ

// Setup netif structures
static struct netif server_netif;
struct netif *echo_netif;

// Global variables for ethernet handling
struct ip4_addr remote_addr;

u32 xcp_100us_counter = 0;


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
     * Initialize IP address to be used.
     */
    // IP address of ZYNQ7000
	IP4_ADDR(&ipaddr, 192, 168, 1, 10);
	IP4_ADDR(&netmask, 255, 255, 255, 0);
	IP4_ADDR(&gw, 192, 168, 1, 1);

	// IP address of the PC
	IP4_ADDR(&remote_addr, 192, 168, 1, 11);

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

	calibrateVarsInit();

	while (1) {
		/*
		 * Do something.
		 */

		/*
		 * Receive packets.
		 */
		xemacif_input(echo_netif);
	}

    cleanup_platform();
    return 0;
}