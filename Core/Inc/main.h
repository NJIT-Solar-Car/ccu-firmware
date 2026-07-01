/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED_Heartbeat_Pin GPIO_PIN_0
#define LED_Heartbeat_GPIO_Port GPIOC
#define LED_User1_Pin GPIO_PIN_1
#define LED_User1_GPIO_Port GPIOC
#define relay1_Pin GPIO_PIN_4
#define relay1_GPIO_Port GPIOA
#define relay2_Pin GPIO_PIN_5
#define relay2_GPIO_Port GPIOA
#define relay3_Pin GPIO_PIN_6
#define relay3_GPIO_Port GPIOA
#define relay4_Pin GPIO_PIN_7
#define relay4_GPIO_Port GPIOA
#define relay5_Pin GPIO_PIN_4
#define relay5_GPIO_Port GPIOC
#define relay6_Pin GPIO_PIN_5
#define relay6_GPIO_Port GPIOC
#define relay7_Pin GPIO_PIN_0
#define relay7_GPIO_Port GPIOB
#define CAN2_FLT_Pin GPIO_PIN_14
#define CAN2_FLT_GPIO_Port GPIOB
#define GPIO4_Pin GPIO_PIN_15
#define GPIO4_GPIO_Port GPIOB
#define GPIO3_Pin GPIO_PIN_8
#define GPIO3_GPIO_Port GPIOD
#define GPIO2_Pin GPIO_PIN_9
#define GPIO2_GPIO_Port GPIOD
#define GPIO1_Pin GPIO_PIN_10
#define GPIO1_GPIO_Port GPIOD
#define GPIO0_Pin GPIO_PIN_11
#define GPIO0_GPIO_Port GPIOD
#define HC6_Pin GPIO_PIN_12
#define HC6_GPIO_Port GPIOD
#define HC5_Pin GPIO_PIN_13
#define HC5_GPIO_Port GPIOD
#define HC4_Pin GPIO_PIN_14
#define HC4_GPIO_Port GPIOD
#define HC3_Pin GPIO_PIN_15
#define HC3_GPIO_Port GPIOD
#define HC2_Pin GPIO_PIN_6
#define HC2_GPIO_Port GPIOC
#define HC1_Pin GPIO_PIN_7
#define HC1_GPIO_Port GPIOC
#define HC0_Pin GPIO_PIN_8
#define HC0_GPIO_Port GPIOC
#define GPIO17_Pin GPIO_PIN_15
#define GPIO17_GPIO_Port GPIOA
#define GPIO16_Pin GPIO_PIN_10
#define GPIO16_GPIO_Port GPIOC
#define GPIO15_Pin GPIO_PIN_11
#define GPIO15_GPIO_Port GPIOC
#define GPIO14_Pin GPIO_PIN_12
#define GPIO14_GPIO_Port GPIOC
#define CAN1_FLT_Pin GPIO_PIN_2
#define CAN1_FLT_GPIO_Port GPIOD
#define GPIO13_Pin GPIO_PIN_3
#define GPIO13_GPIO_Port GPIOD
#define GPIO12_Pin GPIO_PIN_4
#define GPIO12_GPIO_Port GPIOD
#define GPIO11_Pin GPIO_PIN_5
#define GPIO11_GPIO_Port GPIOD
#define GPIO10_Pin GPIO_PIN_6
#define GPIO10_GPIO_Port GPIOD
#define GPIO9_Pin GPIO_PIN_7
#define GPIO9_GPIO_Port GPIOD
#define BC_nCE_Pin GPIO_PIN_4
#define BC_nCE_GPIO_Port GPIOB
#define BC_nQON_Pin GPIO_PIN_5
#define BC_nQON_GPIO_Port GPIOB
#define BC_INT_Pin GPIO_PIN_8
#define BC_INT_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/*This code is technically not accurate and needs to be checked and corrected by the wiring team once
their task is complete. When it does get checked, all that needs to be done is switch the relay # pins
to its correct function.*/

//Ports and Pins
#define Neg_Main_Port GPIOA
#define Neg_Main_Pin  relay1_Pin

#define Motor_Relays_Port  GPIOA
#define Motor_Relays_Pin   (relay2_Pin | relay3_Pin)

#define Pos_Main_Port GPIOA
#define Pos_Main_Pin  relay4_Pin

#define Charge_Relay_Port GPIOC
#define Charge_Relay_Pin  relay5_Pin

#define LV_Relay_Port GPIOC
#define LV_Relay_Pin  relay6_Pin

#define Supp_Battery_Port GPIOB
#define Supp_Battery_Pin  relay7_Pin

#define COOLING_FAN_PORT HC0_GPIO_Port
#define COOLING_FAN_PIN  HC0_Pin

/*Need to know what the start button and estop button is routed to*/

//On/Off Macros
#define Neg_Main_OFF() HAL_GPIO_WritePin(Neg_Main_Port, Neg_Main_Pin, GPIO_PIN_RESET)
#define Neg_Main_ON()  HAL_GPIO_WritePin(Neg_Main_Port, Neg_Main_Pin, GPIO_PIN_SET)

#define Motor_Relays_OFF()  HAL_GPIO_WritePin(Motor_Relays_Port, Motor_Relays_Pin, GPIO_PIN_RESET)
#define Motor_Relays_ON()   HAL_GPIO_WritePin(Motor_Relays_Port, Motor_Relays_Pin, GPIO_PIN_SET)

#define Pos_Main_OFF() HAL_GPIO_WritePin(Pos_Main_Port, Pos_Main_Pin, GPIO_PIN_RESET)
#define Pos_Main_ON()  HAL_GPIO_WritePin(Pos_Main_Port, Pos_Main_Pin, GPIO_PIN_SET)

#define Charge_Relay_OFF() HAL_GPIO_WritePin(Charge_Relay_Port, Charge_Relay_Pin, GPIO_PIN_RESET)
#define Charge_Relay_ON()  HAL_GPIO_WritePin(Charge_Relay_Port, Charge_Relay_Pin, GPIO_PIN_SET)

#define LV_Relay_OFF() HAL_GPIO_WritePin(LV_Relay_Port, LV_Relay_Pin, GPIO_PIN_RESET)
#define LV_Relay_ON()  HAL_GPIO_WritePin(LV_Relay_Port, LV_Relay_Pin, GPIO_PIN_SET)

#define Supp_Battery_OFF() HAL_GPIO_WritePin(Supp_Battery_Port, Supp_Battery_Pin, GPIO_PIN_RESET)
#define Supp_Battery_ON()  HAL_GPIO_WritePin(Supp_Battery_Port, Supp_Battery_Pin, GPIO_PIN_SET)

#define LED_Heartbeat_OFF() HAL_GPIO_WritePin(LED_Heartbeat_GPIO_Port, LED_Heartbeat_Pin, GPIO_PIN_RESET)
#define LED_Heartbeat_ON() HAL_GPIO_WritePin(LED_Heartbeat_GPIO_Port, LED_Heartbeat_Pin, GPIO_PIN_SET)
#define LED_Heartbeat_TOGGLE() HAL_GPIO_TogglePin(LED_Heartbeat_GPIO_Port, LED_Heartbeat_Pin)

#define Cooling_Fan_ON()  HAL_GPIO_WritePin(COOLING_FAN_PORT, COOLING_FAN_PIN, GPIO_PIN_SET)
#define Cooling_Fan_OFF() HAL_GPIO_WritePin(COOLING_FAN_PORT, COOLING_FAN_PIN, GPIO_PIN_RESET)


extern uint8_t ReadyFlag = 0;

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
