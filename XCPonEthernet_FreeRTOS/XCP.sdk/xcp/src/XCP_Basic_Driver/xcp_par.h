/*----------------------------------------------------------------------------
| Project: XCP_Delivery
|
|  Description:   Implementation of the XCP Protocol Layer
|                 XCP V1.0 slave device driver
|                 Basic Version
|
|-----------------------------------------------------------------------------
| Copyright (c) by Vector Informatik GmbH.  All rights reserved.
----------------------------------------------------------------------------*/
#if defined ( __XCP_PAR_H__ )
#else
#define __XCP_PAR_H__

/* XCP default settings */
#include "xcp_def.h"

/* Slave device id */
// CANape uses this to determine the name of the A2L file
#define kXcpStationIdLength 3    /* Slave device identification length in characters */
#define kXcpStationIdString "XCP"
V_MEMROM0 extern vuint8 MEMORY_ROM kXcpStationId[];


#endif
