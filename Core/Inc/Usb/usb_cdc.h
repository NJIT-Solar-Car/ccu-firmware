/*
 * usb.h
 *
 *  Created on: Jun 29, 2026
 *      Author: marko
 */

#ifndef USB_CDC_H_
#define USB_CDC_H_

#include <stddef.h>
#include <string.h>
#include <stdint.h>

void usb_cdc_write(void *data, size_t len);
void usb_cdc_rx_byte(uint8_t byte);

static inline void usb_cdc_write_str(char *s) { return usb_cdc_write(s, strlen(s)); }

#endif /* USB_CDC_H_ */
