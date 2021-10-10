/*
 * xcp_measurement_calibrate.c
 *
 *  Created on: Dec 11, 2020
 *      Author: xzhan134
 */

#include "xil_types.h"
#include "xcp_measure_calibrate.h"

// measurement variables declare
/* add measurement variables here...
###########################
static float xcp_ia, xcp_ib, xcp_ic;
###########################d
*/

// characteristics variables declare
/* add characteristics variables here...
###########################
static u8 xcp_pi_rst = 1;
###########################d
*/

// FPGA
/* add extern FPGA variables here...
###########################
extern cm cm_instance;
###########################d
*/

// union
typedef union
{
	float f;
	struct
	{
		unsigned int mantissa : 23;
		unsigned int exponent : 8;
		unsigned int sign : 1;
	} field;
} myfloat;

int calibarteVarsInit()
{
	/* Init input values in FPGA model here... */

	return 0;
}

int xcpVarsRead100us()
{
	/* read output data from FPGA */

	return 0;
}

int xcpVarsUpdate()
{
	/* update */

	return 0;
}

u32 floatToUFix(float data, int fraction)
{
	u32 res = (u32)((int)data * pow(2.0, fraction)) + (u32)((data - (int)data) * pow(2.0, fraction));

	return res;
}

int floatToSFix(float data, int fraction)
{
	int res = data * pow(2.0, fraction);

	return res;
}

float uFixToFloat(u32 data, int fraction)
{
	float res = data / pow(2.0, fraction);

	return res;
}

float sFixToFloat(int data, int fraction)
{
	float res = data / pow(2.0, fraction);

	return res;
}

u32 floatToIeeeFloat(float data)
{
	myfloat var;
	u32 ieee_float;
	var.f = data;

	u32 sign = var.field.sign * pow(2.0, 31.0);
	u32 exponent = var.field.exponent * pow(2.0, 23.0);
	u32 mantissa = var.field.mantissa;
	ieee_float = sign + exponent + mantissa;

	return ieee_float;
}
