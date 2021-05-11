/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "string.h"

#include "MAX17320.h"
#include "AT_Comands_variables.h"
#include <stdio.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define LED_port GPIOB	//GPIO_TypeDef for LED port
#define Red_LED_pin GPIO_PIN_14 		//Red LED pin nr.
#define Green_LED_pin GPIO_PIN_13	//Green LED pin nr.
#define Blue_LED_pin GPIO_PIN_12		//Blue pin pin nr.

#define RxBufferLength 	24

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

TIM_HandleTypeDef htim1;

UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_rx;

/* USER CODE BEGIN PV */
I2C_HandleTypeDef hi2c1;

TIM_HandleTypeDef htim1;

UART_HandleTypeDef huart1;

float avgDieTemp;
float avgTemp1;
float avgTemp2;
float avgTemp3;
float avgTemp4;
float VC1;

uint8_t rxBuffer[RxBufferLength];
uint8_t rxByte;
uint16_t rxIdx;
uint8_t rxFlag;

uint8_t data[2]; //dit is array data 1 die ik in dataReg[0] wil zetten....
uint8_t data2[2]; //dit is array data 2 die ik in dataReg[1] wil zetten....
uint8_t dataReg[2];

uint8_t addressReg[2];
uint8_t nPackCfg[2];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_TIM1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void RGBTurnOff() {
	HAL_GPIO_WritePin(Red_LED_GPIO_Port, Red_LED_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(Green_LED_GPIO_Port, Green_LED_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(Blue_LED_GPIO_Port, Blue_LED_Pin, GPIO_PIN_SET);
}
void RGBTurnRED() {
	HAL_GPIO_WritePin(Red_LED_GPIO_Port, Red_LED_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Green_LED_GPIO_Port, Green_LED_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(Blue_LED_GPIO_Port, Blue_LED_Pin, GPIO_PIN_SET);
}
void RGBTurnBlue() {
	HAL_GPIO_WritePin(Red_LED_GPIO_Port, Red_LED_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(Green_LED_GPIO_Port, Green_LED_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(Blue_LED_GPIO_Port, Blue_LED_Pin, GPIO_PIN_RESET);
}
void RGBTurnGreen() {
	HAL_GPIO_WritePin(Red_LED_GPIO_Port, Red_LED_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(Green_LED_GPIO_Port, Green_LED_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Blue_LED_GPIO_Port, Blue_LED_Pin, GPIO_PIN_SET);
}
void printDecimal(int data) {
	char buffer1[16];
	HAL_UART_Transmit(&huart1, (uint8_t*) buffer1,
			sprintf(buffer1, "%d\r\n", data), 500);

}

void printFloat(float dataFloat) {
	char buffer1[16];
	HAL_UART_Transmit(&huart1, (uint8_t*) buffer1,
			sprintf(buffer1, "%+.4f\r\n", dataFloat), 500);
}

void printHex(uint16_t data) {
	char buffer[16];
	HAL_UART_Transmit(&huart1, (uint8_t*) buffer,
			sprintf(buffer, "0x%04X\r\n", data), 500);

}
void printString(char *str) {
	HAL_UART_Transmit(&huart1, (uint8_t*) str, strlen(str), 100);
}
void printlnString(char *str) {

	HAL_UART_Transmit(&huart1, (uint8_t*) str, strlen(str), 100);
	printString("\r\n");
}

void printAT_Comand_Error(int errorNr) {
	printString("ERROR ");
	printDecimal(errorNr);
}

/*void printString(char dataString[]){
 char* buffer = dataString;
 HAL_UART_Transmit(&huart1, buffer, sizeof(buffer), 500);
 HAL_UART_Transmit(&huart1, "\r\n", sizeof("\r\n"), 100);

 }*/
/*
 *      Remove given section from string. Negative len means remove
 *      everything up to the end.
 */
int str_cut(char *str, int begin, int len) {
	int l = strlen(str);

	if (len < 0)
		len = l - begin;
	if (begin + len > l)
		len = l - begin;
	memmove(str + begin, str + begin + len, l - len + 1);

	return len;
}

uint32_t stringToInt(char *str) {
	str_cut(str, 4, -1);

	//int l = strlen(str);
	return str[0] << 24 | str[1] << 16 | str[2] << 8 | str[3];

}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	if (huart->Instance == USART1) {
		__HAL_UART_FLUSH_DRREGISTER(&huart1);
		if (rxByte == 13 || rxByte == 10 || rxIdx >= 23) {
			//rxBuffer[rxIdx] = rxByte;
			rxFlag = 1;
			rxIdx = 0;
		} else {
			rxBuffer[rxIdx] = rxByte;
			rxIdx++;
		}
	}

}
void MAX17320Config() {
	//settings of MAX17320
	//boot up time
	RGBTurnBlue();

	//FullReset(&hi2c1);

	HAL_Delay(500); //wait 500ms to make sure MAX17320 is ready

	data[0] = 0x0A; //LSB
	data[1] = 0x22; //MSB

	nsetReg(&hi2c1, nPackCfgReg, data);

	//data[0] = 0x00; //LSB
	//data[1] = 0x00; //MSB
	//nsetReg(&hi2c1, nProtCfgReg, data);

	data[0] = 0xC8; //LSB
	data[1] = 0x00; //MSB
	nsetReg(&hi2c1, nRSenseReg, data);
	data[0] = 0x4B; //LSB
	data[1] = 0x01; //MSB
	nsetReg(&hi2c1, nHProtCfg2Reg, data);
	data[0] = 0x61; //LSB
	data[1] = 0x1C; //MSB
	nsetReg(&hi2c1, nBalThReg, data);
	data[0] = 0x10; //LSB
	data[1] = 0x04; //MSB
	nsetReg(&hi2c1, nProtCfgReg, data);
	data[0] = 0x00; //LSB
	data[1] = 0x00; //MSB
	nsetReg(&hi2c1, nBattStatusReg, data);
	data[0] = 0x00; //LSB
	data[1] = 0x00; //MSB
	nsetReg(&hi2c1, nFaultLogReg, data);

	for (int var = 0; var < 10; ++var) {

		getRepCap(&hi2c1);
		HAL_Delay(10);
		getRepSoC(&hi2c1);
		HAL_Delay(10);
		getTTE(&hi2c1);
		HAL_Delay(10);
		getTTF(&hi2c1);
		HAL_Delay(10);
		getVC1(&hi2c1);
		HAL_Delay(10);
		getVC2(&hi2c1);
		HAL_Delay(10);
		getVC3(&hi2c1);
		HAL_Delay(10);
		getVC4(&hi2c1);
		HAL_Delay(10);
		getVT(&hi2c1);
		HAL_Delay(10);
		getCurrent(&hi2c1);
		HAL_Delay(10);
		getTemp1(&hi2c1);
		HAL_Delay(10);
		getTemp2(&hi2c1);
		HAL_Delay(10);
		getTemp3(&hi2c1);
		HAL_Delay(10);
		getTemp4(&hi2c1);
		HAL_Delay(10);
		getAvgDieTemp(&hi2c1);
		HAL_Delay(10);
		getStatus(&hi2c1);
		HAL_Delay(10);
		getStatus2(&hi2c1);
		HAL_Delay(10);
		getProtStatus(&hi2c1);
		HAL_Delay(10);
		getProtAlrtStatus(&hi2c1);
		HAL_Delay(10);
		getCommStatus(&hi2c1);
		HAL_Delay(10);

	}
	RGBTurnOff();
}
/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_DMA_Init();
	MX_I2C1_Init();
	MX_USART1_UART_Init();
	MX_TIM1_Init();
	/* USER CODE BEGIN 2 */

	//HAL_TIM_Base_Start_IT(&htim1);
	HAL_I2C_Init(&hi2c1);
	//HAL_UART_Receive_IT(&huart1, UART_RX_buffer, 10);
	__HAL_UART_FLUSH_DRREGISTER(&huart1);
	HAL_UART_Receive_DMA(&huart1, &rxByte, 1);
	//MAX17320Init(&hi2c1);

	MAX17320Config();

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
		/*HAL_Delay(1000);
		 HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, 0);
		 HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, 1);
		 HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, 1);
		 HAL_Delay(1000);
		 HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, 1);
		 HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, 0);
		 HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, 1);
		 HAL_Delay(1000);
		 HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, 1);
		 HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, 1);
		 HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, 0);
		 */
		//HAL_Delay(1000);
		/*		uint8_t input[8];
		 uint8_t batt[2];
		 uint8_t status[2];
		 uint8_t status2[2];
		 uint8_t ProtStatus[2];
		 uint8_t ProtAlrt[2];
		 uint8_t nRSense[2];

		 uint8_t CommStat[2];

		 uint8_t lock[2];
		 uint8_t nBattStatus[2];
		 uint8_t nHProtCfg2[2];
		 uint8_t nTAlrtTh[2];
		 uint8_t nProtCfg[2];
		 uint8_t nFaultLog[2];

		 uint8_t standaardReg[] = { StatusReg, RepCapReg, RepSOCReg, VCellReg,
		 TempReg, CurrentReg,
		 TTEReg, TTFReg, ProtStatusReg, ProtAlrtReg };
		 uint8_t standaardRegOut[10][2];

		 //clear Status
		 setReg(&hi2c1, StatusReg, 0x0000);
		 //clear ProtStatus
		 setReg(&hi2c1, ProtStatusReg, 0x0000);
		 //clear ProtAlrt
		 setReg(&hi2c1, ProtAlrtReg, 0x0000);
		 //clear commStat
		 //setReg(&hi2c1, CommStatReg, 0x0000);

		 for (int var1 = 0; var1 < 10; var1++) {
		 HAL_I2C_Mem_Read(&hi2c1, 0x6C, standaardReg[var1], 1,
		 standaardRegOut[var1], 2, 100);
		 }*/

		//char buffer[16];
		//HAL_I2C_Master_Transmit(&hi2c1, 0x6C, 0xE2FB, sizeof(0xE2FB), 100);
		//HAL_Delay(5);
		//HAL_I2C_Mem_Read(&hi2c1, 0x16, 0x35, 1, input, 2, 100); //werkt!
		//HAL_I2C_Mem_Read(&hi2c1, 0x6C, 0x1C, 1, &input, 2, 100); //read current
		/*
		 data[0] = 0x0A; //LSB
		 data[1] = 0x22; //MSB
		 uint8_t uit;
		 //uit = nsetReg(&hi2c1, nPackCfgReg, data);
		 nsetRegMulti(&hi2c1, addressReg, dataReg, 2);
		 data[0] = 0xC8; //LSB
		 data[1] = 0x00; //MSB
		 nsetReg(&hi2c1, nRSenseReg, data);
		 */

		/*		HAL_I2C_Mem_Read(&hi2c1, 0x16, nPackCfgReg, 1, nPackCfg, 2, 100);
		 HAL_I2C_Mem_Read(&hi2c1, 0x16, nRSenseReg, 1, nRSense, 2, 100);
		 HAL_I2C_Mem_Read(&hi2c1, 0x16, 0xF1, 1, nHProtCfg2, 2, 100);
		 HAL_I2C_Mem_Read(&hi2c1, 0x16, nProtCfgReg, 1, nProtCfg, 2, 100);
		 HAL_I2C_Mem_Read(&hi2c1, 0x16, nFaultLogReg, 1, nFaultLog, 2, 100);

		 HAL_I2C_Mem_Read(&hi2c1, 0x6C, 0x61, 1, CommStat, 2, 100);

		 HAL_I2C_Mem_Read(&hi2c1, 0x6C, 0x7F, 1, lock, 2, 100); // read format of lock register
		 HAL_Delay(10);

		 HAL_I2C_Mem_Read(&hi2c1, 0x16, nTAlrtThReg, 1, nTAlrtTh, 2, 100);*/
		//HAL_I2C_Mem_Read(&hi2c1, 0x6C, 0xB0, 1, input, 2, 100);//read status2 register
		//input[0] = 0x08; //LSB
		//input[1] = 0x00; //MSB
		//HAL_I2C_Mem_Write(&hi2c1, 0x16, 0xF1, 1, input, 2, 100);
		//HAL_I2C_Mem_Read(&hi2c1, 0x16, 0xF1, 1, input, 2, 100);
		/*
		 HAL_I2C_Mem_Read(&hi2c1, 0x6C, 0x00, 1, status, 2, 100); //read status register
		 HAL_Delay(10);
		 HAL_I2C_Mem_Read(&hi2c1, 0x6C, 0xB0, 1, status2, 2, 100); //read status2 register
		 HAL_Delay(10);
		 HAL_I2C_Mem_Read(&hi2c1, 0x6C, 0xD9, 1, ProtStatus, 2, 100); //read ProtStatus register
		 HAL_Delay(10);
		 HAL_I2C_Mem_Read(&hi2c1, 0x6C, 0xAF, 1, ProtAlrt, 2, 100); //read ProtAlrt register
		 */

		/*
		 * ProtAlrt 0x1808
		 * p 92
		 * TooColdc, OVP
		 */

		/*		HAL_Delay(10);

		 HAL_I2C_Mem_Read(&hi2c1, 0x16, 0xA8, 1, nBattStatus, 2, 100); //read nBattStatus register
		 HAL_Delay(10);

		 HAL_I2C_Mem_Read(&hi2c1, 0x6C, 0xD5, 1, input, 8, 100); //read voltages
		 HAL_Delay(10);
		 HAL_I2C_Mem_Read(&hi2c1, 0x6C, 0xDA, 1, batt, 2, 100); //read voltages
		 HAL_Delay(10);

		 avgDieTemp = getAvgDieTemp(&hi2c1);
		 avgTemp1 = getTemp1(&hi2c1);
		 avgTemp2 = getTemp2(&hi2c1);
		 avgTemp3 = getTemp3(&hi2c1);
		 avgTemp4 = getTemp4(&hi2c1);
		 */
		//VC1 = getVC1(&hi2c1);
		//char buffer2[] = "hello world! I'm alive!\r\n";
		//printString("Hello...\r\n");
		//HAL_UART_Transmit(&huart1, "\r\n", sizeof("\r\n"), 100);
		//HAL_UART_Transmit(&huart1, buffer2, sizeof(buffer2), 500);
		//HAL_UART_Transmit(&huart1, "\r\n", sizeof("\r\n"), 100);
		//HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "%d", input[0]), 500);
		//HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "%d", (uint8_t)VC1), 500);
		//char buffer3[] = ".";
		//HAL_UART_Transmit(&huart1, buffer3, sizeof(buffer3), 500);
		//HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "%d", (int)((VC1*10000)-(((uint8_t)VC1)*10000))), 500);
		//printString(&buffer2);
		/*printFloat(getVC1(&hi2c1));
		 printFloat(getVC2(&hi2c1));
		 printFloat(getVC3(&hi2c1));
		 printFloat(getVC4(&hi2c1));
		 printFloat(getVT(&hi2c1));
		 printFloat(getCurrent(&hi2c1));
		 //float current =getCurrent(&hi2c1);
		 //print2scompliment(current);
		 //printBinair(current);
		 printFloat(avgTemp1);
		 printFloat(avgTemp2);
		 printFloat(avgTemp3);
		 printFloat(avgTemp4);
		 printHex(getStatus(&hi2c1));
		 */
		uint8_t data1[2];
		if (rxFlag == 1) {
			RGBTurnRED();
			rxFlag = 0;

			printlnString(rxBuffer);

			if (rxBuffer[0] == 'A' && rxBuffer[1] == 'T'
					&& rxBuffer[2] == '+') {
				str_cut(rxBuffer, 0, 3); //cut AT+ off
				//printlnString(rxBuffer);

				switch (stringToInt(rxBuffer)) {
				case 'O' << 24 | 'K' << 16 | '?' << 8 | 0:
					printlnString("OK");
					break;
				case 'O' << 24 | 'V' << 16 | '?' << 8 | 0:
					printlnString("Overview:\r\n");

					printlnString("Reported remaining Capacity: [Ah]");
					printFloat(getRepCap(&hi2c1));
					printlnString("Reported state of charge: [%]");
					printFloat(getRepSoC(&hi2c1));
					printlnString("Time to empty: [s]");
					printFloat(getTTE(&hi2c1));
					printlnString("Time to full: [s]");
					printFloat(getTTF(&hi2c1));
					printlnString("Cell voltage 1: [V]");
					printFloat(getVC1(&hi2c1));
					printlnString("Cell voltage 2: [V]");
					printFloat(getVC2(&hi2c1));
					printlnString("Cell voltage 3: [V]");
					printFloat(getVC3(&hi2c1));
					printlnString("Cell voltage 4: [V]");
					printFloat(getVC4(&hi2c1));
					printlnString("Cell voltage of total pack: [V]");
					printFloat(getVT(&hi2c1));
					printlnString("Current from system to battery: [A]");
					printFloat(getCurrent(&hi2c1));
					printlnString("Temperature 1: [°C]");
					printFloat(getTemp1(&hi2c1));
					printlnString("Temperature 2: [°C]");
					printFloat(getTemp2(&hi2c1));
					printlnString("Temperature 3: [°C]");
					printFloat(getTemp3(&hi2c1));
					printlnString("Temperature 4: [°C]");
					printFloat(getTemp4(&hi2c1));
					printlnString("Die temperature: [°C]");
					printFloat(getAvgDieTemp(&hi2c1));
					printlnString("Status register: [Hex]");
					printHex(getStatus(&hi2c1));
					printlnString("Status2 register: [Hex]");
					printHex(getStatus2(&hi2c1));
					printlnString("ProtStatus register: [Hex]");
					printHex(getProtStatus(&hi2c1));
					printlnString("ProtAlrt register: [Hex]");
					printHex(getProtAlrtStatus(&hi2c1));
					printlnString("CommStatus register: [Hex]");
					printHex(getCommStatus(&hi2c1));

					break;
				case 'E' << 24 | 'N' << 16 | 'P' << 8 | 'B':
					//DO TO enable passive balancing
					printlnString("ENPB OK");
					break;
				case 'D' << 24 | 'S' << 16 | 'P' << 8 | 'B':
					//DO TO Disable passive balancing
					printlnString("DSPB OK");
					break;
				case 'R' << 24 | 'C' << 16 | '?' << 8 | 0:
					printString("RC: ");
					printFloat(getRepCap(&hi2c1));
					break;
				case 'R' << 24 | 'S' << 16 | 'C' << 8 | '?':
					printString("RSC: ");
					printFloat(getRepSoC(&hi2c1));
					break;
				case 'T' << 24 | 'T' << 16 | 'E' << 8 | '?':
					printString("TTE: ");
					printFloat(getTTE(&hi2c1));
					break;
				case 'T' << 24 | 'T' << 16 | 'F' << 8 | '?':
					printString("TTF: ");
					printFloat(getTTF(&hi2c1));
					break;
				case 'C' << 24 | 'V' << 16 | '1' << 8 | '?':
					printString("CV1: ");
					printFloat(getVC1(&hi2c1));
					break;
				case 'C' << 24 | 'V' << 16 | '2' << 8 | '?':
					printString("CV2: ");
					printFloat(getVC2(&hi2c1));
					break;
				case 'C' << 24 | 'V' << 16 | '3' << 8 | '?':
					printString("CV3: ");
					printFloat(getVC3(&hi2c1));
					break;
				case 'C' << 24 | 'V' << 16 | '4' << 8 | '?':
					printString("CV4: ");
					printFloat(getVC4(&hi2c1));
					break;
				case 'C' << 24 | 'V' << 16 | 'T' << 8 | '?':
					printString("CVT: ");
					printFloat(getVT(&hi2c1));
					break;
				case 'P' << 24 | 'C' << 16 | '?' << 8 | 0:
					printString("PC: ");
					printFloat(getCurrent(&hi2c1));
					break;
				case 'T' << 24 | 'H' << 16 | '1' << 8 | '?':
					printString("TH1: ");
					printFloat(getTemp1(&hi2c1));
					break;
				case 'T' << 24 | 'H' << 16 | '2' << 8 | '?':
					printString("TH2: ");
					printFloat(getTemp2(&hi2c1));
					break;
				case 'T' << 24 | 'H' << 16 | '3' << 8 | '?':
					printString("TH3: ");
					printFloat(getTemp3(&hi2c1));
					break;
				case 'T' << 24 | 'H' << 16 | '4' << 8 | '?':
					printString("TH4: ");
					printFloat(getTemp4(&hi2c1));
					break;
				case 'T' << 24 | 'H' << 16 | 'D' << 8 | '?':
					printString("THD: ");
					printFloat(getAvgDieTemp(&hi2c1));
					break;
				case 'S' << 24 | 'T' << 16 | '1' << 8 | '?':
					printString("ST1: ");
					printHex(getStatus(&hi2c1));
					break;
				case 'S' << 24 | 'T' << 16 | '2' << 8 | '?':
					printString("ST2: ");
					printHex(getStatus2(&hi2c1));
					break;
				case 'S' << 24 | 'T' << 16 | '3' << 8 | '?':
					printString("ST3: ");
					printHex(getProtStatus(&hi2c1));
					break;
				case 'S' << 24 | 'T' << 16 | '4' << 8 | '?':
					printString("ST4: ");
					printHex(getProtAlrtStatus(&hi2c1));
					break;
				case 'S' << 24 | 'T' << 16 | '5' << 8 | '?':
					printString("ST5: ");
					printHex(getCommStatus(&hi2c1));
					break;
				case 'R' << 24 | 'S' << 16 | 'T' << 8 | 0:
					printString("RESET ");
					RGBTurnBlue();

					if (FullReset(&hi2c1) == 0) {
						RGBTurnOff();
						printlnString("OK");
					} else
						printlnString("NOK");

					break;
				case 'I' << 24 | 'N' << 16 | 'T' << 8 | 0:
					printString("INT ");
					RGBTurnBlue();
					MAX17320Config();
					printlnString("OK");
					RGBTurnOff();

					break;

					/*				case 'F' << 24 | 'T' << 16 | 'O' << 8 | 'F':
					 printlnString("FTOF");
					 //commStat

					 data1[0] = 0x00; //LSB
					 data1[1] = 0x00; //MSB
					 setReg(&hi2c1, CommStatReg, data1);
					 setReg(&hi2c1, CommStatReg, data1);
					 data1[0] = 0x00; //LSB
					 data1[1] = 0x03; //MSB
					 setReg(&hi2c1, CommStatReg, data1);
					 data1[0] = 0xF9; //LSB
					 data1[1] = 0x03; //MSB
					 setReg(&hi2c1, CommStatReg, data1);
					 setReg(&hi2c1, CommStatReg, data1);
					 break;
					 case 'F' << 24 | 'T' << 16 | 'O' << 8 | 'N':
					 printlnString("FTON");
					 //data1[2];
					 data1[0] = 0x00; //LSB
					 data1[1] = 0x00; //MSB
					 setReg(&hi2c1, CommStatReg, data1);
					 setReg(&hi2c1, CommStatReg, data1);
					 data1[0] = 0x0F9; //LSB
					 data1[1] = 0x00; //MSB
					 setReg(&hi2c1, CommStatReg, data1);
					 data1[0] = 0xF9; //LSB
					 data1[1] = 0x00; //MSB
					 setReg(&hi2c1, CommStatReg, data1);
					 setReg(&hi2c1, CommStatReg, data1);
					 break;*/
				default:
					printAT_Comand_Error(AT_WRONG_COMMAND);
					break;
				}

			} else
				printAT_Comand_Error(AT_WRONG_COMMAND);

			for (int var = 0; var < RxBufferLength; var++) {
				rxBuffer[var] = 0;
			}
			//HAL_Delay(1000);
			RGBTurnOff();
		}
		/*
		 * char in[8];
		 HAL_UART_Receive(&huart1, (uint8_t *)in, 8, 1000);

		 HAL_UART_Transmit(&huart1, "\r\n", sizeof("\r\n"), 100);

		 HAL_UART_Transmit(&huart1, (uint8_t *)in, 8, 1);
		 HAL_UART_Transmit(&huart1, "\r\n", sizeof("\r\n"), 100);*/

		//char buffer[] = "test2\r\n";
		// HAL_UART_Transmit(&huart1, buffer, sizeof(buffer), 100);
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}
	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK) {
		Error_Handler();
	}
}

/**
 * @brief I2C1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_I2C1_Init(void) {

	/* USER CODE BEGIN I2C1_Init 0 */

	/* USER CODE END I2C1_Init 0 */

	/* USER CODE BEGIN I2C1_Init 1 */

	/* USER CODE END I2C1_Init 1 */
	hi2c1.Instance = I2C1;
	hi2c1.Init.ClockSpeed = 100000;
	hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
	hi2c1.Init.OwnAddress1 = 0;
	hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c1.Init.OwnAddress2 = 0;
	hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	if (HAL_I2C_Init(&hi2c1) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN I2C1_Init 2 */

	/* USER CODE END I2C1_Init 2 */

}

/**
 * @brief TIM1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM1_Init(void) {

	/* USER CODE BEGIN TIM1_Init 0 */

	/* USER CODE END TIM1_Init 0 */

	TIM_ClockConfigTypeDef sClockSourceConfig = { 0 };
	TIM_MasterConfigTypeDef sMasterConfig = { 0 };
	TIM_OC_InitTypeDef sConfigOC = { 0 };
	TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = { 0 };

	/* USER CODE BEGIN TIM1_Init 1 */

	/* USER CODE END TIM1_Init 1 */
	htim1.Instance = TIM1;
	htim1.Init.Prescaler = 4;
	htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim1.Init.Period = 65535;
	htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim1.Init.RepetitionCounter = 0;
	htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	if (HAL_TIM_Base_Init(&htim1) != HAL_OK) {
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK) {
		Error_Handler();
	}
	if (HAL_TIM_OC_Init(&htim1) != HAL_OK) {
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig)
			!= HAL_OK) {
		Error_Handler();
	}
	sConfigOC.OCMode = TIM_OCMODE_TIMING;
	sConfigOC.Pulse = 0;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
	sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
	if (HAL_TIM_OC_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK) {
		Error_Handler();
	}
	sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
	sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
	sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
	sBreakDeadTimeConfig.DeadTime = 0;
	sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
	sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
	sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
	if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig)
			!= HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN TIM1_Init 2 */

	/* USER CODE END TIM1_Init 2 */

}

/**
 * @brief USART1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART1_UART_Init(void) {

	/* USER CODE BEGIN USART1_Init 0 */

	/* USER CODE END USART1_Init 0 */

	/* USER CODE BEGIN USART1_Init 1 */

	/* USER CODE END USART1_Init 1 */
	huart1.Instance = USART1;
	huart1.Init.BaudRate = 115200;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = UART_PARITY_NONE;
	huart1.Init.Mode = UART_MODE_TX_RX;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart1) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN USART1_Init 2 */

	/* USER CODE END USART1_Init 2 */

}

/**
 * Enable DMA controller clock
 */
static void MX_DMA_Init(void) {

	/* DMA controller clock enable */
	__HAL_RCC_DMA1_CLK_ENABLE();

	/* DMA interrupt init */
	/* DMA1_Channel5_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);

}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOB, Blue_LED_Pin | Green_LED_Pin | Red_LED_Pin,
			GPIO_PIN_SET);

	/*Configure GPIO pins : Blue_LED_Pin Green_LED_Pin Red_LED_Pin */
	GPIO_InitStruct.Pin = Blue_LED_Pin | Green_LED_Pin | Red_LED_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	HAL_GPIO_WritePin(LED_port, Red_LED_pin, 0); //turn Red LED on
	HAL_GPIO_WritePin(LED_port, Green_LED_pin, 1); //turn Green LED off
	HAL_GPIO_WritePin(LED_port, Blue_LED_pin, 1); // turn Blue LED off
	/* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
