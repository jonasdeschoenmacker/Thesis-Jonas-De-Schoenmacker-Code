/*
 * BQ25713.c
 *
 *  Created on: Feb 11, 2021
 *      Author: dsmjo
 */

#include "BQ25713.h"

uint16_t getChargeOption0(I2C_HandleTypeDef *hi2c) {
	uint8_t data_in[2];
	HAL_I2C_Mem_Read(hi2c, BQ25713Address, ChargeOption0Reg, 1, data_in, 2,
			100);
	return (data_in[1] << 8 | data_in[0]);
}
uint16_t getChargeCurrent(I2C_HandleTypeDef *hi2c) {
	uint8_t data_in[2];
	HAL_I2C_Mem_Read(hi2c, BQ25713Address, ChargeCurrentReg, 1, data_in, 2,
			100);
	return (data_in[1] << 8 | data_in[0]);
}
uint16_t getMaxChargeVoltage(I2C_HandleTypeDef *hi2c) {
	uint8_t data_in[2];
	HAL_I2C_Mem_Read(hi2c, BQ25713Address, MaxChargeVoltageReg, 1, data_in, 2,
			100);
	return (data_in[1] << 8 | data_in[0]);
}
uint16_t getChargeOption1(I2C_HandleTypeDef *hi2c) {
	uint8_t data_in[2];
	HAL_I2C_Mem_Read(hi2c, BQ25713Address, ChargeOption1Reg, 1, data_in, 2,
			100);
	return (data_in[1] << 8 | data_in[0]);
}
uint16_t getChargeOption2(I2C_HandleTypeDef *hi2c) {
	uint8_t data_in[2];
	HAL_I2C_Mem_Read(hi2c, BQ25713Address, ChargeOption2Reg, 1, data_in, 2,
			100);
	return (data_in[1] << 8 | data_in[0]);
}
uint16_t getChargeOption3(I2C_HandleTypeDef *hi2c) {
	uint8_t data_in[2];
	HAL_I2C_Mem_Read(hi2c, BQ25713Address, ChargeOption3Reg, 1, data_in, 2,
			100);
	return (data_in[1] << 8 | data_in[0]);
}
uint16_t getProchotOption0(I2C_HandleTypeDef *hi2c) {
	uint8_t data_in[2];
	HAL_I2C_Mem_Read(hi2c, BQ25713Address, ProchotOption0Reg, 1, data_in, 2,
			100);
	return (data_in[1] << 8 | data_in[0]);
}
uint16_t getProchotOption1(I2C_HandleTypeDef *hi2c) {
	uint8_t data_in[2];
	HAL_I2C_Mem_Read(hi2c, BQ25713Address, ProchotOption1Reg, 1, data_in, 2,
			100);
	return (data_in[1] << 8 | data_in[0]);
}
uint16_t getADCOption(I2C_HandleTypeDef *hi2c) {
	uint8_t data_in[2];
	HAL_I2C_Mem_Read(hi2c, BQ25713Address, ADCOptionReg, 1, data_in, 2, 100);
	return (data_in[1] << 8 | data_in[0]);
}
uint16_t getChargerStatus(I2C_HandleTypeDef *hi2c) {
	uint8_t data_in[2];
	HAL_I2C_Mem_Read(hi2c, BQ25713Address, ChargerStatusReg, 1, data_in, 2,
			100);
	return (data_in[1] << 8 | data_in[0]);
}
uint16_t getProchotStatus(I2C_HandleTypeDef *hi2c) {
	uint8_t data_in[2];
	HAL_I2C_Mem_Read(hi2c, BQ25713Address, ProchotStatusReg, 1, data_in, 2,
			100);
	return (data_in[1] << 8 | data_in[0]);
}
uint16_t getIIN_DPM(I2C_HandleTypeDef *hi2c) {
	uint8_t data_in[2];
	HAL_I2C_Mem_Read(hi2c, BQ25713Address, IIN_DPMReg, 1, data_in, 2, 100);
	return (data_in[1] << 8 | data_in[0]);
}
uint16_t getADC_VBUS_PSYS(I2C_HandleTypeDef *hi2c) {
	uint8_t data_in[2];
	HAL_I2C_Mem_Read(hi2c, BQ25713Address, ADC_VBUS_PSYSReg, 1, data_in, 2,
			100);
	return (data_in[1] << 8 | data_in[0]);
}
uint16_t getADC_IBAT(I2C_HandleTypeDef *hi2c) {
	uint8_t data_in[2];
	HAL_I2C_Mem_Read(hi2c, BQ25713Address, ADC_IBATReg, 1, data_in, 2, 100);
	return (data_in[1] << 8 | data_in[0]);
}
uint16_t getADC_IINCMPIN(I2C_HandleTypeDef *hi2c) {
	uint8_t data_in[2];
	HAL_I2C_Mem_Read(hi2c, BQ25713Address, ADC_IINCMPINReg, 1, data_in, 2, 100);
	return (data_in[1] << 8 | data_in[0]);
}
uint16_t getADC_VSYS_VBAT(I2C_HandleTypeDef *hi2c) {
	uint8_t data_in[2];
	HAL_I2C_Mem_Read(hi2c, BQ25713Address, ADC_VSYS_VBATReg, 1, data_in, 2,
			100);
	return (data_in[1] << 8 | data_in[0]);
}
uint16_t getOTGVoltage(I2C_HandleTypeDef *hi2c) {
	uint8_t data_in[2];
	HAL_I2C_Mem_Read(hi2c, BQ25713Address, OTGVoltageReg, 1, data_in, 2, 100);
	return (data_in[1] << 8 | data_in[0]);
}
uint16_t getOTGCurrent(I2C_HandleTypeDef *hi2c) {
	uint8_t data_in[2];
	HAL_I2C_Mem_Read(hi2c, BQ25713Address, OTGCurrentReg, 1, data_in, 2, 100);
	return (data_in[1] << 8 | data_in[0]);
}
uint16_t getInputVoltage(I2C_HandleTypeDef *hi2c) {
	uint8_t data_in[2];
	HAL_I2C_Mem_Read(hi2c, BQ25713Address, InputVoltageReg, 1, data_in, 2, 100);
	return (data_in[1] << 8 | data_in[0]);
}
uint16_t getMinSystemVoltage(I2C_HandleTypeDef *hi2c) {
	uint8_t data_in[2];
	HAL_I2C_Mem_Read(hi2c, BQ25713Address, MinSystemVoltageReg, 1, data_in, 2,
			100);
	return (data_in[1] << 8 | data_in[0]);
}
uint16_t getIIN_HOST(I2C_HandleTypeDef *hi2c) {
	uint8_t data_in[2];
	HAL_I2C_Mem_Read(hi2c, BQ25713Address, IIN_HOSTReg, 1, data_in, 2, 100);
	return (data_in[1] << 8 | data_in[0]);
}
uint16_t getManufactureID(I2C_HandleTypeDef *hi2c) {
	uint8_t data_in[2];
	HAL_I2C_Mem_Read(hi2c, BQ25713Address, ManufactureIDReg, 1, data_in, 2,
			100);
	return (data_in[1] << 8 | data_in[0]);
}
uint16_t getDeviceID(I2C_HandleTypeDef *hi2c) {
	uint8_t data_in[2];
	HAL_I2C_Mem_Read(hi2c, BQ25713Address, DeviceIDReg, 1, data_in, 2, 100);
	return (data_in[1] << 8 | data_in[0]);
}

uint8_t setRegBQ25713(I2C_HandleTypeDef *hi2c, uint8_t addres, uint8_t *data) {
	HAL_I2C_Mem_Write(hi2c, BQ25713Address, addres, 1, data, 2, 100);

	return 0;
}


