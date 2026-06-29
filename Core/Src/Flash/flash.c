/*
 * flash.c
 *
 *  Created on: Jun 28, 2026
 *      Author: marko
 */

#include "Flash/flash.h"

#if defined(USE_FLASH_INTERNAL)
#include "flash_internal.h"

flash_status_t flash_read(uint32_t offset, void *dst, size_t len) {
	return flash_internal_read(FLASH_INTERNAL_LOG_BASE + offset, dst, len);
}

flash_status_t flash_write(uint32_t offset, const void *src, size_t len) {
	return flash_internal_write(FLASH_INTERNAL_LOG_BASE + offset, src, len);
}

flash_status_t flash_erase() {
	return flash_internal_erase_log();
}

uint32_t flash_size(void) {
	return FLASH_INTERNAL_LOG_SIZE;
}

size_t flash_write_alignment(void) {
	return FLASH_INTERNAL_WRITE_ALIGN;
}

#elif defined(USE_FLASH_EXTERNAL)
#error "external flash not implemented."
#endif
