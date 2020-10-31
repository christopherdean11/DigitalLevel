///*
// * accelerometer.c
// *
// *  Created on: Oct 19, 2020
// *      Author: christopherdean
// */

#include "accelerometer.h"
#include "led.h"

#define HALF_INT16 32768


uint32_t I2C_TIMEOUT = 5;

void ACCEL_verifyI2C(I2C_HandleTypeDef *h, uint16_t deviceAddress, uint8_t adrToCheck, uint8_t expectedResponse){
	uint8_t i2cbuf[1];
	i2cbuf[0] = adrToCheck;
	uint32_t timeout = 10;
	HAL_I2C_Master_Transmit(h, deviceAddress, i2cbuf, 1, timeout);
	HAL_I2C_Master_Receive(h, deviceAddress, i2cbuf, 1, timeout);
	if (*i2cbuf != expectedResponse){
		LED_errorState();
	}
}

void ACCEL_enableMeasurements(I2C_HandleTypeDef *hi2c){
	// enable measurement mode
	uint8_t i2cbuf[2];
	i2cbuf[0] = 0x2D; // POWER_CTL address
	i2cbuf[1] = (1 << 3);  // set bit3 to put into measure mode and out of sleep
	HAL_I2C_Master_Transmit(hi2c, ADXL343_ADDRESS, i2cbuf, 2, I2C_TIMEOUT);
}

uint8_t ACCEL_readReg(I2C_HandleTypeDef *hi2c,uint8_t address){
	uint8_t i2cbuf[1];
	i2cbuf[0] = address;
	HAL_I2C_Master_Transmit(hi2c, ADXL343_ADDRESS, i2cbuf, 1, I2C_TIMEOUT);
	HAL_I2C_Master_Receive( hi2c, ADXL343_ADDRESS, i2cbuf, 1, I2C_TIMEOUT);
	return i2cbuf[0];
}

void ACCEL_readRegs(I2C_HandleTypeDef *hi2c,uint8_t address, uint8_t* outbuf, uint16_t length){
	uint8_t writebuf[1];
	writebuf[0] = address;
	HAL_I2C_Master_Transmit(hi2c, ADXL343_ADDRESS, writebuf, 1, I2C_TIMEOUT);
	HAL_I2C_Master_Receive( hi2c, ADXL343_ADDRESS, outbuf, length, I2C_TIMEOUT);
	return;
}

float ACCEL_getX(I2C_HandleTypeDef *hi2c){
	return ACCELp_readAcceleration(hi2c, 0x32);
}

float ACCEL_getY(I2C_HandleTypeDef *hi2c){
	return ACCELp_readAcceleration(hi2c, 0x34);
}
float ACCEL_getZ(I2C_HandleTypeDef *hi2c){
	return ACCELp_readAcceleration(hi2c, 0x36);
}

// "p_" in name to denote as "private function"
float ACCELp_readAcceleration(I2C_HandleTypeDef *hi2c, uint8_t address){
	float g;
	uint8_t outbuf[2];
	ACCEL_readRegs(hi2c, address, outbuf, 2);
	int16_t data = outbuf[0] | ( outbuf[1]<<8 );
//	if (data >= HALF_INT16){
//		data = data - HALF_INT16;
//	}
	g = data * 0.004;
	return g;
}




//	  i2cbuf[0] = 0x31; // DATA_FORMAT address
//	  i2cbuf[1] = (1 << 3) | 1;  // set bit3 to put into FULL_RES mode, for 4mg/LSB
//	  HAL_I2C_Master_Transmit(&hi2c1, ADXL343_ADDRESS, i2cbuf, 2, timeout);
//
//	  // check DATA_FORMAT
//	  i2cbuf[0] = 0x31; // DATA_FORMAT address
//	  HAL_I2C_Master_Transmit(&hi2c1, ADXL343_ADDRESS, i2cbuf, 1, timeout);
//	  HAL_I2C_Master_Receive( &hi2c1, ADXL343_ADDRESS, i2cbuf, 1, timeout);
//
//	  // Get X DATA
//	  i2cbuf[0] = 0x32;
//	  HAL_I2C_Master_Transmit(&hi2c1, ADXL343_ADDRESS, i2cbuf, 1, timeout);
//	  HAL_I2C_Master_Receive( &hi2c1, ADXL343_ADDRESS, i2cbuf, 2, timeout);
//
//	  xdata = i2cbuf[0] + (i2cbuf[1] << 8);
//	  // 4mg/LSB
//	  gx = xdata * 0.004;
//
//	  int16_t led_id;
//	  if (gx > 0){
//		  led_id = -gx / 0.25;
//		  led_id = 4 - led_id;
//		  writeLedState(led_id, 1);
//	  }
//	  if (gx < 0){
//		  led_id = gx / 0.25;
//		  led_id = 5 + led_id;
//		  writeLedState(led_id, 1);
//	  }
