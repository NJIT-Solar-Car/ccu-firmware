/*
 * flash_internal.h
 *
 *  Created on: Jun 23, 2026
 *      Author: marko
 */

#ifndef FLASH_INTERNAL_H_
#define FLASH_INTERNAL_H_

#include "Flash/flash.h"

// linker region
extern uint8_t __log_flash_start__;
extern uint8_t __log_flash_end__;

#define FLASH_INTERNAL_LOG_BASE ((uint32_t)&__log_flash_start__)
#define FLASH_INTERNAL_LOG_SIZE ((uint32_t)(&__log_flash_end__ - &__log_flash_start__))
#define FLASH_INTERNAL_WRITE_ALIGN 8u

flash_status_t flash_internal_read(uint32_t addr, void *dst, size_t len);
flash_status_t flash_internal_write(uint32_t addr, const void *src, size_t len);
flash_status_t flash_internal_erase_log(void);

#endif /* FLASH_INTERNAL_H_ */
