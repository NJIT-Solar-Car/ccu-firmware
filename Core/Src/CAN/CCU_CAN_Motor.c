/**
 * @file CCU_CAN_Motor.c
 * @brief Holds function definitions for functions that interact with the motor controllers *specifically*. Mainly to populate
 * \ref MotorControllerInfo structs.
 *
 * @author Arnav Revankar
 */

#include "CAN/Inc/can_dictionary.h"
#include "CAN/Inc/can_motor.h"

#include "stm32f4xx_hal_can.h"

#include "FreeRTOS.h"
#include "task.h"

extern volatile TaskHandle_t xLeftMotorWaiting;
extern volatile TaskHandle_t xRightMotorWaiting;

extern uint8_t LeftMotorRxBuffer[8];
extern uint8_t RightMotorRxBuffer[8];

extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;

/* Motor controller state starts out at NONE (which is equivalent to IDLE in our case) */
CommandType LeftMotorState  = COMMAND_NONE;
CommandType RightMotorState = COMMAND_NONE;

HAL_StatusTypeDef A2D_BATCH_READ1_Func(MotorPacketInfo * pmpi) {
	/* Check if already run for current packet */
	if (pmpi->Command & A2D_BATCH_READ1 > 0) {
		return HAL_OK
	}

	/* Left motor */
	if (pmpi->ControllerInfo->ID == CAN_ID_MOTOR_LEFT_CMD) {
		/* Set motor state, and get task handle */
		LeftMotorState = A2D_BATCH_READ1;
		xLeftMotorWaiting = xTaskGetCurrentHandle();

		/* Prepare and send message */
		uint32_t 				assignedMailbox;
		CAN_TxHeaderTypeDef 	txHeader;
		HAL_StatusTypeDef 		txStatus;

		txHeader.StdId 	= (uint32_t)pmpi->ControllerInfo->ID;
		txHeader.ExtId 	= 0x00;
		txHeader.IDE 	= CAN_ID_STD;
		txHeader.RTR 	= CAN_RTR_DATA;
		txHeader.DLC 	= 8;
		txHeader.TransmitGlobalTime = DISABLE;

		uint8_t txData[] = {0x1b};

		txStatus = CCU_CAN_Tx(&hcan1, &txHeader, txData, &assignedMailbox, pdMS_TO_TICKS(CCU_MOTOR_TIMEOUT));

		/* Return any error that occurred when transmitting command */
		if (txStatus != HAL_OK) {
			xLeftMotorWaiting = NULL;
			return txStatus;
		}

		/* Make FreeRTOS block this task entirely. 
		 * By this task I mean any parent task which called this function */
		uint32_t notification_value;
		BaseType_t wait_status = xTaskNotifyWait(0x00, 0xFFFFFFFF, pdMS_TO_TICKS(CCU_MOTOR_TIMEOUT));

		/* Handle timeout */
		if (wait_status == pdFALSE) {
			xLeftMotorWaiting = NULL;
			return HAL_TIMEOUT;
		}

		/* Populate pmpi */
		pmpi->Command |= A2D_BATCH_READ1;
		pmpi->BrakePos = LeftMotorRxBuffer[0];
		pmpi->AccelPos = LeftMotorRxBuffer[1];
		pmpi->BatteryVoltage = LeftMotorRxBuffer[4];

		return HAL_OK;
	}
}
