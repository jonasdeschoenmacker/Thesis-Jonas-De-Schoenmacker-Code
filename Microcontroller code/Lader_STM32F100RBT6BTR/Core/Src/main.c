/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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
#include <stdio.h>
#include "AT_Comands_variables.h"
#include "string.h"
#include "BQ25713.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define RxBufferLength 	24
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c2;

UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_rx;

/* USER CODE BEGIN PV */
uint8_t rxBuffer[RxBufferLength];
uint8_t rxByte;
uint16_t rxIdx;
uint8_t rxFlag;
uint8_t data_input[2];
uint8_t current[2];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_I2C2_Init(void);
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
void BQ25713Config() {
	//boot up time
	RGBTurnBlue();
	HAL_Delay(1000); //wait 1000ms to make sure IC is ready

	//data_input[0] = 0x00;
	//data_input[1] = 0x00;
	//setRegBQ25713(&hi2c1, InputVoltageReg, data_input);

	//data_input[0] = 0x08;
	//data_input[1] = 0x75;
	data_input[0] = 0x00;
	data_input[1] = 0x16;
	setRegBQ25713(&hi2c1, ChargeOption0Reg, data_input);

	data_input[0] = 0x19;
	data_input[1] = 0x02;
	setRegBQ25713(&hi2c1, ChargeOption1Reg, data_input);

	//data_input[0] = 0x34;
	//data_input[1] = 0x02;
	data_input[0] = 0x05;
	data_input[1] = 0x02;
	setRegBQ25713(&hi2c1, ChargeOption2Reg, data_input);

	//data_input[0] = 0x38;
	//data_input[1] = 0x28;
	data_input[0] = 0x30;
	data_input[1] = 0x20;
	setRegBQ25713(&hi2c1, ChargeOption3Reg, data_input);

	//data_input[0] = 0x65;
	//data_input[1] = 0xFA;
	//setRegBQ25713(&hi2c1, ProchotOption0Reg, data_input);

	//data_input[0] = 0x7F;
	//data_input[1] = 0xA0;
	//setRegBQ25713(&hi2c1, ADCOptionReg, data_input);

	//data_input[0] = 0xC0;
	//data_input[1] = 0x1F;
	//setRegBQ25713(&hi2c1, IIN_DPMReg, data_input);

	data_input[0] = 0xFF;
	data_input[1] = 0x7F;
	setRegBQ25713(&hi2c1, IIN_HOSTReg, data_input);

	//data_input[0] = 0x40;
	//data_input[1] = 0x00;
	//setRegBQ25713(&hi2c1, ChargeCurrentReg, data_input);

	//HAL_I2C_Mem_Write(&hi2c1, BQ25713Address, ChargeCurrentReg, 1, data_input,2, 100);

	//HAL_I2C_DeInit(&hi2c1);
	//HAL_I2C_Init(&hi2c2);

	//set CELL_BATPRESZ_Pin -> auto disable IDPM
	HAL_GPIO_WritePin(CELL_BATPRESZ_GPIO_Port, CELL_BATPRESZ_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(OTG_VAP_GPIO_Port, OTG_VAP_Pin, GPIO_PIN_RESET);

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
	MX_I2C2_Init();
	/* USER CODE BEGIN 2 */
	HAL_I2C_Init(&hi2c1);

	__HAL_UART_FLUSH_DRREGISTER(&huart1);
	HAL_UART_Receive_DMA(&huart1, &rxByte, 1);

	BQ25713Config();
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
		/* uint8_t start[10];
		 HAL_I2C_Mem_Read(&hi2c2, 0x50, 0x0D, 1, start, 10, 100);
		 HAL_Delay(100);

		 uint8_t Device_ID[2];
		 HAL_I2C_Mem_Read(&hi2c2, 0x50, 0x2F, 1, Device_ID, 1, 100);
		 HAL_Delay(100);*/
		/*
		 uint8_t data_in[2];
		 data_in[0] = 0x01;


		 HAL_I2C_Mem_Read(&hi2c1, 0xD6, 0x0E, 1, data_in, 2, 100);
		 HAL_Delay(100);
		 uint8_t ChargeOption0[2];
		 HAL_I2C_Mem_Read(&hi2c1, 0xD6, 0x00, 1, ChargeOption0, 2, 100);
		 HAL_Delay(100);
		 uint8_t ChargeCurrent[2];
		 HAL_I2C_Mem_Read(&hi2c1, 0xD6, 0x02, 1, ChargeCurrent, 2, 100);
		 HAL_Delay(100);
		 uint8_t ChargeVoltage[2];
		 HAL_I2C_Mem_Read(&hi2c1, 0xD6, 0x04, 1, ChargeVoltage, 2, 100);
		 HAL_Delay(100);

		 //HAL_I2C_Mem_Write(&hi2c1, 0xD6, 0x02, 1, data_input, 2, 100);
		 HAL_I2C_Mem_Read(&hi2c1, 0xD6, 0x02, 1, ChargeCurrent, 2, 100);
		 HAL_Delay(100);
		 */
		//setRegBQ25713(&hi2c1, 0x0F, data_input);
		//	  HAL_I2C_Mem_Write(&hi2c1, 0xD6, 0x0E, 1, data_input, 2, 100);
		RGBTurnRED();
		HAL_Delay(500);
		RGBTurnOff();
		HAL_Delay(500);

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

					printlnString("ChargeOption0: [Hex]");
					printHex(getChargeOption0(&hi2c1));
					printlnString("ChargeCurrent: [Hex]");
					printHex(getChargeCurrent(&hi2c1));
					printlnString("MaxChargeVoltage: [Hex]");
					printHex(getMaxChargeVoltage(&hi2c1));
					printlnString("ChargeOption1: [Hex]");
					printHex(getChargeOption1(&hi2c1));
					printlnString("ChargeOption2: [Hex]");
					printHex(getChargeOption2(&hi2c1));
					printlnString("ChargeOption3: [Hex]");
					printHex(getChargeOption3(&hi2c1));
					printlnString("ProchotOption0: [Hex]");
					printHex(getProchotOption0(&hi2c1));
					printlnString("ProchotOption1: [Hex]");
					printHex(getProchotOption1(&hi2c1));
					printlnString("ADCOption: [Hex]");
					printHex(getADCOption(&hi2c1));
					printlnString("ChargerStatus: [Hex]");
					printHex(getChargerStatus(&hi2c1));
					printlnString("ProchotStatus: [Hex]");
					printHex(getProchotStatus(&hi2c1));
					printlnString("IIN_DPM: [Hex]");
					printHex(getIIN_DPM(&hi2c1));
					printlnString("ADC_VBUS_PSYS: [Hex]");
					printHex(getADC_VBUS_PSYS(&hi2c1));
					printlnString("ADC_IBAT: [Hex]");
					printHex(getADC_IBAT(&hi2c1));
					printlnString("ADC_IINCMPIN: [Hex]");
					printHex(getADC_IINCMPIN(&hi2c1));
					printlnString("ADC_VSYS_VBAT: [Hex]");
					printHex(getADC_VSYS_VBAT(&hi2c1));
					printlnString("OTGVoltage: [Hex]");
					printHex(getOTGVoltage(&hi2c1));
					printlnString("OTGCurrent: [Hex]");
					printHex(getOTGCurrent(&hi2c1));
					printlnString("InputVoltage: [Hex]");
					printHex(getInputVoltage(&hi2c1));
					printlnString("MinSystemVoltage: [Hex]");
					printHex(getMinSystemVoltage(&hi2c1));
					printlnString("IIN_HOST: [Hex]");
					printHex(getIIN_HOST(&hi2c1));

					break;
				case 'R' << 24 | 'S' << 16 | 'I' << 8 | 'V':
					data_input[0] = 0x00;
					data_input[1] = 0x06; //zet dit ies wa hoger dan 3.2V (bij 00h) 06h -> 3.2V+1,024+0,512 = 4,736V
					setRegBQ25713(&hi2c1, InputVoltageReg, data_input);
					printlnString("RSIV");
					break;
				case 'R' << 24 | 'S' << 16 | 'M' << 8 | 'V':
					data_input[0] = 0x80;
					data_input[1] = 0x41; // set voltage to 16,768V
					setRegBQ25713(&hi2c1, MaxChargeVoltageReg, data_input);
					printlnString("RSMV");
					break;

				case 'I' << 24 | 'N' << 16 | 'T' << 8 | 0:
					BQ25713Config();
					printlnString("INT");
					break;
				case 'R' << 24 | 'S' << 16 | 'T' << 8 | 'V':
					data_input[0] = 0x30;
					data_input[1] = 0x20;
					setRegBQ25713(&hi2c1, ChargeOption3Reg, data_input);
					printlnString("RSTV");
					break;
				case 'R' << 24 | 'S' << 16 | 'T' << 8 | 'P':
					data_input[0] = 0x00;
					data_input[1] = 0x20;
					setRegBQ25713(&hi2c1, ProchotStatusReg, data_input);
					printlnString("RSTP");
					break;
				case 'C' << 24 | 'U' << 16 | 'R' << 8 | '0':

					current[0] = 0x00;
					current[1] = 0x00;
					setRegBQ25713(&hi2c1, ChargeCurrentReg, current);
					printlnString("CUR=0: OK");
					break;
				case 'C' << 24 | 'U' << 16 | 'R' << 8 | '1':

					current[0] = 0x40;
					current[1] = 0x00;
					setRegBQ25713(&hi2c1, ChargeCurrentReg, current);
					printlnString("CUR=1: OK");
					break;
				case 'C' << 24 | 'U' << 16 | 'R' << 8 | '2':

					current[0] = 0x80;
					current[1] = 0x00;
					setRegBQ25713(&hi2c1, ChargeCurrentReg, current);
					printlnString("CUR=2: OK");
					break;
				case 'C' << 24 | 'U' << 16 | 'R' << 8 | '3':

					current[0] = 0x00;
					current[1] = 0x01;
					setRegBQ25713(&hi2c1, ChargeCurrentReg, current);
					printlnString("CUR=3: OK");
					break;
				case 'C' << 24 | 'U' << 16 | 'R' << 8 | '4':

					current[0] = 0x00;
					current[1] = 0x02;
					setRegBQ25713(&hi2c1, ChargeCurrentReg, current);
					printlnString("CUR=4: OK");
					break;
				case 'C' << 24 | 'U' << 16 | 'R' << 8 | '5':

					current[0] = 0x00;
					current[1] = 0x04;
					setRegBQ25713(&hi2c1, ChargeCurrentReg, current);
					printlnString("CUR=5: OK");
					break;
				case 'C' << 24 | 'U' << 16 | 'R' << 8 | '6':

					current[0] = 0x00;
					current[1] = 0x08;
					setRegBQ25713(&hi2c1, ChargeCurrentReg, current);
					printlnString("CUR=6: OK");
					break;
				case 'C' << 24 | 'U' << 16 | 'R' << 8 | '7':

					current[0] = 0x00;
					current[1] = 0x10;
					setRegBQ25713(&hi2c1, ChargeCurrentReg, current);
					printlnString("CUR=7: OK");
					break;
				case 'C' << 24 | 'U' << 16 | 'R' << 8 | '8':

					current[0] = 0x00;
					current[1] = 0x18;
					setRegBQ25713(&hi2c1, ChargeCurrentReg, current);
					printlnString("CUR=8: OK");
					break;
				case 'C' << 24 | 'U' << 16 | 'R' << 8 | '9':

					current[0] = 0x00;
					current[1] = 0x1C;
					setRegBQ25713(&hi2c1, ChargeCurrentReg, current);
					printlnString("CUR=9: OK");
					break;
				case 'C' << 24 | 'U' << 16 | 'R' << 8 | 'A':

					current[0] = 0xC0;
					current[1] = 0x1F;
					setRegBQ25713(&hi2c1, ChargeCurrentReg, current);
					printlnString("CUR=A: OK");
					break;

				default:
					printAT_Comand_Error(AT_WRONG_COMMAND);
					break;
				}

			} else
				printAT_Comand_Error(AT_WRONG_COMMAND);

			for (int var = 0; var < RxBufferLength; var++) {
				rxBuffer[var] = 0;
			}
			RGBTurnOff();
		}
		setRegBQ25713(&hi2c1, ChargeCurrentReg, current);
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
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
 * @brief I2C2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_I2C2_Init(void) {

	/* USER CODE BEGIN I2C2_Init 0 */

	/* USER CODE END I2C2_Init 0 */

	/* USER CODE BEGIN I2C2_Init 1 */

	/* USER CODE END I2C2_Init 1 */
	hi2c2.Instance = I2C2;
	hi2c2.Init.ClockSpeed = 100000;
	hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
	hi2c2.Init.OwnAddress1 = 0;
	hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c2.Init.OwnAddress2 = 0;
	hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	if (HAL_I2C_Init(&hi2c2) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN I2C2_Init 2 */

	/* USER CODE END I2C2_Init 2 */

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
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOC,
	CELL_BATPRESZ_Pin | Blue_LED_Pin | Green_LED_Pin | Red_LED_Pin,
			GPIO_PIN_SET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(OTG_VAP_GPIO_Port, OTG_VAP_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pins : CELL_BATPRESZ_Pin OTG_VAP_Pin Blue_LED_Pin Green_LED_Pin
	 Red_LED_Pin */
	GPIO_InitStruct.Pin = CELL_BATPRESZ_Pin | OTG_VAP_Pin | Blue_LED_Pin
			| Green_LED_Pin | Red_LED_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

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
	__disable_irq();
	while (1) {
	}
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
