/*
 * telemetry.h
 *
 *  Created on: Jun 29, 2026
 *      Author: marko
 */

#ifndef TELEMETRY_H_
#define TELEMETRY_H_

#include <stdint.h>

typedef struct {
	uint32_t timestamp_ms;
} telemetry_t;

void telemetry_publish(const telemetry_t *telemetry);

#endif /* TELEMETRY_H_ */
