/**
 * @file can_motor.h
 * @brief Holds function and struct definitions which pertain to the Kelly KBL motor controller *and* CAN bus functions in general.
 * @author Arnav Revankar
 */

/**
 * @addtogroup CCU_Firmware
 *
 * @{
 */

/**
 * @addtogroup CAN_Firmware
 *
 * @{
 */

#ifndef CAN_MOTOR
#define CAN_MOTOR

#include <stdint.h>

#include "FreeRTOS.h"
#include "stm32f4xx_hal.h"

/**
 * @defgroup CAN_Defines
 * @brief Define macros to be used *by us* for CAN communication
 *
 * @{
 */

#define CCU_CAN_RINGBUFFER_SIZE 20	/**< Size of the \ref MotorControllerInfo ringbuffer */
#define CCU_MOTOR_TIMEOUT 500		/**< Timeout (in ms) of the CAN motor controller functions */

/**
 * @}
 */

/**
 * @defgroup CAN_Structs
 * @brief Data structures to be used for CAN communication. This includes motor controller state, state of the CAN-logic on CCU side, and possibly more.
 *
 * @{
 */

/**
 * @brief Specifies which commands have been run. Only members filled in by those commands are valid. Check
 * individual command functions for more details.
 */
typedef enum CommandType {
	COMMAND_NONE		= 0x00,		/**< No command */
	A2D_BATCH_READ1		= (1 << 0),	/**< A2D Batch Read1. Provides brake and throttle position, and battery voltage */
	A2D_BATCH_READ2		= (1 << 1),	/**< A2D Batch Read2. Provides 3 phase voltage and current readings. *CURRENTLY UNUSED!!!* */
	MONITOR1			= (1 << 2),	/**< Monitor1. Provides PWM, as well as motor and controller temp, and low/high side mosfet temp */
	MONITOR2			= (1 << 3),	/**< Monitor2. Provides motor RPM */
	SW_ACC				= (1 << 4),	/**< Provides ACC field of #MotorStatus */
	SW_BRAKE			= (1 << 5),	/**< Provides BRK field of #MotorStatus */
	SW_REV				= (1 << 6)	/**< Provides REV field of #MotorStatus */
} CommandType;

/**
 * @brief Specifies the status of the motor. Accelerating, braking, and/or reversing.
 */
typedef enum MotorStatus {
	STATUS_NONE = 0x00,		/**< No status */
	ACC			= (1 << 0),	/**< Accelerator being pressed */
	BRK			= (1 << 1),	/**< Brake being pressed */
	REV			= (1 << 2)	/**< Motor is reversing */
} MotorStatus;

/**
 * @brief Struct holding general, static info about a motor controller. Order is optimized for [struct size](https://www.geeksforgeeks.org/c/structure-member-alignment-padding-and-data-packing/)
 * which is currently 20 bytes, when tested with GCC.
 */
typedef struct MotorControllerInfo {
	char 		Model[8];					/**< Model name of the motor controller. Really unnecessary, but we have two
					 						     motors and will only have two of these structs instantiated. */
	uint16_t 	ID;							/**< 11 bit ID of the motor controller. */
	char 		SoftwareVer[2];				/**< Software version. I believe it should be 83. */
	uint8_t		ThrottleLowEndDeadZone;		/**< Throttle Low-end dead zone of controller. Can be altered via kelly software over rs232. */
	uint8_t		ThrottleHighEndDeadZone;	/**< Throttle High-end dead zone of controller. Can be altered via kelly software over rs232. */
	uint8_t		BrakeLowEndDeadZone;		/**< Brake Low-end dead zone of controller. Can be altered via kelly software over rs232. */
	uint8_t		BrakeHighEndDeadZone;		/**< Brake High-end dead zone of controller. Can be altered via kelly software over rs232. */
} MotorControllerInfo;

/**
 * @brief Struct holding information about a received packet. By default, this has all the data you can acquire from the motor controller. The `Command` byte (with knowledge of CommandType enum)
 * can let one decipher which commands have been run. This struct is also optimized for padding.
 */
typedef struct MotorPacketInfo {
	MotorControllerInfo * ControllerInfo;	/**< Pointer to #MotorControllerInfo struct */

	uint16_t MotorRPM;		/**< This is the RPM of the motor */
	
	CommandType	Command;	/**< Bitmask which tells you which command has been run. Check #CommandType for more info. */

	uint8_t BrakePos;		/**< Brake position */
	uint8_t	AccelPos;		/**< Accelerator position */
	uint8_t	BatteryVoltage;	/**< Battery voltage. This / 2.71 will give the battery voltage */
	
	uint8_t	PWM;			/**< Output duty cycle */

	uint8_t	MotorTemp;		/**< Motor temperature in degrees C. 0xFF if no sensor connected */
	uint8_t	ControllerTemp;	/**< Controller temperature in degrees C. Inaccurate below 30 degrees */
	uint8_t TempHighSide;	/**< Temperature of the high side mosfets in degrees C. Inaccurate below 30 degrees */
	uint8_t TempLowSide;	/**< Temperature of the low side mosfets in degrees C. Inaccurate below 30 degrees */
	
	MotorStatus Status;		/**< Functions as a bitmask. Check #MotorStatus for more info. */
} MotorPacketInfo;

/**
 * @}
 */

/**
 * @defgroup CAN_Funcs
 * @brief *User defined* functions for interacting with CAN bus. These effectively wrap the HAL functions and use FreeRTOS mutexes
 * to guarantee no race conditions. **CAN Function docs begin on page 87 of UM1725!!**
 *
 * @{
 */

/**
 * @brief Initializes the global CAN mutex variable (in \ref CCU_CAN.c)
 * @retval BaseType_t pdPASS if successful, pdFAIL if mutex creation failed.
 */
BaseType_t CCU_CAN_Init(void);

/**
 * @brief Initialize the filters for the CAN peripheral. Assumes that \ref MX_CAN1_Init() has been called. Ideally place 
 * after USER CODE BEGIN 2.
 */
void CCU_CAN_FilterConfig(CAN_HandleTypeDef *phcan1, CAN_HandleTypeDef *phcan2);

/**
 * @brief  Thread-safe wrapper for HAL_CAN_AddTxMessage. Blocks until a hardware mailbox is available and transmits safely.
 * @param  hcan: pointer to a CAN_HandleTypeDef structure
 * @param  pHeader: pointer to a CAN_TxHeaderTypeDef structure
 * @param  aData: array containing the data payload
 * @param  pTxMailbox: pointer to a variable where the assigned mailbox will be stored
 * @param  xTicksToWait: Maximum FreeRTOS ticks to wait to acquire the mutex. Optimal value TBD.
 * @retval HAL_StatusTypeDef: HAL_OK, HAL_ERROR, or HAL_TIMEOUT (if mutex couldn't be acquired)
 */
HAL_StatusTypeDef CCU_CAN_Tx(CAN_HandleTypeDef *hcan, CAN_TxHeaderTypeDef *pHeader, uint8_t aData[], uint32_t *pTxMailbox, TickType_t xTicksToWait);

/**
 * @}
 */

/**
 * @defgroup CAN_Motor_Funcs
 * @brief Functions for interacting with motor controllers via the CAN bus. Each function's parameter is a pointer to a motor packet info struct, 
 * and the functions assume they've been initialized to a default. It is from here (pmpi->ControllerInfo->ID) that the motor controller's CAN ID
 * will be obtained. Additionally, all functions will be blocking until either the controller responds, or the \ref CCU_MOTOR_TIMEOUT times out.
 *
 * Overall procedure for each function is: Send a command to the motor controller. Then, use xTaskNotifyWait to block until either the call times out, or a message is received.
 *
 * A recieved message is processed by the ISR callback function \ref HAL_CAN_RxFifo0MsgPendingCallback. This function will GLOBALLY handle
 * all received messages, categorize them based on its ID and/or data, and accordingly pipe the message along. Same applies here.
 *
 * In main.c there are two global volatile variables of TaskHandle_t named xLeftMotorWaiting and xRightMotorWaiting. According to which
 * motor controller we're communicating with, the corresponding task handle will be set to the current task handle with xTaskGetCurrentTaskHandle. 
 * Then, when a received message is categorized within the ISR callback function, it will accordingly wake the task back up.
 *
 * @{
 */

/**
 * @brief A2D Batch Read1. Check \ref A2D_BATCH_READ1
 * @param pmpi: Pointer to MotorPacketInfo struct.
 */
HAL_StatusTypeDef A2D_BATCH_READ1_Func(MotorPacketInfo * pmpi);

/**
 * @brief A2D Batch Read2. Check \ref A2D_BATCH_READ2. Currently UNUSED!!! WILL RETURN ABSOLUTELY NOTHING, BE WARNED!!!
 * @param pmpi: Pointer to MotorPacketInfo struct.
 */
HAL_StatusTypeDef A2D_BATCH_READ2_Func(MotorPacketInfo * pmpi);

/**
 * @brief Monitor1. Check \ref MONITOR1
 * @param pmpi: Pointer to MotorPacketInfo struct.
 */
HAL_StatusTypeDef MONITOR1_Func(MotorPacketInfo * pmpi);

/**
 * @brief Monitor2. Check \ref MONITOR2
 * @param pmpi: Pointer to MotorPacketInfo struct.
 */
HAL_StatusTypeDef MONITOR2_Func(MotorPacketInfo * pmpi);

/**
 * @brief Check \ref SW_ACC
 * @param pmpi: Pointer to MotorPacketInfo struct.
 */
HAL_StatusTypeDef SW_ACC_Func(MotorPacketInfo * pmpi);

/**
 * @brief Check \ref SW_BRAKE
 * @param pmpi: Pointer to MotorPacketInfo struct.
 */
HAL_StatusTypeDef SW_BRAKE_Func(MotorPacketInfo * pmpi);

/**
 * @brief Check \ref SW_REV
 * @param pmpi: Pointer to MotorPacketInfo struct.
 */
HAL_StatusTypeDef SW_REV_Func(MotorPacketInfo * pmpi);

/**
 * @}
 */

#endif /* CAN_MOTOR */

/**
 * @}
 */

/**
 * @}
 */
