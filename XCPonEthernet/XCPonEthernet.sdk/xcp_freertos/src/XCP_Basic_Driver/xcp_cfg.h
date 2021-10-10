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

#if !defined(__XCP_CFG_H__)
#define __XCP_CFG_H__

/* define here configuration parameters for customizing XcpBasic driver */
// General includes
/*----------------------------------------------------------------------------*/
/* Test */

// #define XCP_ENABLE_TESTMODE
#ifdef XCP_ENABLE_TESTMODE
  #define ApplXcpPrint xxxxx
#endif

#define XCP_ENABLE_PARAMETER_CHECK
#define XCP_ASSERT(x)



/* Service request message */
#define XCP_DISABLE_SERV_TEXT
#define XCP_DISABLE_SERV_TEXT_PUTCHAR
#define XCP_DISABLE_SERV_TEXT_PRINT

/*----------------------------------------------------------------------------*/
/* Platform dependant type definition */

  /* 8-Bit qualifier */
typedef unsigned char  vuint8;
typedef signed char    vsint8;

/* 16-Bit qualifier */
typedef unsigned short vuint16;
typedef signed short   vsint16;

/* 32-Bit qualifier */
typedef unsigned long  vuint32;
typedef signed long    vsint32;


/*----------------------------------------------------------------------------*/
/* Interrupt control */

/* Has to be used if XcpSendCallBack() can interrupt XcpEvent() */

#define XcpInterruptDisable()
#define XcpInterruptEnable()


/*----------------------------------------------------------------------------*/
/* XCP protocol parameters */

/* Byte order */
#define C_CPUTYPE_LITTLEENDIAN /* Intel */

/* XCP message length */
#define kXcpMaxCTO     8        /* Maximum CTO Message Lenght */
#define kXcpMaxDTO     255      /* Maximum DTO Message Lenght */ //Max: 255

/* Note that the max. data rate from CANape to ECU depends on kXcpMaxCTO and
   the rate of the dummy events.
*/

/*----------------------------------------------------------------------------*/
/* Memory copy and fill */

#define xcpMemCpy memcpy
#define xcpMemSet memset


/*----------------------------------------------------------------------------*/
/* Optional Service Request Messages */

/* Enable XcpSendEvent used for dummy messages to keep the SPI alive */
/* See XCP on SPI transport layer specification */
#define XCP_ENABLE_SEND_EVENT


/*----------------------------------------------------------------------------*/
/* XCP Data Acquisition Parameters */

/* Enable data acquisition */
#define XCP_ENABLE_DAQ
#define XCP_DISABLE_DAQ_HDR_ODT_DAQ

/* Memory space reserved for DAQ */
#define kXcpDaqMemSize 1024

/* Use the exented DAQ packet header (ODT+DAQ) */
//#define XCP_ENABLE_DAQ_HDR_ODT_DAQ

/* Enable optional timestamped data acquisition */
#define XCP_ENABLE_DAQ_TIMESTAMP
typedef vuint16 XcpDaqTimestampType;
#define kXcpDaqTimestampSize                 2
#define kXcpDaqTimestampUnit                 DAQ_TIMESTAMP_UNIT_100US
#define kXcpDaqTimestampTicksPerUnit         1
extern vuint32 XCP_100us_Counter;
#define ApplXcpGetTimestamp()                (XcpDaqTimestampType)XCP_100us_Counter
#define ApplXcpDaqGetTimestamp()             (XcpDaqTimestampType)XCP_100us_Counter



/*----------------------------------------------------------------------------*/
/* Optional GET_SEED and UNLOCK */

/* Enable Seed & Key */
// #define XCP_ENABLE_SEED_KEY


/*----------------------------------------------------------------------------*/
/* Optional memory checksum command */

/* Enable memory checksum calculation */
//#define XCP_ENABLE_CHECKSUM
// #define XcpChecksumType        vuint16
// #define kXcpChecksumMethod     kXcpChecksumTypeAdd12
//#define kXcpChecksumMethod 		  XCP_CHECKSUM_TYPE_ADD14


/* Note: The checksum will be calculated in XcpBackground() */


/*----------------------------------------------------------------------------*/
/* There are many other options not mentioned here */
/* Please see the XCP driver documentatiop */

/* Enable calibration */
#define XCP_ENABLE_CALIBRATION
#define XCP_DISABLE_CALIBRATION_PAGE
#define XCP_ENABLE_SHORT_UPLOAD

#endif /* __XCP_CFG_H__ */
