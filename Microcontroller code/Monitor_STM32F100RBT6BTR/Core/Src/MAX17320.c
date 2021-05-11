/*
 * MAX17320.c
 *
 *  Created on: 7 dec. 2020
 *      Author: dsmjo
 */

#include "MAX17320.h"

void MAX17320Init(I2C_HandleTypeDef *hi2c) {
	hi2c = hi2c;
}
uint16_t FullReset(I2C_HandleTypeDef *hi2c) {
	uint8_t data_in[2];
	data_in[0] = 0x00; //LSB
	data_in[1] = 0x00; //MSB

	HAL_I2C_Mem_Write(hi2c, 0x6C, CommStatReg, 1, data_in, 2, 100);
	HAL_I2C_Mem_Write(hi2c, 0x6C, CommStatReg, 1, data_in, 2, 100);
	data_in[0] = 0x0F; //LSB
	data_in[1] = 0x00; //MSB
	HAL_I2C_Mem_Write(hi2c, 0x6C, CommandReg, 1, data_in, 2, 100);
	HAL_Delay(10);
	data_in[0] = 0x00; //LSB
	data_in[1] = 0x00; //MSB

	HAL_I2C_Mem_Write(hi2c, 0x6C, CommStatReg, 1, data_in, 2, 100);
	HAL_I2C_Mem_Write(hi2c, 0x6C, CommStatReg, 1, data_in, 2, 100);
	//HAL_Delay(10);
	data_in[0] = 0x00; //LSB
	data_in[1] = 0x80; //MSB
	HAL_I2C_Mem_Write(hi2c, 0x6C, Config2Reg, 1, data_in, 2, 100);
	//HAL_Delay(10);
	uint8_t data_read[2];
	HAL_I2C_Mem_Read(hi2c, 0x16, Config2Reg, 1, data_read, 2, 100);
	while ((data_read[1] & 0x80) != 0x00) {
		HAL_Delay(10);
		HAL_I2C_Mem_Read(hi2c, 0x6C, Config2Reg, 1, data_read, 2, 100);
	}
	HAL_Delay(10);
	data_in[0] = 0xF9; //LSB
	data_in[1] = 0x00; //MSB
	HAL_I2C_Mem_Write(hi2c, 0x6C, CommStatReg, 1, data_in, 2, 100);
	HAL_I2C_Mem_Write(hi2c, 0x6C, CommStatReg, 1, data_in, 2, 100);
	return 0;
}

float getAvgDieTemp(I2C_HandleTypeDef *hi2c) {

	uint8_t data_in[2];
	HAL_I2C_Mem_Read(hi2c, 0x6C, AvgDieTempReg, 1, data_in, 2, 100); //read AvgDieTemp
	return (data_in[1] << 8 | data_in[0]) * 0.00391;
}

float getTemp1(I2C_HandleTypeDef *hi2c) {

	uint8_t data_in[2];
	HAL_I2C_Mem_Read(hi2c, 0x16, AvgTemp1Reg, 1, data_in, 2, 100); //read AvgTemp1
	return (data_in[1] << 8 | data_in[0]) * 0.00391;
}
float getTemp2(I2C_HandleTypeDef *hi2c) {

	uint8_t data_in[2];
	HAL_I2C_Mem_Read(hi2c, 0x16, AvgTemp2Reg, 1, data_in, 2, 100); //read AvgTemp2
	return (data_in[1] << 8 | data_in[0]) * 0.00391;
}
float getTemp3(I2C_HandleTypeDef *hi2c) {

	uint8_t data_in[2];
	HAL_I2C_Mem_Read(hi2c, 0x16, AvgTemp3Reg, 1, data_in, 2, 100); //read AvgTemp3
	return (data_in[1] << 8 | data_in[0]) * 0.00391;
}
float getTemp4(I2C_HandleTypeDef *hi2c) {

	uint8_t data_in[2];
	HAL_I2C_Mem_Read(hi2c, 0x16, AvgTemp4Reg, 1, data_in, 2, 100); //read AvgTemp4
	return (data_in[1] << 8 | data_in[0]) * 0.00391;
}

float getVC1(I2C_HandleTypeDef *hi2c) {

	uint8_t data_in[2];
	HAL_I2C_Mem_Read(hi2c, 0x6C, AvgVC1Reg, 1, data_in, 2, 100); //read AvgVC1
	return (data_in[1] << 8 | data_in[0]) * 0.000078125;
}

float getVC2(I2C_HandleTypeDef *hi2c) {

	uint8_t data_in[2];
	HAL_I2C_Mem_Read(hi2c, 0x6C, AvgVC2Reg, 1, data_in, 2, 100); //read AvgVC2
	return (data_in[1] << 8 | data_in[0]) * 0.000078125;
}
float getVC3(I2C_HandleTypeDef *hi2c) {

	uint8_t data_in[2];
	HAL_I2C_Mem_Read(hi2c, 0x6C, AvgVC3Reg, 1, data_in, 2, 100); //read AvgVC3
	return (data_in[1] << 8 | data_in[0]) * 0.000078125;
}
float getVC4(I2C_HandleTypeDef *hi2c) {

	uint8_t data_in[2];
	HAL_I2C_Mem_Read(hi2c, 0x6C, AvgVC4Reg, 1, data_in, 2, 100); //read AvgVC4
	return (data_in[1] << 8 | data_in[0]) * 0.000078125;
}

float getVT(I2C_HandleTypeDef *hi2c) {
	uint8_t data_in[2];
	HAL_I2C_Mem_Read(hi2c, 0x6C, PCKPReg, 1, data_in, 2, 100); //read packV
	return (data_in[1] << 8 | data_in[0]) * 0.0003125;
}
float getCurrent(I2C_HandleTypeDef *hi2c) {
	uint8_t data_in[2];
	HAL_I2C_Mem_Read(hi2c, 0x6C, AvgCurrentReg, 1, data_in, 2, 100); //read AvgCurrent
	//return (data_in[1]<<8 | data_in[0]) * 0.00025;
	return ((int16_t) (data_in[1] << 8 | data_in[0])) * 0.00078125;
}

float getRepCap(I2C_HandleTypeDef *hi2c) {
	uint8_t data_in[2];
	HAL_I2C_Mem_Read(hi2c, 0x6C, RepCapReg, 1, data_in, 2, 100);

	return ((int16_t) (data_in[1] << 8 | data_in[0])) * 0.0025; //0.000005/Rsense (2mohm);
}

float getRepSoC(I2C_HandleTypeDef *hi2c) {
	uint8_t data_in[2];
	HAL_I2C_Mem_Read(hi2c, 0x6C, RepSOCReg, 1, data_in, 2, 100);

	return ((int16_t) (data_in[1] << 8 | data_in[0])) * 0.00390625;
}
float getTTE(I2C_HandleTypeDef *hi2c) {
	uint8_t data_in[2];
	HAL_I2C_Mem_Read(hi2c, 0x6C, TTEReg, 1, data_in, 2, 100);

	return ((int16_t) (data_in[1] << 8 | data_in[0])) * 5.625;
}
float getTTF(I2C_HandleTypeDef *hi2c) {
	uint8_t data_in[2];
	HAL_I2C_Mem_Read(hi2c, 0x6C, TTFReg, 1, data_in, 2, 100);

	return ((int16_t) (data_in[1] << 8 | data_in[0])) * 5.625;
}
uint8_t nsetReg(I2C_HandleTypeDef *hi2c, uint8_t addres, uint8_t *data) {

	uint8_t data_in_reg[2];
	data_in_reg[0] = 0x01;
	data_in_reg[1] = 0x02;
	HAL_I2C_Mem_Read(hi2c, 0x16, addres, 1, data_in_reg, 2, 100);
	/*if (HAL_I2C_Mem_Read(hi2c, 0x16, addres, 1, data_in_reg, 2, 100)
			!= HAL_OK) {
		return 255;
	}*/
	if ((data_in_reg[0] == data[0]) & (data_in_reg[1] == data[1])) {
		return 1;
	} else {
		uint8_t data_in[2];
		data_in[0] = 0x00; //LSB
		data_in[1] = 0x00; //MSB

		HAL_I2C_Mem_Write(hi2c, 0x6C, CommStatReg, 1, data_in, 2, 100);
		HAL_I2C_Mem_Write(hi2c, 0x6C, CommStatReg, 1, data_in, 2, 100);

		HAL_I2C_Mem_Write(hi2c, 0x16, addres, 1, data, 2, 100);

		//HAL_I2C_Mem_Read(&hi2c1, 0x16, 0xB5, 1, nPackCfg, 2, 100);

		data_in[0] = 0xF9; //LSB
		data_in[1] = 0x00; //MSB
		HAL_I2C_Mem_Write(hi2c, 0x6C, CommStatReg, 1, data_in, 2, 100);
		HAL_I2C_Mem_Write(hi2c, 0x6C, CommStatReg, 1, data_in, 2, 100);
	}

	return 0;

}
uint8_t nsetRegMulti(I2C_HandleTypeDef *hi2c, uint8_t *addres, uint8_t *data,
		uint8_t length) {
	for (int var = 0; var < length; var++) {
		nsetReg(hi2c, addres[var], data[var]);
	}
	return 0;
}
uint8_t setReg(I2C_HandleTypeDef *hi2c, uint8_t addres, uint8_t *data) {
	HAL_I2C_Mem_Write(hi2c, 0x6C, addres, 1, data, 2, 100);
	return 0;
}
/*uint8_t* ReadAllNReg(I2C_HandleTypeDef *hi2c, uint8_t aantalReg, uint8_t aantalBytes){
 adressen[] =
 HAL_I2C_Mem_Read(&hi2c1, 0x16, 0xB5, 1, nPackCfg, 2, 100);
 }*/

uint16_t getStatus(I2C_HandleTypeDef *hi2c) {
	uint8_t data_in[2];
	HAL_I2C_Mem_Read(hi2c, 0x6C, StatusReg, 1, data_in, 2, 100);
	return (data_in[1] << 8 | data_in[0]);
}
uint16_t getStatus2(I2C_HandleTypeDef *hi2c) {
	uint8_t data_in[2];
	HAL_I2C_Mem_Read(hi2c, 0x6C, Status2Reg, 1, data_in, 2, 100);
	return (data_in[1] << 8 | data_in[0]);
}
uint16_t getProtStatus(I2C_HandleTypeDef *hi2c) {
	uint8_t data_in[2];
	HAL_I2C_Mem_Read(hi2c, 0x6C, ProtStatusReg, 1, data_in, 2, 100);
	return (data_in[1] << 8 | data_in[0]);
}
uint16_t getProtAlrtStatus(I2C_HandleTypeDef *hi2c) {
	uint8_t data_in[2];
	HAL_I2C_Mem_Read(hi2c, 0x6C, ProtAlrtReg, 1, data_in, 2, 100);
	return (data_in[1] << 8 | data_in[0]);
}
uint16_t getCommStatus(I2C_HandleTypeDef *hi2c) {
	uint8_t data_in[2];
	HAL_I2C_Mem_Read(hi2c, 0x6C, CommStatReg, 1, data_in, 2, 100);
	return (data_in[1] << 8 | data_in[0]);
}
