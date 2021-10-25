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
#include "xcpBasic.h"
#include "xcp_cfg.h"
#include "xcp_par.h"
#include "xcp_def.h"


// Slave device id
// CANape uses this to determine the name of the A2L file
#if defined ( kXcpStationIdLength )
  V_MEMROM0 MEMORY_ROM vuint8 kXcpStationId[kXcpStationIdLength] = kXcpStationIdString;
#endif


/* implement here parameters for customizing XcpBasic driver (e.g. kXcpStationId) */


