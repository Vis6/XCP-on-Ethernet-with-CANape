/*
 * xcp_measure_calibrate.h
 *
 *  Created on: Dec 11, 2020
 *      Author: xzhan134
 */

#ifndef SRC_XCP_MEASURE_CALIBRATE_H_
#define SRC_XCP_MEASURE_CALIBRATE_H_

#ifndef _MATH_H_
#define _MATH_H_
#include <math.h>
#endif

/* include FPGA library here... */
/* 
#############################################
#ifndef _CM_H_
#define _CM_H_
#include "cm.h"
#endif
#############################################
*/

//typedef uint8_t u8;
//typedef uint16_t u16;
//typedef uint32_t u32;

int calibarteVarsInit();
int xcpVarsRead100us();
int xcpVarsUpdate();
u32 floatToUFix(float data, int fraction);
float uFixToFloat(u32 data, int fraction);
int floatToSFix(float data, int fraction);
float sFixToFloat(int data, int fraction);
u32 floatToIeeeFloat(float x);

#endif /* SRC_XCP_MEASURE_CALIBRATE_H_ */
