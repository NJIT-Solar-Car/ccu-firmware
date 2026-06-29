/**
 * @file log.h
 * @brief
 * @author Marco Sitar
 */
#ifndef LOG_H_
#define LOG_H_

#include <stdint.h>

#include "log_events.h"

typedef struct {
	log_event_t event;
	uint32_t timestamp_ms;
	uint16_t len;
} log_record_t;

void log_init();

void log_info(log_event_t event, const void *payload, uint16_t len);

typedef void (*log_read_fn_t)(
    const log_record_t *record,
    const void *payload
);

void log_read_all(log_read_fn_t block);

#endif /* LOG_H_ */
