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

#endif /* SRC_XCP_MEASURE_CALIBRATE_H_ */
