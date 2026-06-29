#include "Telemetry/telemetry.h"
#include "Log/log.h"

void telemetry_publish(const telemetry_t *telemetry) {
	log_info(LOG_EVENT_TELEMETRY, telemetry, sizeof(*telemetry));
	// lora
}
