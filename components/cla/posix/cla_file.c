#include "cla/mtcp_proto.h"
#include "cla/posix/cla_file.h"

#include "platform/hal_config.h"
#include "platform/hal_io.h"
#include "platform/hal_queue.h"
#include "platform/hal_semaphore.h"

#include "upcn/bundle_agent_interface.h"
#include "upcn/common.h"
#include "upcn/config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Configuration of the file CLA
struct filecla_config {
    struct cla_config base;
};

/* Returns a unique identifier of the CLA as part of the CLA address */
static const char *filecla_name_get(void)
{
	return "file";
}

/* Starts the TX/RX tasks and, e.g., the socket listener */
static enum upcn_result filecla_launch(struct cla_config *const config)
{
	//TODO Launch CLA
	return UPCN_OK;
}

/* Obtains the max. serialized size of outgoing bundles for this CLA. */
static size_t filecla_mbs_get(struct cla_config *const config)
{
	(void)config;
	return SIZE_MAX;
}

/* Returns the transmission queue for the given node EID and address */
static struct cla_tx_queue filecla_get_tx_queue(
	struct cla_config *config, const char *eid, const char *cla_addr)
{
	(void)eid;
	(void)cla_addr;
	(void)config;
	
	//TODO Create tx communication queue

	return (struct cla_tx_queue){
		.tx_queue_handle = NULL,
		.tx_queue_sem = NULL,
	};
}

/* Initiates a scheduled contact for a given EID and CLA address */
static enum upcn_result filecla_start_scheduled_contact(
	struct cla_config *config, const char *eid, const char *cla_addr)
{
	(void)config;
	(void)eid;
	(void)cla_addr;

	//TODO Load folder and create link

	return UPCN_OK;
}

/* Ends a scheduled contact for a given EID and CLA address */
static enum upcn_result filecla_end_scheduled_contact(
	struct cla_config *config, const char *eid, const char *cla_addr)
{
	(void)config;
	(void)eid;
	(void)cla_addr;

	//TODO Remove folder watching for new bundles

	return UPCN_OK;
}

/* Initiates bundle transmission for a single bundle */
static void filecla_begin_packet(struct cla_link *link, size_t length)
{
	//TODO
}

/* Terminates bundle transmission for a single bundle */
static void filecla_end_packet(struct cla_link *link)
{
	// STUB
	(void)link;
}

/* Sends part of the serialized bundle. Can be called multiple times. */
static void filecla_send_packet_data(
	struct cla_link *link, const void *data, const size_t length)
{
	//TODO
}

void filecla_reset_parsers(struct cla_link *link)
{
	//TODO
}

static size_t filecla_forward_to_specific_parser(struct cla_link *link,
						const uint8_t *buffer,
						size_t length)
{
	//TODO

	return 0;
}

/* Reads a chunk of data */
static enum upcn_result filecla_read(struct cla_link *link,
				    uint8_t *buffer, size_t length,
				    size_t *bytes_read)
{
	//TODO

	return UPCN_OK;
}

/* Cleans up resources after a link broke */
static void filecla_disconnect_handler(struct cla_link *link)
{
	(void)link;
	ASSERT(0);
}


// Vtable of methods used for communication with this CLA
const struct cla_vtable filecla_vtable = {
	.cla_name_get = filecla_name_get,
	.cla_launch = filecla_launch,
	.cla_mbs_get = filecla_mbs_get,

	.cla_get_tx_queue = filecla_get_tx_queue,
	.cla_start_scheduled_contact = filecla_start_scheduled_contact,
	.cla_end_scheduled_contact = filecla_end_scheduled_contact,

	.cla_begin_packet = filecla_begin_packet,
	.cla_end_packet = filecla_end_packet,
	.cla_send_packet_data = filecla_send_packet_data,

	.cla_rx_task_reset_parsers = filecla_reset_parsers,
	.cla_rx_task_forward_to_specific_parser =
			filecla_forward_to_specific_parser,

	.cla_read = filecla_read,

	.cla_disconnect_handler = filecla_disconnect_handler,
};

// Entry point of cla creation and configuration
struct cla_config *filecla_create(
	const char *const options[], const size_t option_count,
	const struct bundle_agent_interface *bundle_agent_interface)
{
	(void)options;
	(void)option_count;

	struct filecla_config *config = malloc(sizeof(struct filecla_config));

	if (!config) {
		LOG("FileCLA: Memory allocation failed!");
		return NULL;
	}
	cla_config_init(&config->base, bundle_agent_interface);
    
	config->base.vtable = &filecla_vtable;

	return &config->base;
}
