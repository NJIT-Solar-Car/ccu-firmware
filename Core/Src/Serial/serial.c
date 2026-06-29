#include "Log/dump.h"
#include "Usb/usb_cdc.h"
#include "serial.h"

#define SERIAL_LINE_MAX 32

static char line_buf[SERIAL_LINE_MAX];
static size_t line_len;

void usb_cdc_rx_byte(uint8_t byte)
{
	serial_rx_byte(byte);
}

void _serial_execute();

void serial_rx_byte(uint8_t byte)
{
	if (byte == '\r' || byte == '\n') {
		line_buf[line_len] = '\0';

		if (line_len > 0) {
			_serial_execute();
		}

		line_len = 0;
		return;
	}

	if (line_len + 1 >= SERIAL_LINE_MAX) {
		line_len = 0;
		return;
	}

	line_buf[line_len++] = (char)byte;
}

void _serial_execute() {
	if (strcmp(line_buf, "log dump") == 0) {
		log_dump_usb();
		return;
	}
}
