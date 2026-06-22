/**
 * @file can_dictionary.h
 * @brief Holds the addresses of the Kelly KBL controller, as well as motor commands
 * @author JP Ochiagha
 */

/**
 * @addtogroup CCU_Firmware
 *
 * @{
 */

/**
 * @defgroup CAN_Firmware
 * @brief Holds the addresses of the Kelly KBL controller, as well as motor commands
 *
 * @{
 */

#ifndef CAN_DICTIONARY_H
#define CAN_DICTIONARY_H

/**
 * @defgroup CAN_Addresses
 * @brief Addresses of CAN devices (controller and CCU).
 *
 * @{
 */
/*Kelly Motor Controller Base IDs*/
#define CAN_ID_MOTOR1_CMD                0x6B    /**< Command RX*/
#define CAN_ID_MOTOR2_CMD				 0x6C
#define CAN_ID_MOTOR1_RSP                0x73    /**< Response TX*/
#define CAN_ID_MOTOR2_RSP				 0x74
/**
 * @}
 */

/**
 * @defgroup Motor_IDs
 * @brief Command IDs for the various commands
 *
 * @{
 */
/*Kelly Motor Command IDs*/
#define MOTOR_CMD_ANALOG1               0x1B    /**< Request analog data: Brake_AD, TPS_AD, OpVoltage_AD, Vs_AD, Bplus_AD, uint8*/
#define MOTOR_CMD_ANALOG2               0x1A    /**< Request phase data: Ia_AD, Ib_AD, Ic_AD, Va_AD, Vb_AD, Vc_AD, uint8*/
#define MOTOR_CMD_READ_PWM              0x33    /**< Request PWM, enable status, motor temp, controller/FET temperatures, uint8*/
#define MOTOR_CMD_READ_RPM              0x37    /**< Request mechanical RPM and current percent, uint8*/
#define MOTOR_CMD_THROTTLE              0x42    /**< Request throttle switch status, uint8*/
#define MOTOR_CMD_BRAKE                 0x43    /**< Request brake switch status, uint8*/
#define MOTOR_CMD_REVERSE               0x44    /**< Request reverse switch status, uint8*/
/**
 * @}
 */

/**
 * @defgroup MPPT1_IDs
 * @brief Base IDs for the Elmar MPPT1 controller
 *
 * @{
 */

/*Elmar MPPT1 Base IDs*/
#define CAN_ID_MPPT1_INPUTMEAS          0x600   /**< Bytes 0-3: input_voltage; Bytes 4-7: input_current, float32, tx*/
#define CAN_ID_MPPT1_OUTPUTMEAS         0x601   /**< Bytes 0-3: output_voltage; Bytes 4-7: output_current, float32, tx*/
#define CAN_ID_MPPT1_TEMP               0x602   /**< Bytes 0-3: mosfet_temp; Bytes 4-7: ctrl_temp, float32, tx*/
#define CAN_ID_MPPT1_AUXPSU             0x603   /**< Bytes 0-3: rail_12V; Bytes 4-7: rail_3.3V, float32, tx*/
#define CAN_ID_MPPT1_LIMITS             0x604   /**< Bytes 0-3: max_out_volt; Bytes 4-7: max_in_current, float32, tx*/
#define CAN_ID_MPPT1_STATUS             0x605   /**< Byte 0: can_rx_error; Byte 1: can_tx_error, uint8; Byte 2: can_tx_overflow_counter, uint8; Byte 3: error_flags (b7=low array power, b6=MOSFET overheat, b5=battery low, b4=battery full, b3=12V undervoltage, b2=reserved, b1=HW overcurrent, b0=HW overvoltage, uint8; Byte 4: limit_flags (b7=input current min, b6=input current max, b5=output voltage max, b4=MOSFET temp, b3=duty min, b2=duty max, b1=local MPPT, b0=global MPPT), uint8; Byte 5: mode (0 = standby, 1 = on), uint8; Byte 6: reserved, uint8; Byte 7: test_counter (use as heartbeat), uint8; tx */
#define CAN_ID_MPPT1_POWER_CONN         0x606   /**< Bytes 0-3: vout_fuse; Bytes 4-7: power_conn_temp, float32, tx*/
#define CAN_ID_CCU_MPPT1_MODE           0x608   /**< mode_cmd, 0 = standby, 1 = on, uint8, rx*/
#define CAN_ID_CCU_MPPT1_VMAX           0x60A   /**< Bytes 0-3: max_out_volt_cmd, float32, rx*/
#define CAN_ID_CCU_MPPT1_IMAX           0x60B   /**< Bytes 0-3: max_out_current_cmd, float32, rx*/

/*Elmar MPPT1 Data*/
typedef struct {
	// 0x600 Inputs
	float InputVoltage;
	float InputCurrent;
	// 0x601 Outputs
	float OutputVoltage;
	float OutputCurrent;
	// 0x602 Temps
	float MosfetTemp;
	float CtrlTemp;
	// 0x603 - Aux Power Supply
	float Rail12V;
	float Rail3V3;
	// 0x604 Limits
	float MaxOutVolt;
	float MaxInCurrent;
	// 0x605 Status & Flags
	uint8_t ErrorFlags;  
	uint8_t LimitFlags;
	uint8_t Mode;  
	// 0x606 Power Connector Safety
	float VoutFuse;
	float PowerConnTemp;   
} MPPT1_Data_t;

/**
 * @}
 */

#endif /*CAN_DICTIONARY_H*/

/**
 * @}
 */

/**
 * @}
 */
