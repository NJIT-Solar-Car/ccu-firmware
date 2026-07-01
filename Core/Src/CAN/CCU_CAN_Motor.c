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

/* Does the boilerplate stuff for each function. */
int setStuff(uint8_t 			Command,
		CAN_TxHeaderTypeDef 	*txHeader,
		MotorPacketInfo 		*pmpi,
		volatile TaskHandle_t	*xWaitingMotor, 
		CommandType 			*MotorState, 
		volatile uint8_t		*buffer) {

	/* Check if already run for current packet */
	if ((pmpi->Command & Command) > 0) {
		return 1;
	}
	
	/* Select the motor controller */
	if (pmpi->ControllerInfo->ID == CAN_ID_MOTOR_LEFT_CMD) {
		xWaitingMotor 	= &xLeftMotorWaiting;
		MotorState		= &LeftMotorState;
		buffer			= &LeftMotorRxBuffer[0];
	} else {
		xWaitingMotor 	= &xRightMotorWaiting;
		MotorState		= &RightMotorState;
		buffer			= &RightMotorRxBuffer[0];
	}

	/* Set motor state, and get task handle */
	*MotorState = Command;
	*xWaitingMotor = xTaskGetCurrentTaskHandle();

	/* Prepare and send message */
	txHeader->StdId = (uint32_t)pmpi->ControllerInfo->ID;
	txHeader->ExtId = 0x00;
	txHeader->IDE 	= CAN_ID_STD;
	txHeader->RTR 	= CAN_RTR_DATA;
	txHeader->DLC 	= 8;
	txHeader->TransmitGlobalTime = DISABLE;

	return 0;
}

HAL_StatusTypeDef A2D_BATCH_READ1_Func(MotorPacketInfo * pmpi) {
	CAN_TxHeaderTypeDef 	txHeader;
	volatile TaskHandle_t 	*xWaitingMotor;
	CommandType 			*MotorState;
	volatile uint8_t 		*buffer;
	uint32_t				assignedMailbox;

	if (setStuff(A2D_BATCH_READ1, &txHeader, pmpi, xWaitingMotor, MotorState, buffer) != 0) {
		/* Command already run for this packet */
		return HAL_OK;
	}

	uint8_t txData[] = {MOTOR_A2D_BATCH_READ1};

	HAL_StatusTypeDef txStatus = CCU_CAN_Tx(&hcan1, &txHeader, txData, &assignedMailbox, pdMS_TO_TICKS(CCU_MOTOR_TIMEOUT));
	
	/* Return any error that occurred when transmitting command */
	if (txStatus != HAL_OK) {
		*xWaitingMotor = NULL;
		return txStatus;
	}

	/* Make FreeRTOS block this task entirely. 
	 * By this task I mean any parent task which called this function.
	 * Clear all notification bits too. */
	uint32_t notification_value;
	BaseType_t wait_status = xTaskNotifyWait(0x00, 0xFFFFFFFF, &notification_value, pdMS_TO_TICKS(CCU_MOTOR_TIMEOUT));

	/* Handle timeout */
	if (wait_status == pdFALSE) {
		*xWaitingMotor = NULL;
		return HAL_TIMEOUT;
	}

	/* Populate pmpi */
	pmpi->Command |= A2D_BATCH_READ1;
	pmpi->BrakePos = buffer[0];
	pmpi->AccelPos = buffer[1];
	pmpi->BatteryVoltage = buffer[4];

	return HAL_OK;
}

HAL_StatusTypeDef MONITOR1_Func(MotorPacketInfo * pmpi) {	
	CAN_TxHeaderTypeDef 	txHeader;
	volatile TaskHandle_t 	*xWaitingMotor;
	CommandType 			*MotorState;
	volatile uint8_t 		*buffer;
	uint32_t				assignedMailbox;

	if (setStuff(MONITOR1, &txHeader, pmpi, xWaitingMotor, MotorState, buffer) != 0) {
		/* Command already run for this packet */
		return HAL_OK;
	}

	uint8_t txData[] = {MOTOR_MONITOR1};

	HAL_StatusTypeDef txStatus = CCU_CAN_Tx(&hcan1, &txHeader, txData, &assignedMailbox, pdMS_TO_TICKS(CCU_MOTOR_TIMEOUT));
	
	/* Return any error that occurred when transmitting command */
	if (txStatus != HAL_OK) {
		*xWaitingMotor = NULL;
		return txStatus;
	}

	/* Make FreeRTOS block this task entirely. 
	 * By this task I mean any parent task which called this function.
	 * Clear all notification bits too. */
	uint32_t notification_value;
	BaseType_t wait_status = xTaskNotifyWait(0x00, 0xFFFFFFFF, &notification_value, pdMS_TO_TICKS(CCU_MOTOR_TIMEOUT));

	/* Handle timeout */
	if (wait_status == pdFALSE) {
		*xWaitingMotor = NULL;
		return HAL_TIMEOUT;
	}

	/* Populate pmpi */
	pmpi->Command |= MONITOR1;
	pmpi->PWM 			 = buffer[0];
	pmpi->MotorTemp 	 = buffer[2];
	pmpi->ControllerTemp = buffer[3];
	pmpi->TempHighSide 	 = buffer[4];
	pmpi->TempLowSide 	 = buffer[5];

	return HAL_OK;
}

HAL_StatusTypeDef MONITOR2_Func(MotorPacketInfo * pmpi) {
	CAN_TxHeaderTypeDef 	txHeader;
	volatile TaskHandle_t 	*xWaitingMotor;
	CommandType 			*MotorState;
	volatile uint8_t 		*buffer;
	uint32_t				assignedMailbox;

	if (setStuff(MONITOR2, &txHeader, pmpi, xWaitingMotor, MotorState, buffer) != 0) {
		/* Command already run for this packet */
		return HAL_OK;
	}

	uint8_t txData[] = {MOTOR_MONITOR2};

	HAL_StatusTypeDef txStatus = CCU_CAN_Tx(&hcan1, &txHeader, txData, &assignedMailbox, pdMS_TO_TICKS(CCU_MOTOR_TIMEOUT));
	
	/* Return any error that occurred when transmitting command */
	if (txStatus != HAL_OK) {
		*xWaitingMotor = NULL;
		return txStatus;
	}

	/* Make FreeRTOS block this task entirely. 
	 * By this task I mean any parent task which called this function.
	 * Clear all notification bits too. */
	uint32_t notification_value;
	BaseType_t wait_status = xTaskNotifyWait(0x00, 0xFFFFFFFF, &notification_value, pdMS_TO_TICKS(CCU_MOTOR_TIMEOUT));

	/* Handle timeout */
	if (wait_status == pdFALSE) {
		*xWaitingMotor = NULL;
		return HAL_TIMEOUT;
	}

	/* Populate pmpi */
	pmpi->Command |= MONITOR2;
	pmpi->MotorRPM = ((uint16_t)buffer[0] << 8) | (uint16_t)buffer[1];

	return HAL_OK;
}

HAL_StatusTypeDef SW_ACC_Func(MotorPacketInfo * pmpi) {
	CAN_TxHeaderTypeDef 	txHeader;
	volatile TaskHandle_t 	*xWaitingMotor;
	CommandType 			*MotorState;
	volatile uint8_t 		*buffer;
	uint32_t				assignedMailbox;

	if (setStuff(SW_ACC, &txHeader, pmpi, xWaitingMotor, MotorState, buffer) != 0) {
		/* Command already run for this packet */
		return HAL_OK;
	}

	uint8_t txData[] = {MOTOR_SW_ACC};

	HAL_StatusTypeDef txStatus = CCU_CAN_Tx(&hcan1, &txHeader, txData, &assignedMailbox, pdMS_TO_TICKS(CCU_MOTOR_TIMEOUT));
	
	/* Return any error that occurred when transmitting command */
	if (txStatus != HAL_OK) {
		*xWaitingMotor = NULL;
		return txStatus;
	}

	/* Make FreeRTOS block this task entirely. 
	 * By this task I mean any parent task which called this function.
	 * Clear all notification bits too. */
	uint32_t notification_value;
	BaseType_t wait_status = xTaskNotifyWait(0x00, 0xFFFFFFFF, &notification_value, pdMS_TO_TICKS(CCU_MOTOR_TIMEOUT));

	/* Handle timeout */
	if (wait_status == pdFALSE) {
		*xWaitingMotor = NULL;
		return HAL_TIMEOUT;
	}

	/* Populate pmpi */
	pmpi->Command |= SW_ACC;
	switch (buffer[0]) {
	case 0:
		pmpi->Status = STATUS_NONE;
	case 1:
		pmpi->Status = ACC;
	}

	return HAL_OK;
}

HAL_StatusTypeDef SW_BRAKE_Func(MotorPacketInfo * pmpi) {
	CAN_TxHeaderTypeDef 	txHeader;
	volatile TaskHandle_t 	*xWaitingMotor;
	CommandType 			*MotorState;
	volatile uint8_t 		*buffer;
	uint32_t				assignedMailbox;

	if (setStuff(SW_BRAKE, &txHeader, pmpi, xWaitingMotor, MotorState, buffer) != 0) {
		/* Command already run for this packet */
		return HAL_OK;
	}

	uint8_t txData[] = {MOTOR_SW_BRAKE};

	HAL_StatusTypeDef txStatus = CCU_CAN_Tx(&hcan1, &txHeader, txData, &assignedMailbox, pdMS_TO_TICKS(CCU_MOTOR_TIMEOUT));
	
	/* Return any error that occurred when transmitting command */
	if (txStatus != HAL_OK) {
		*xWaitingMotor = NULL;
		return txStatus;
	}

	/* Make FreeRTOS block this task entirely. 
	 * By this task I mean any parent task which called this function.
	 * Clear all notification bits too. */
	uint32_t notification_value;
	BaseType_t wait_status = xTaskNotifyWait(0x00, 0xFFFFFFFF, &notification_value, pdMS_TO_TICKS(CCU_MOTOR_TIMEOUT));

	/* Handle timeout */
	if (wait_status == pdFALSE) {
		*xWaitingMotor = NULL;
		return HAL_TIMEOUT;
	}

	/* Populate pmpi */
	pmpi->Command |= SW_BRAKE;
	switch (buffer[0]) {
	case 0:
		pmpi->Status = STATUS_NONE;
	case 1:
		pmpi->Status = BRK;
	}

	return HAL_OK;
}

HAL_StatusTypeDef SW_REV_Func(MotorPacketInfo * pmpi) {
	CAN_TxHeaderTypeDef 	txHeader;
	volatile TaskHandle_t 	*xWaitingMotor;
	CommandType 			*MotorState;
	volatile uint8_t 		*buffer;
	uint32_t				assignedMailbox;

	if (setStuff(SW_REV, &txHeader, pmpi, xWaitingMotor, MotorState, buffer) != 0) {
		/* Command already run for this packet */
		return HAL_OK;
	}

	uint8_t txData[] = {MOTOR_SW_REV};

	HAL_StatusTypeDef txStatus = CCU_CAN_Tx(&hcan1, &txHeader, txData, &assignedMailbox, pdMS_TO_TICKS(CCU_MOTOR_TIMEOUT));
	
	/* Return any error that occurred when transmitting command */
	if (txStatus != HAL_OK) {
		*xWaitingMotor = NULL;
		return txStatus;
	}

	/* Make FreeRTOS block this task entirely. 
	 * By this task I mean any parent task which called this function.
	 * Clear all notification bits too. */
	uint32_t notification_value;
	BaseType_t wait_status = xTaskNotifyWait(0x00, 0xFFFFFFFF, &notification_value, pdMS_TO_TICKS(CCU_MOTOR_TIMEOUT));

	/* Handle timeout */
	if (wait_status == pdFALSE) {
		*xWaitingMotor = NULL;
		return HAL_TIMEOUT;
	}

	/* Populate pmpi */
	pmpi->Command |= SW_REV;
	switch (buffer[0]) {
	case 0:
		pmpi->Status = STATUS_NONE;
	case 1:
		pmpi->Status = REV;
	}

	return HAL_OK;
}

HAL_StatusTypeDef A2D_BATCH_READ2_Func(MotorPacketInfo * pmpi) {
	/* Check if already run for current packet */
	// if (pmpi->Command & A2D_BATCH_READ2 > 0) {
	// 	return HAL_OK
	// }

	// TaskHandle_t 	*xWaitingMotor;
	// CommandType  	*MotorState;
	// uint8_t 		*buffer;

	/* Select the motor controller */
	// if (pmpi->ControllerInfo->ID == CAN_ID_MOTOR_LEFT_CMD) {
	// 	xWaitingMotor 	= &xLeftMotorWaiting;
	// 	MotorState		= &LeftMotorState;
	// 	buffer			= &LeftMotorRxBuffer[0];
	// } else {
	// 	xWaitingMotor 	= &xRightMotorWaiting;
	// 	MotorState		= &RightMotorState;
	// 	buffer			= &RightMotorRxBuffer[0];
	// }

	/* Set motor state, and get task handle */
	// *MotorState = A2D_BATCH_READ2;
	// *xWaitingMotor = xTaskGetCurrentHandle();

	/* Prepare and send message */
	// uint32_t 				assignedMailbox;
	// HAL_StatusTypeDef 		txStatus;
	// CAN_TxHeaderTypeDef 	txHeader;

	// txHeader.StdId 	= (uint32_t)pmpi->ControllerInfo->ID;
	// txHeader.ExtId 	= 0x00;
	// txHeader.IDE 	= CAN_ID_STD;
	// txHeader.RTR 	= CAN_RTR_DATA;
	// txHeader.DLC 	= 8;
	// txHeader.TransmitGlobalTime = DISABLE;

	// uint8_t txData[] = {MOTOR_A2D_BATCH_READ1};

	// txStatus = CCU_CAN_Tx(&hcan1, &txHeader, txData, &assignedMailbox, pdMS_TO_TICKS(CCU_MOTOR_TIMEOUT));
	// 
	/* Return any error that occurred when transmitting command */
	// if (txStatus != HAL_OK) {
	// 	*xWaitingMotor = NULL;
	// 	return txStatus;
	// }

	// /* Make FreeRTOS block this task entirely. 
	//  * By this task I mean any parent task which called this function.
	//  * Clear all notification bits too. */
	// uint32_t notification_value;
	// BaseType_t wait_status = xTaskNotifyWait(0x00, 0xFFFFFFFF, &notification_value, pdMS_TO_TICKS(CCU_MOTOR_TIMEOUT));

	/* Handle timeout */
	// if (wait_status == pdFALSE) {
	// 	*xWaitingMotor = NULL;
	// 	return HAL_TIMEOUT;
	// }

	/* Populate pmpi */
	// pmpi->Command |= A2D_BATCH_READ2;
	// pmpi->BrakePos = LeftMotorRxBuffer[0];
	// pmpi->AccelPos = LeftMotorRxBuffer[1];
	// pmpi->BatteryVoltage = LeftMotorRxBuffer[4];

	return HAL_OK;
}
