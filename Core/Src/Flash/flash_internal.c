/*
 * flash_internal.c
 *
 *  Created on: Jun 23, 2026
 *      Author: marko
 */

#include "flash_internal.h"

#include <string.h>
#include <stdbool.h>

#include "stm32f4xx_hal.h"

static bool is_in_log_region(uint32_t addr, size_t len) {
	const uint32_t base = FLASH_INTERNAL_LOG_BASE;
	const uint32_t end = base + FLASH_INTERNAL_LOG_SIZE;

	if (len == 0u) {
		return true;
	}

	return addr >= base &&
		   addr <= end &&
		   len <= (size_t)(end - addr);
}

static bool is_aligned(uint32_t value, uint32_t align) {
	return (value % align) == 0u;
}

flash_status_t flash_internal_read(uint32_t addr, void *dst, size_t len) {
	if (dst == NULL && len != 0u) {
		return FLASH_ERR_RANGE;
	}

	if (!is_in_log_region(addr, len)) {
		return FLASH_ERR_RANGE;
	}

	memcpy(dst, (const void *)addr, len);
	return FLASH_OK;
}

flash_status_t flash_internal_write(uint32_t addr, const void *src, size_t len) {
	if (src == NULL && len != 0) {
		return FLASH_ERR_RANGE;
	}

	if (!is_in_log_region(addr, len)) {
		return FLASH_ERR_RANGE;
	}

	if (!is_aligned(addr, FLASH_INTERNAL_WRITE_ALIGN) ||
		!is_aligned((uint32_t) len, FLASH_INTERNAL_WRITE_ALIGN)) {
		return FLASH_ERR_ALIGN;
	}

	HAL_FLASH_Unlock();

	const uint8_t *bytes = src;

	for (size_t offset = 0; offset < len; offset += FLASH_INTERNAL_WRITE_ALIGN) {
		uint64_t word;
		memcpy(&word, bytes + offset, sizeof(word));

		if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, addr + (uint32_t) offset, word) != HAL_OK) {
			HAL_FLASH_Lock();
			return FLASH_ERR_IO;
		}
	}

	HAL_FLASH_Lock();
	return FLASH_OK;
}

#define FLASH_INTERNAL_LOG_FIRST_SECTOR FLASH_SECTOR_15
#define FLASH_INTERNAL_LOG_NUM_SECTORS  1u

flash_status_t flash_internal_erase_log(void) {
	FLASH_EraseInitTypeDef erase = {0};
	uint32_t sector_error = 0;

	erase.TypeErase = FLASH_TYPEERASE_SECTORS;
	erase.VoltageRange = FLASH_VOLTAGE_RANGE_3;
	erase.Sector = FLASH_INTERNAL_LOG_FIRST_SECTOR;
	erase.NbSectors = FLASH_INTERNAL_LOG_NUM_SECTORS;

	HAL_FLASH_Unlock();

	if (HAL_FLASHEx_Erase(&erase, &sector_error) != HAL_OK) {
		HAL_FLASH_Lock();
		return FLASH_ERR_IO;
	}

	HAL_FLASH_Lock();
	return FLASH_OK;
}
