/*
 * accelerometer.h
 *
 *  Created on: Oct 19, 2020
 *      Author: christopherdean
 */

#ifndef SRC_ACCELEROMETER_H_
#define SRC_ACCELEROMETER_H_
#include "stm32f0xx_hal.h"

#define ADXL343_ADDRESS  (uint16_t)(0x53<<1) // 7bit address shifted by 1
#define ACCEL_X_ID 0
#define ACCEL_Y_ID 1
#define ACCEL_Z_ID 2


void ACCEL_verifyI2C(I2C_HandleTypeDef *, uint16_t deviceAddress, uint8_t adrToCheck, uint8_t expectedResponse);
void ACCEL_enableMeasurements(I2C_HandleTypeDef *);
uint8_t ACCEL_readReg(I2C_HandleTypeDef *, uint8_t address);
void ACCEL_readRegs(I2C_HandleTypeDef *hi2c,uint8_t address, uint8_t* outbuf, uint16_t length);

float ACCEL_getX(I2C_HandleTypeDef *hi2c);
float ACCEL_getY(I2C_HandleTypeDef *hi2c);
float ACCEL_getZ(I2C_HandleTypeDef *hi2c);

float ACCELp_readAcceleration(I2C_HandleTypeDef *hi2c, uint8_t address);

#endif /* SRC_ACCELEROMETER_H_ */
