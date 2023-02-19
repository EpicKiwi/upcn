/*
 * bundle_retry.h
 *
 *  Created on: 4 janv. 2023
 *      Author: EpicKiwi
 */

#include "platform/hal_types.h"
#include "upcn/bundle.h"

#ifndef INCLUDE_UPCN_BUNDLE_RETRY_H_
#define INCLUDE_UPCN_BUNDLE_RETRY_H_

enum bundle_retry_signal_type {
	BR_SIGNAL_SCHEDULE_RETRY,
	BR_SIGNAL_RETRY_BUNDLE
};

struct bundle_retry_signal {
	enum bundle_retry_signal_type type;
	bundleid_t bundle;
};

struct bundle_retry_task_parameters {
	QueueIdentifier_t retry_signaling_queue;
	QueueIdentifier_t bundle_processing_signaling_queue;
};

void bundle_retry_inform(
	QueueIdentifier_t signaling_queue, bundleid_t bundle,
	enum bundle_retry_signal_type type);

void bundle_retry_task(void * const param);

#endif /* INCLUDE_UPCN_BUNDLE_RETRY_H_ */
