/*
 * serial.h
 *
 *  Created on: Jun 29, 2026
 *      Author: marko
 */

/**
 * @file serial.h
 * @brief Functions to interact with serial input/output (via UART)
 */

#ifndef SERIAL_H_
#define SERIAL_H_

/**
 * @addtogroup CCU_Firmware
 *
 * @{
 */

/**
 * @defgroup Serial
 *
 * @{
 */

/**
 * @brief Reads a byte into an internal line buffer. Upon a newline
 * or carriage return, if line has command "log dump", it will dump to
 * usb
 * @param byte: the byte to be consumed.
 */ 
void serial_rx_byte(uint8_t byte);

/**
 * @}
 */

/**
 * @}
 */

#endif /* SRC_SERIAL_SERIAL_H_ */
