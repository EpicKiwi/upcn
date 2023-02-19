/*
 * bundle_retry.c
 *
 *  Created on: 4 janv. 2023
 *      Author: EpicKiwi
 */

#include "upcn/bundle_retry.h"

#include "platform/hal_io.h"
#include "platform/hal_queue.h"
#include "platform/hal_semaphore.h"
#include "platform/hal_task.h"
#include "platform/hal_time.h"
#include "platform/hal_semaphore.h"
#include "upcn/config.h"
#include <stdlib.h>

void handle_signal(const struct bundle_retry_signal signal);
void schedule_retry(bundleid_t bundleid, uint64_t timeout);
void check_retry_bundles();

struct retry_schedule {
	bundleid_t id;
	uint64_t expiration;
	struct retry_schedule *next;
};

static QueueIdentifier_t out_queue;
static QueueIdentifier_t self_queue;
static struct retry_schedule *first_retry;
static uint64_t smallest_timeout = 0;

void bundle_retry_task(void * const param)
{
	struct bundle_retry_task_parameters *p =
		(struct bundle_retry_task_parameters *)param;
	struct bundle_retry_signal signal;

	first_retry = NULL;

	out_queue = p->bundle_processing_signaling_queue;
	self_queue = p->retry_signaling_queue;

	LOG("BundleRetry: Retry task initialized");

	for (;;) {
		if (hal_queue_receive(self_queue, &signal,
			smallest_timeout > 0 ? smallest_timeout * 1000 : -1) == UPCN_OK
		) {
			handle_signal(signal);
		} else {
			check_retry_bundles();
		}
	}
}

void handle_signal(const struct bundle_retry_signal signal){
	switch(signal.type){
		case BR_SIGNAL_SCHEDULE_RETRY:
			schedule_retry(signal.bundle, BUNDLE_RETRY_TIMEOUT);
			break;
		case BR_SIGNAL_RETRY_BUNDLE:
			LOGF("BundleRetry: Retrying bundle #%d...", signal.bundle);
			break;
	}
}

void check_retry_bundles(){
	struct retry_schedule *next_schedule = first_retry;
	struct retry_schedule *prev = NULL;

	uint64_t now = hal_time_get_timestamp_s();

	smallest_timeout = 0;

	while (next_schedule != NULL)
	{
		if(next_schedule->expiration <= now){
			bundle_retry_inform(self_queue, next_schedule->id, BR_SIGNAL_RETRY_BUNDLE);

			if(prev == NULL){
				first_retry = next_schedule->next;
			} else {
				prev->next = next_schedule->next;
			}

			next_schedule = next_schedule->next;

			free(next_schedule);
		} else {

			if(smallest_timeout == 0 || next_schedule->expiration - now < smallest_timeout){
				smallest_timeout = next_schedule->expiration - now;
			}

			next_schedule = next_schedule->next;
			prev = next_schedule;
		}
	}
}

void schedule_retry(bundleid_t bundleid, uint64_t timeout){
	struct retry_schedule *schedule = malloc(sizeof(struct retry_schedule));
	ASSERT(schedule != NULL);

	schedule->id = bundleid;
	schedule->expiration = hal_time_get_timestamp_s() + timeout;
	schedule->next = first_retry;

	first_retry = schedule;

	if(smallest_timeout == 0 || timeout < smallest_timeout){
		smallest_timeout = timeout;
	}

	LOGF("BundleRetry: Scheduled bundle #%d for retry in %d seconds", bundleid, timeout);
}

/* COMMUNICATION */

void bundle_retry_inform(
	QueueIdentifier_t signaling_queue, bundleid_t bundle,
	enum bundle_retry_signal_type type)
{
	struct bundle_retry_signal signal = {
		.type = type,
		.bundle = bundle
	};

	hal_queue_push_to_back(signaling_queue, &signal);
}
