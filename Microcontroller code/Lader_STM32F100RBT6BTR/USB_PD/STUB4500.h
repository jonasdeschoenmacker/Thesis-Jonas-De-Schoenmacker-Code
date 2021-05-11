/*
 * STUB4500.h
 *
 *  Created on: Feb 11, 2021
 *      Author: dsmjo
 */

#ifndef STUB4500_H_
#define STUB4500_H_
#include "stm32f1xx_hal.h"
#define DeviceAddress		   0x50

//registers
#define deviceIDReg				0x2F //resetstate: 0x25

#define PORT_STATUS_0Reg		0x0D

void beginSTUB4500(I2C_HandleTypeDef *hi2c);

float getManufacture(I2C_HandleTypeDef *hi2c);

#endif /* STUB4500_H_ */
