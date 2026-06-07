#ifndef CAN_DICTIONARY_H
#define CAN_DICTIONARY_H

//Kelly Motor Controller Base IDs
#define CAN_ID_MOTOR_CMD  0x6B  //Command RX
#define CAN_ID_MOTOR_RSP  0x73  //Response TX

//Command IDs
#define MOTOR_CMD_ANALOG1  0x1B  //Request analog data: Brake_AD, TPS_AD, OpVoltage_AD, Vs_AD, Bplus_AD, uint8
#define MOTOR_CMD_ANALOG2  0x1A  //Request phase data: Ia_AD, Ib_AD, Ic_AD, Va_AD, Vb_AD, Vc_AD, uint8
#define MOTOR_CMD_PWM      0x33  //Request PWM, enable status, motor temp, controller/FET temperatures, uint8
#define 
