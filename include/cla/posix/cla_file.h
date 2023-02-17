#ifndef CLA_FILE
#define CLA_FILE

#include "cla/cla.h"

#include "upcn/bundle_agent_interface.h"

#include <stddef.h>

struct cla_config *filecla_create(
	const char *const options[], const size_t option_count,
	const struct bundle_agent_interface *bundle_agent_interface);

#endif // CLA_FILE
