/*
 * flash.h
 *
 *  Created on: Jun 28, 2026
 *      Author: marko
 */

#ifndef FLASH_H_
#define FLASH_H_

#include <stdint.h>
#include <stddef.h>

#define USE_FLASH_INTERNAL	// make the flash backend use internal flash
//#define USE_FLASH_EXTERNAL	// make the flash backend use external flash

#if defined(USE_FLASH_INTERNAL) == defined(USE_FLASH_EXTERNAL)
#error "exactly one flash backend must be selected."
#endif

typedef enum {
    FLASH_OK,
    FLASH_ERR_RANGE,
    FLASH_ERR_ALIGN,
    FLASH_ERR_IO,
    FLASH_ERR_CORRUPT,
    FLASH_ERR_FULL,
} flash_status_t;

flash_status_t flash_read(uint32_t offset, void *dst, size_t len);
flash_status_t flash_write(uint32_t offset, const void *src, size_t len);
flash_status_t flash_erase();

uint32_t flash_size(void);
size_t flash_write_alignment(void);

#endif /* FLASH_H_ */
