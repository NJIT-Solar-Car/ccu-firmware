/*
 * log_flash.h
 *
 *  Created on: Jun 28, 2026
 *      Author: marko
 */

/**
 * @file log_flash.h
 * @brief Functions to log data into flash memory
 */

#ifndef LOG_FLASH_H_
#define LOG_FLASH_H_

/**
 * @addtogroup CCU_Firmware
 *
 * @{
 */

/**
 * @defgroup Logging
 * @brief Functions to log data into flash memory
 *
 * @{
 */

#include "Log/log.h"
#include "Flash/flash.h"

#include <stddef.h>
#include <stdint.h>

/**
 * @brief Initializes the logging library
 * @retval flash_status_t status of flash init
 */
flash_status_t log_flash_init(void);

/**
 * @brief TODO: Marco plz fill this in
 * @param record:
 * @param payload:
 * @retval flash_status_t
 */
flash_status_t log_flash_append(const log_record_t *record, const void *payload);

/**
 * @brief TODO: Marco plz fill this in
 * @param block:
 * @param end_offset:
 * @retval flash_status_t
 */
flash_status_t log_flash_iterate(log_read_fn_t block, uint32_t *end_offset);

/**
 * @brief Erases flash? TODO: Marco plz fill this in
 * @retval flash_status_t
 */
flash_status_t log_flash_erase(void);

/**
 * @}
 */

/**
 * @}
 */

#endif /* LOG_FLASH_H_ */
