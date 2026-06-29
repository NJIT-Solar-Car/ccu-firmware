/*
 * log.c
 *
 *  Created on: Jun 23, 2026
 *      Author: marko
 */

#include "FreeRTOS.h"
#include "message_buffer.h"
#include "task.h"

#include <string.h>

#include "Log/log.h"
#include "log_flash.h"

#define LOG_BUFFER_SIZE_BYTES 4096
#define LOG_TASK_STACK_WORDS  512u
#define LOG_TASK_PRIORITY     (tskIDLE_PRIORITY + 1u)

#define LOG_MAX_PAYLOAD_SIZE  256u
#define LOG_MAX_MESSAGE_SIZE  (sizeof(log_record_t) + LOG_MAX_PAYLOAD_SIZE)

static uint8_t log_buffer_storage[LOG_BUFFER_SIZE_BYTES];
static StaticMessageBuffer_t log_message_buffer_struct;
static MessageBufferHandle_t log_message_buffer;

static StaticTask_t log_task_tcb;
static StackType_t log_task_stack[LOG_TASK_STACK_WORDS];

static void log_task(void *arg);

void log_init() {
	// init buffer
	log_message_buffer = xMessageBufferCreateStatic(
		LOG_BUFFER_SIZE_BYTES,
		log_buffer_storage,
		&log_message_buffer_struct
	);

	// flash
	log_flash_init();

	// task
	xTaskCreateStatic(
		log_task,
		"log",
		LOG_TASK_STACK_WORDS,
		NULL,
		LOG_TASK_PRIORITY,
		log_task_stack,
		&log_task_tcb
	);
}

void log_info(log_event_t event, const void *payload, uint16_t len)
{
	uint8_t msg[LOG_MAX_MESSAGE_SIZE];

	log_record_t record = {
		.event = (uint16_t) event,
		.timestamp_ms = xTaskGetTickCount(),
		.len = len
	};

	memcpy(msg, &record, sizeof(record));

	if (len != 0u) {
		memcpy(msg + sizeof(record), payload, len);
	}

	xMessageBufferSend(
		log_message_buffer,
		msg,
		sizeof(record) + len,
		0
	);
}

static void log_task(void *arg)
{
	uint8_t msg[LOG_MAX_MESSAGE_SIZE];

	for (;;) {
		size_t n = xMessageBufferReceive(
			log_message_buffer,
			msg,
			sizeof(msg),
			portMAX_DELAY
		);

		if (n < sizeof(log_record_t)) {
			continue;
		}

		const log_record_t *record = (const log_record_t *) msg;

		if ((record->len > LOG_MAX_PAYLOAD_SIZE) || (n != sizeof(*record) + record->len)) {
			continue;
		}

		const void *payload = msg + sizeof(*record);

		log_flash_append(record, payload);
	}
}

void log_read_all(log_read_fn_t block)
{
	log_flash_iterate(block, NULL);
}

