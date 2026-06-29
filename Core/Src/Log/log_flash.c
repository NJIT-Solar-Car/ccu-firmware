/*
 * log_flash.c
 *
 *  Created on: Jun 28, 2026
 *      Author: marko
 */

#include "log_flash.h"
#include "Log/log_events.h"

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#define LOG_FLASH_MAX_RECORD_SIZE 256u

static uint32_t _write_offset;

static uint32_t _align_up_u32(uint32_t value, uint32_t align)
{
	return (value + align - 1u) & ~(align - 1u);
}

flash_status_t log_flash_append(const log_record_t *record, const void *payload)
{
	uint8_t buf[LOG_FLASH_MAX_RECORD_SIZE];

	const uint32_t raw_size = (uint32_t) sizeof record + (uint32_t) record->len;
	const uint32_t record_size = _align_up_u32(raw_size, flash_write_alignment());

	if (record_size > sizeof buf) {
		return FLASH_ERR_RANGE;
	}

	memset(buf, 0xFFu, record_size);
	memcpy(buf, &record, sizeof record);

	if (record->len != 0) {
		memcpy(buf + sizeof record, payload, record->len);
	}

	flash_status_t st = flash_write(_write_offset, buf, record_size);
	if (st != FLASH_OK) {
		return st;
	}

	_write_offset += record_size;
	return FLASH_OK;
}

flash_status_t log_flash_iterate(log_read_fn_t block, uint32_t *end_offset)
{
	const uint32_t size = flash_size();

	uint32_t offset = 0;

	while (offset + sizeof(log_record_t) <= size) {
		log_record_t record;

		flash_status_t st = flash_read(offset, &record, sizeof(record));
		if (st != FLASH_OK) {
			return st;
		}

		if (record.event == 0xFF) {
			break; // no more written records.
		}

		const uint32_t raw_size = (uint32_t) sizeof(record) + (uint32_t) record.len;
		const uint32_t record_size = _align_up_u32(raw_size, flash_write_alignment());

		if (record_size > size - offset) {
			return FLASH_ERR_CORRUPT;
		}

		if (block != NULL) {
			uint8_t buf[LOG_FLASH_MAX_RECORD_SIZE];

			if (record.len > 0) {
				st = flash_read(offset + sizeof(record), buf, record.len);

				if (st != FLASH_OK) {
					return st;
				}
			}

			block(&record, buf);
		}

		offset += record_size;
	}

	if (end_offset != NULL) {
		*end_offset = offset;
	}

	return FLASH_OK;
}

// we only need to figure out where to start writing.
flash_status_t log_flash_init(void)
{
	return log_flash_iterate(NULL, &_write_offset);
}

flash_status_t log_flash_erase(void)
{
	flash_status_t status = flash_erase();
	if (status != FLASH_OK) {
		return status;
	}

	_write_offset = 0;

	return FLASH_OK;
}
