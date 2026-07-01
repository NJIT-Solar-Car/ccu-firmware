/*
 * flash_internal.h
 *
 *  Created on: Jun 23, 2026
 *      Author: marko
 */

/**
 * @file flash_internal.h
 * @brief Defines constants, macros, and functions to do with internal logging
 * @author Marco Sitar
 */

#ifndef FLASH_INTERNAL_H_
#define FLASH_INTERNAL_H_

/**
 * @addtogroup CCU_Firmware
 *
 * @{
 */

/**
 * @defgroup Flash
 * @brief Functions to interact with internal flash
 * 
 * @{
 */

#include "Flash/flash.h"

/**
 * @defgroup Flash_Variables
 *
 * @{
 */

// linker region
extern uint8_t __log_flash_start__;	/**< Start address of flash logging area */
extern uint8_t __log_flash_end__;	/**< End address of flash logging area */

/**
 * @}
 */

/**
 * @defgroup Flash_Defines
 *
 * @{
 */

#define FLASH_INTERNAL_LOG_BASE ((uint32_t)&__log_flash_start__)						/**< Start address of flash logging area */
#define FLASH_INTERNAL_LOG_SIZE ((uint32_t)(&__log_flash_end__ - &__log_flash_start__))	/**< Size of logging area */
#define FLASH_INTERNAL_WRITE_ALIGN 8u													/**< TODO: Marco plz fill this in */

/**
 * @}
 */

/**
 * @defgroup Flash_Funcs
 *
 * @{
 */

/**
 * @brief Reads from internal flash, and writes to the destination.
 * @param addr: address in flash to read from
 * @param dst:  destination to write bytes to (void pointer)
 * @param len:  length to read in bytes
 * @retval flash_status_t: status of flash read
 */
flash_status_t flash_internal_read(uint32_t addr, void *dst, size_t len);

/**
 * @brief Reads from source in memory, and writes internal flash.
 * @param addr: address in flash to read from
 * @param src:  source to read bytes from (void pointer)
 * @param len:  length to write in bytes
 * @retval flash_status_t: status of flash read
 */
flash_status_t flash_internal_write(uint32_t addr, const void *src, size_t len);

/**
 * @brief Erases all logged data.
 * @retval flash_status_t: status of flash read
 */
flash_status_t flash_internal_erase_log(void);

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

#endif /* FLASH_INTERNAL_H_ */
