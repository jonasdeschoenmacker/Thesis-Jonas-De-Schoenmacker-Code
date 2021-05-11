/*
 * STUB4500.c
 *
 *  Created on: Feb 11, 2021
 *      Author: dsmjo
 */

#ifndef STUB4500_C_
#define STUB4500_C_

#include "STUB4500.h"

void beginSTUB4500(I2C_HandleTypeDef *hi2c){
	//Clear all interrupts by reading (I²C multi-read command for instance) all 10 registers from address 0x0D to 0x16
	uint8_t data_in[10];
	HAL_I2C_Mem_Read(hi2c, DeviceAddress, PORT_STATUS_0Reg, 1, data_in, 10, 100);
}

float getManufacture(I2C_HandleTypeDef *hi2c){

}

#endif /* STUB4500_C_ */
