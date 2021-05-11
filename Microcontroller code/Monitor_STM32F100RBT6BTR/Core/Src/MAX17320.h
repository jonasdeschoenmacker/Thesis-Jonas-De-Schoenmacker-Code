/*
 * MAX17320.h
 *
 *  Created on: 7 dec. 2020
 *      Author: dsmjo
 */

#ifndef MAX17320_H_
#define MAX17320_H_

#include "stm32f1xx_hal.h"

//normal mem reg
#define StatusReg			0x00 //Alert status and chip status
#define RepCapReg			0x05 //Reported remaining capacity
#define RepSOCReg			0x06 //Reported state of charge
#define VCellReg			0x1A //Cell voltage for a single cell
#define TempReg				0x1B //Temperature
#define CurrentReg			0x1C //Battery current
#define TTEReg				0x11 //Time to empty
#define TTFReg				0x20 //Time to full
#define	ProtStatusReg		0xD9 //Fault status of the protection functionality
#define ProtAlrtReg			0xAF //History of previous fault status of the protection functionality



#define AvgDieTempReg 		0x34
#define AvgVC1Reg			0xD1
#define AvgVC2Reg			0xD2
#define AvgVC3Reg			0xD3
#define AvgVC4Reg			0xD4
#define VC1Reg				0xD5
#define VC2Reg				0xD6
#define VC3Reg				0xD7
#define VC4Reg				0xD8
#define PCKPReg				0xDB
#define CommStatReg			0x61
#define AvgCurrentReg		0x1D
#define Status2Reg			0xB0
#define CommandReg			0x60
#define Config2Reg			0xAB

//Nonvolatile mem reg
#define nPackCfgReg 		0xB5
#define nUVPrtThReg 		0xD0
#define nJEITAVReg			0xD9
#define nOVPrtThReg 		0xDA
#define nBalThReg			0xD4
#define nODSCThReg			0xDD
#define nODSCCfgReg			0xDE
#define nIPrtTh1Reg			0xD3
#define nJEITACReg			0xD8
#define nTPrtTh1Reg			0xD1
#define nTPrtTh2Reg			0xD5
#define nTPrtTh3Reg			0xD2
#define nProtMiscThReg		0xD6
#define nDelayCfgReg		0xDC
#define nProtCfg2Reg		0xDF
#define nTCurveReg			0xC9
#define LeakCurrRepReg		0x6F
#define nProtCfgReg			0xD7

#define nBattStatusReg		0xA8
#define nFaultLogReg		0xAE
#define nCGainReg			0xC8
#define nStepChgReg			0xDB
#define nChgCfgReg			0xC2
#define nConfigReg			0xB0
#define nNVCfg0Reg			0xB8
#define nNVCfg1Reg			0xB9
#define nNVCfg2Reg			0xBA
#define nHibCfgReg			0xBB
#define nTTFCfgReg			0xC7
#define nConvgCfgReg		0xB7
#define nRippleCfgReg		0xB1
#define nROMIDReg			0xBC
#define nHProtCfg2Reg		0xF1
#define nTAlrtThReg			0x8D

#define AvgTemp1Reg 		0x36
#define AvgTemp2Reg 		0x35
#define AvgTemp3Reg 		0x34
#define AvgTemp4Reg 		0x33
#define nRSenseReg			0xCF


//I2C_HandleTypeDef hi2c;

//void MAX17320Init(I2C_HandleTypeDef *hi2c);

uint16_t FullReset(I2C_HandleTypeDef *hi2c);

float getAvgDieTemp(I2C_HandleTypeDef *hi2c);
float getTemp1(I2C_HandleTypeDef *hi2c);
float getTemp2(I2C_HandleTypeDef *hi2c);
float getTemp3(I2C_HandleTypeDef *hi2c);
float getTemp4(I2C_HandleTypeDef *hi2c);

float getVC1(I2C_HandleTypeDef *hi2c);
float getVC2(I2C_HandleTypeDef *hi2c);
float getVC3(I2C_HandleTypeDef *hi2c);
float getVC4(I2C_HandleTypeDef *hi2c);
float getVT(I2C_HandleTypeDef *hi2c);
float getCurrent(I2C_HandleTypeDef *hi2c);
uint16_t getStatus(I2C_HandleTypeDef *hi2c);
uint16_t getStatus2(I2C_HandleTypeDef *hi2c);
uint16_t getProtStatus(I2C_HandleTypeDef *hi2c);
uint16_t getProtAlrtStatus(I2C_HandleTypeDef *hi2c);
uint16_t getCommStatus(I2C_HandleTypeDef *hi2c);

float getRepCap(I2C_HandleTypeDef *hi2c);
float getRepSoC(I2C_HandleTypeDef *hi2c);

float getTTE(I2C_HandleTypeDef *hi2c);
float getTTF(I2C_HandleTypeDef *hi2c);


uint8_t* ReadAllNReg(I2C_HandleTypeDef *hi2c, uint8_t aantalReg, uint8_t aantalBytes);

uint8_t nsetReg(I2C_HandleTypeDef *hi2c, uint8_t addres , uint8_t *data);
uint8_t nsetRegMulti(I2C_HandleTypeDef *hi2c, uint8_t *addres , uint8_t *data, uint8_t length);
uint8_t setReg(I2C_HandleTypeDef *hi2c, uint8_t addres , uint8_t *data);

#endif /* MAX17320_H_ */
