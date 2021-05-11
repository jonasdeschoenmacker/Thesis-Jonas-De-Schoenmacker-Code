/*
 * BQ25713.h
 *
 *  Created on: Feb 11, 2021
 *      Author: dsmjo
 */

#ifndef BQ25713_H_
#define BQ25713_H_

#include "stm32f1xx_hal.h"

//BQ25713 device address
#define BQ25713Address			0xD6

//BQ25713 registers
//all registers are 2 bytes long

#define ChargeOption0Reg		0x00 // R/W
#define ChargeCurrentReg		0x02 // R/W
#define MaxChargeVoltageReg		0x04 // R/W
#define ChargeOption1Reg		0x30 // R/W
#define ChargeOption2Reg		0x32 // R/W
#define ChargeOption3Reg		0x34 // R/W
#define ProchotOption0Reg		0x36 // R/W
#define ProchotOption1Reg		0x38 // R/W
#define ADCOptionReg			0x3A // R/W
#define ChargerStatusReg		0x20 // R
#define ProchotStatusReg		0x22 // R
#define IIN_DPMReg				0x24 // R
#define ADC_VBUS_PSYSReg		0x26 // R
#define ADC_IBATReg				0x28 // R
#define ADC_IINCMPINReg			0x2A // R
#define ADC_VSYS_VBATReg		0x2C // R
#define OTGVoltageReg			0x06 // R/W
#define OTGCurrentReg			0x08 // R/W
#define InputVoltageReg			0x0A // R/W
#define MinSystemVoltageReg		0x0C // R/W
#define IIN_HOSTReg				0x0E // R/W
#define ManufactureIDReg		0x2E // R		=0x0040
#define DeviceIDReg				0x2F // R

uint16_t getChargeOption0(I2C_HandleTypeDef *hi2c);
uint16_t getChargeCurrent(I2C_HandleTypeDef *hi2c);
uint16_t getMaxChargeVoltage(I2C_HandleTypeDef *hi2c);
uint16_t getChargeOption1(I2C_HandleTypeDef *hi2c);
uint16_t getChargeOption2(I2C_HandleTypeDef *hi2c);
uint16_t getChargeOption3(I2C_HandleTypeDef *hi2c);
uint16_t getProchotOption0(I2C_HandleTypeDef *hi2c);
uint16_t getProchotOption1(I2C_HandleTypeDef *hi2c);
uint16_t getADCOption(I2C_HandleTypeDef *hi2c);
uint16_t getChargerStatus(I2C_HandleTypeDef *hi2c);
uint16_t getProchotStatus(I2C_HandleTypeDef *hi2c);
uint16_t getIIN_DPM(I2C_HandleTypeDef *hi2c);
uint16_t getADC_VBUS_PSYS(I2C_HandleTypeDef *hi2c);
uint16_t getADC_IBAT(I2C_HandleTypeDef *hi2c);
uint16_t getADC_IINCMPIN(I2C_HandleTypeDef *hi2c);
uint16_t getADC_VSYS_VBAT(I2C_HandleTypeDef *hi2c);
uint16_t getOTGVoltage(I2C_HandleTypeDef *hi2c);
uint16_t getOTGCurrent(I2C_HandleTypeDef *hi2c);
uint16_t getInputVoltage(I2C_HandleTypeDef *hi2c);
uint16_t getMinSystemVoltage(I2C_HandleTypeDef *hi2c);
uint16_t getIIN_HOST(I2C_HandleTypeDef *hi2c);
uint16_t getManufactureID(I2C_HandleTypeDef *hi2c);
uint16_t getDeviceID(I2C_HandleTypeDef *hi2c);

uint8_t setRegBQ25713(I2C_HandleTypeDef *hi2c, uint8_t addres, uint8_t *data);



#endif /* BQ25713_H_ */
