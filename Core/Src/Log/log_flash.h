/*
 * log_flash.h
 *
 *  Created on: Jun 28, 2026
 *      Author: marko
 */

#ifndef LOG_FLASH_H_
#define LOG_FLASH_H_

#include "Log/log.h"
#include "Flash/flash.h"

#include <stddef.h>
#include <stdint.h>

flash_status_t log_flash_init(void);
flash_status_t log_flash_append(const log_record_t *record, const void *payload);
flash_status_t log_flash_iterate(log_read_fn_t block, uint32_t *end_offset);
flash_status_t log_flash_erase(void);

#endif /* LOG_FLASH_H_ */
