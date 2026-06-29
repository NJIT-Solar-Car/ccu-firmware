/*
 * dump.c
 *
 *  Created on: Jun 29, 2026
 *      Author: marko
 */

#include <stdio.h>

#include "Log/log.h"
#include "Log/dump.h"
#include "Usb/usb_cdc.h"

static void _record_dump_usb(const log_record_t *record, const void *payload)
{
	char line[64];

	int n = snprintf(
		line,
		sizeof line,
		"%lu,%u,%u,",
		(unsigned long)record->timestamp_ms,
		(unsigned)record->event,
		(unsigned)record->len
	);

	usb_cdc_write(line, (size_t ) n);

	static const char lut[] = "0123456789ABCDEF";

	const uint8_t *bytes = payload;
	for (uint16_t i = 0; i < record->len; ++i) {
		char hex[2];

		hex[0] = lut[bytes[i] >> 4];
		hex[1] = lut[bytes[i] & 0x0F];
		usb_cdc_write(hex, 2);
	}

	usb_cdc_write_str("\r\n");
}

void log_dump_usb()
{
	usb_cdc_write_str("timestamp_ms,event,len,payload\r\n");
	log_read_all(_record_dump_usb);
}
