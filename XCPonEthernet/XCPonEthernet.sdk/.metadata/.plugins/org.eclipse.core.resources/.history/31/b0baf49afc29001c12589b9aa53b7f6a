/*
 * xcp_measure_calibrate.h
 *
 *  Created on: Dec 11, 2020
 *      Author: xzhan134
 */

#ifndef SRC_XCP_MEASURE_CALIBRATE_H_
#define SRC_XCP_MEASURE_CALIBRATE_H_

//typedef uint8_t u8;
//typedef uint16_t u16;
//typedef uint32_t u32;

int calibrateVarsInit();
int xcpVarsRead100us();
int xcpVarsUpdate();
u32 floatToUFix(float data, int fraction);
float uFixToFloat(u32 data, int fraction);
int floatToSFix(float data, int fraction);
float sFixToFloat(int data, int fraction);
u32 floatToIeeeFloat(float x);

#endif /* SRC_XCP_MEASURE_CALIBRATE_H_ */
