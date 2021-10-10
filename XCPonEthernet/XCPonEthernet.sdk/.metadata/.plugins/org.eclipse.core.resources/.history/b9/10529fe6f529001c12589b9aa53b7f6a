/*
 * echo.c
 *
 *  Created on: Apr 10, 2020
 *      Author: Hamid Mahmoudi, Ximu Zhang
 *      This source code provides Ethernet transport layer for XCP communication in FreeRTOS.
 */

// Header files
#include	"lwip/udp.h"
#include 	"lwip/err.h"
#include 	"lwip/sockets.h"
#include 	"lwip/sys.h"
#include	"lwip/sockets.h"
#include	"xcpBasic.h"
#include	"xil_types.h"
#include	<string.h>
#include 	<xil_printf.h>

#define RAW_MODE 1

// Required definitions
#define		ETH_UDP_PORT 7         		//Ethernet port number of Zynq-7000
#define		ETH_MAX_RX kXcpMaxDTO+4		//Max size of Ethernet receive buffer
#define 	ETH_MAX_TX kXcpMaxDTO+4		//Max size of Ethernet transmit buffer

// Global variables for Ethernet handling
u8 rx_xcp_buffer[ETH_MAX_RX] = {0};
err_t udpsenderr;
#if RAW_MODE != 1
static int sock;
struct sockaddr_in client_addr;
socklen_t client_len = sizeof(client_addr);
#endif

volatile u32 EthBytesReceived;
struct udp_pcb send_pcb;
struct pbuf *psnd;
volatile u16_t RemotePort;
extern struct ip4_addr RemoteAddr;
u16 XCP_Eth_TxCounter;

void ApplXcpSend( vuint8 XCPLength, const BYTEPTR XCPData ) {
    if (XCPLength<8)
    	XCPLength=8;

#if RAW_MODE == 1
    // raw mode
    // Header for XCP on ETH Transport Layer configuration
    u8 tx_buffer_hdr[4];
    tx_buffer_hdr[0] = XCPLength;
    tx_buffer_hdr[1] = 0;
    tx_buffer_hdr[2] = (u8)XCP_Eth_TxCounter;
    tx_buffer_hdr[3] = (u8)(XCP_Eth_TxCounter>>8);

    // Ethernet transmit
	psnd = pbuf_alloc(PBUF_TRANSPORT, XCPLength + sizeof(tx_buffer_hdr), PBUF_REF);
	memcpy(psnd->payload, &tx_buffer_hdr, 4);
	memcpy(psnd->payload+(u32)4, XCPData, XCPLength);
	udpsenderr = udp_sendto(&send_pcb, psnd, &RemoteAddr, RemotePort);
	pbuf_free(psnd);

    XCP_Eth_TxCounter++;
    XcpSendCallBack();

#else
    // socket mode
    u8 xcp_header[4] = {0};
    u8 xcp_packet_id[4];
    u16 xcp_packet_length;
    u8 *tx_xcp_buffer = (u8*)calloc(sizeof(xcp_header) + sizeof(xcp_packet_id) + XCPData, sizeof(u8));

    // XCP header
    xcp_packet_length = XCPLength + sizeof(xcp_packet_id);
    xcp_header[0] = (u8)xcp_packet_length;
    xcp_header[1] = (u8)(xcp_packet_length >> 8);

    // XCP packet identification field
    xcp_packet_id[0] = XCPLength;
    xcp_packet_id[1] = 0;
    xcp_packet_id[2] = (u8)XCP_Eth_TxCounter;
    xcp_packet_id[3] = (u8)(XCP_Eth_TxCounter >> 8);

    // Ethernet transmit
    memcpy(tx_xcp_buffer, &xcp_header, 4);
	memcpy(tx_xcp_buffer + (u8)4, &xcp_packet_id, 4);
	memcpy(tx_xcp_buffer + (u8)8, XCPData, XCPLength);

    udpsenderr = lwip_sendto(sock, &tx_xcp_buffer, sizeof(tx_xcp_buffer), 0,
			(struct sockaddr *)&client_addr, client_len);
    close(sock);

    XCP_Eth_TxCounter++;
    XcpSendCallBack();

    free(tx_xcp_buffer);
#endif
}

void ApplXcpInterruptEnable(void) {
	/* Re-enable Interrupts. */
}

void ApplXcpInterruptDisable(void) {
	/* Save interrupt state and disable. */
}

/* start_application: function to set up UDP listener */
void start_udp_application() {
#if RAW_MODE != 1
	struct sockaddr_in server_addr;

	memset(&server_addr, 0, sizeof(struct sockaddr_in));

	if ((sock = lwip_socket(AF_INET, SOCK_DGRAM, 0)) < 0)
		xil_printf("Failed to create socket\r\n");

	/* bind to specified port */
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(ETH_UDP_PORT);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (lwip_bind(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
		xil_printf("Failed to bind at port '%d'\r\n", ETH_UDP_PORT);
		close(sock);
	}

	int count;
	char recv_buf[ETH_MAX_RX];

	while (1) {
		if((count = lwip_recvfrom(sock, &recv_buf, ETH_MAX_RX, 0,
				(struct sockaddr *)&client_addr, &client_len)) <= 0) {
			continue;
		}

		memcpy(rx_xcp_buffer, (u8*)recv_buf + (u8)4, recv_buf[0] + 256 * recv_buf[1]);
		XcpCommand((void*)rx_xcp_buffer);
	}
#endif
}

void recv_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p, struct ip4_addr *addr, u16_t port) {
	u8 rx_xcp_buffer[ETH_MAX_RX];

	/* Do not read the packet if we are not in ESTABLISHED state */
	if (!p) {
		udp_disconnect(upcb);
		return;
	}

	/* Assign the Remote IP:port from the callback on each first pulse */
	RemotePort = port;
	RemoteAddr = *addr;

	/* Keep track of the control block so we can send data back in the main while loop */
	send_pcb = *upcb;

	// Determine the number of bytes received and copy this segment to the temp array
	EthBytesReceived = p->len;
	memcpy(&rx_xcp_buffer, (u8*)(p->payload+(u32)4), ETH_MAX_RX-4);
	XcpCommand((void*)rx_xcp_buffer);

	/* free the received pbuf */
	pbuf_free(p);
}

/* start_application: function to set up UDP listener */
int start_application()
{
	struct udp_pcb *pcb;
	err_t err;
	unsigned port = ETH_UDP_PORT;

	/* create new UDP PCB structure */
	pcb = udp_new();

	/* bind to specified @port */
	err = udp_bind(pcb, IP_ADDR_ANY, port);

	/* specify callback to use for incoming connections */
	udp_recv(pcb, recv_callback, NULL);

	return 0;
}
