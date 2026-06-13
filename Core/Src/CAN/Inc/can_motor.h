/**
 * @file can_motor.h
 * @brief Holds function and struct definitions which pertain to the Kelly KBL motor controller.
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

/**
 * @defgroup CAN_Structs
 * @brief Data structures to be used for CAN communication. This includes motor controller state, state of the CAN-logic on CCU side, and possibly more.
 *
 * @{
 */

/**
 * @brief Struct holding general, static info about a motor controller. Order is optimized for [struct size](https://www.geeksforgeeks.org/c/structure-member-alignment-padding-and-data-packing/)
 * which is currently 20 bytes, when tested with GCC.
 */
typedef struct MotorControllerInfo {
	char 		Model[8];					/**< Model name of the motor controller. Really unnecessary, but we have two
					 						     motors and will only have two of these structs instantiated. */
	uint32_t 	ID;							/**< 29 bit ID of the motor controller. This will be split into Std_ID and
											     ExtID within the send/receive functions */
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
 * @brief Specifies which commands have been run. Only members filled in by those commands are valid. Check
 * individual command functions for more details.
 */
typedef enum CommandType {
	NONE				= 0x00,		/**< No command */
	A2D_BATCH_READ1		= (1 << 0),	/**< A2D Batch Read1. Provides brake and throttle position, and battery voltage */
	A2D_BATCH_READ2		= (1 << 1),	/**< A2D Batch Read2. Provides 3 phase voltage and current readings. *CURRENTLY UNUSED!!!* */
	MONITOR1			= (1 << 2),	/**< Monitor1. Provides PWM, as well as motor and controller temp, and low/high side mosfet temp */
	MONITOR2			= (1 << 3),	/**< Monitor2. Provides motor RPM */
	SW_ACC				= (1 << 4),	/**< Provides ACC field of #MotorStatus */
	SW_BRAKE			= (1 << 5),	/**< Provides BRK field of #MotorStatus */
	SW_REV				= (1 << 6)	/**< Provides REV field of #MotorStatus */
} __attribute__((packed)) CommandType;

/**
 * @brief Specifies the status of the motor. Accelerating, braking, and/or reversing.
 */
typedef enum MotorStatus {
	NONE	= 0x00,		/**< No motor state :( */
	ACC		= (1 << 0),	/**< Accelerator being pressed */
	BRK		= (1 << 1),	/**< Brake being pressed */
	REV		= (1 << 2)	/**< Motor is reversing */
} __attribute__((packed)) MotorStatus;

/**
 * @}
 */

/**
 * @defgroup CAN_Funcs
 * @brief Functions for interacting with CAN bus
 *
 * @{
 */

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
