/*
 * Copyright (c) 2013-2015 ARM Limited. All rights reserved.
 *
 * SPDX-License-Identifier: LicenseRef-PBL
 *
 * Licensed under the Permissive Binary License, Version 1.0 (the "License"); you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * https://www.mbed.com/licenses/PBL-1.0
 *
 * See the License for the specific language governing permissions and limitations under the License.
 *
 */

#ifndef DHCP_SERVICE_API_H_
#define DHCP_SERVICE_API_H_

#include <ns_types.h>
/**
 * \file dhcp_service_api.h
 * \brief DHCP server connection interfaces
 *
 * \section dhcp-service DHCP Service Instance
 * - dhcp_service_init(), Initializes a DHCP service
 * - dhcp_service_delete(), Removes the DHCP service
 *
 * \section dhcp-msg DHCP Service Messages
 * - dhcp_service_send_req(), Sends out DHCP request messages
 * - dhcp_service_send_resp(), Sends out DHCP response messages
 *
 * \section dhcp-tim DHCP Service Timers (retry timers)
 * - dhcp_service_send_req(), Sends out DHCP request messages
 * - dhcp_service_set_retry_timers(), sets the retransmission parameters
 * - dhcp_service_req_remove(), stops retrying and retransmissions
 * - dhcp_service_timer_tick(), indicates if a timeout occurred
 *
 */

/** Defines Debug Trace String for DHCP service */
#define DHCP_SERVICE_API_TRACE_STR "DHcS"

/*
 * Return values for callbacks
 */

/** If returned msg belongs to someone else */
#define RET_MSG_NOT_MINE 0
/** If returned msg is handled */
#define RET_MSG_ACCEPTED 1
/** If returned msg is not the final one and need to hold on a bit */
#define RET_MSG_WAIT_ANOTHER -1
/** If returned msg is unexpected or corrupted */
#define RET_MSG_CORRUPTED -2

/**
 * \brief DHCP Service receive callback.
 *
 * When dhcp service receives dhcp message it will go through list of registered DHCP services instances
 * until some instance acknowledges that message belongs to it.
 * @param instance_id Instance of registered server.
 * @param msg_tr_id Message transaction id.
 * @param msg_ptr Allocated message pointer should not deallocate unless returned RET_MSG_ACCEPTED then responsibility of client.
 * @param msg_len Length of message.
 *
 * Return values
 * RET_MSG_ACCEPTED - Message is handled
 * RET_MSG_CORRUPTED - Message is corrupted
 * RET_MSG_NOT_MINE - Message belongs to someone else.
 */

typedef int (dhcp_service_receive_req_cb)(uint16_t instance_id, uint32_t msg_tr_id, uint8_t msg_name, uint8_t *msg_ptr, uint16_t msg_len);

/**
 * \brief DHCP Service Message Response callback.
 *
 * When dhcp service receives a response to a dhcp message, this callback receives it.
 *
 * @param instance_id Instance of registered server.
 * @param ptr pointer for client opbject.
 * @param msg_ptr Allocated message pointer should not deallocate unless returned RET_MSG_ACCEPTED then responsibility of client.
 * @param msg_len Length of message.
 *
 * Return values
 * RET_MSG_ACCEPTED - Message is handled
 * RET_MSG_WAIT_ANOTHER - this message was not last one for this transaction and new reply is waited.
 */

typedef int (dhcp_service_receive_resp_cb)(uint16_t instance_id, void *ptr, uint8_t msg_name,  uint8_t *msg_ptr, uint16_t msg_len);


/**
 * \brief Initialize server instance.
 *
 * Creates and shares socket for other dhcp services.
 *
 * \return instance id that is used to identify service.
 */

uint16_t dhcp_service_init(int8_t interface_id, dhcp_service_receive_req_cb *receive_req_cb);

/**
* \brief Deletes server instance.
*
* Removes all data related to this instance
*
* \param instance id of registered server.
*/
void dhcp_service_delete(uint16_t instance);

/**
* \brief Sends DHCP response message.
*
* @param msg_tr_id Message transaction id.
* @param options Options for this request
* @param msg_ptr Allocated message pointer should not deallocate unless returned RET_MSG_ACCEPTED then responsibility of client.
* @param msg_len Length of message.
*
* \return 0, if everything went fine.
* \return -1, if error occurred.
*/
int dhcp_service_send_resp(uint32_t msg_tr_id, uint8_t options, uint8_t *msg_ptr, uint16_t msg_len);


/**
 * \ Sends DHCP request message.
 *
 * Service takes care of retransmissions.
 *
 * @param instance_id Instance of registered server.
 * @param options Options for this request
 * @param ptr void pointer to client object
 * @param addr address of the server.
 * @param msg_ptr Allocated message pointer this pointer is the responsibility of service after this call.
 * @param msg_len Length of message.
 *
 * \return message transaction id of dhcp transaction
 * \return 0, if error happened.
 */
uint32_t dhcp_service_send_req(uint16_t instance_id, uint8_t options, void *ptr, const uint8_t addr[static 16], uint8_t *msg_ptr, uint16_t msg_len, dhcp_service_receive_resp_cb *receive_resp_cb);

/**
 * \brief Setting retransmission parameters
 *
 * Sets the retransmission parameters for this transaction.
 *
 * @param msg_tr_id Message transaction id.
 * @param timeout_init Initial timeout value.
 * @param timeout_max Maximum timeout value when initial timeout is doubled with every retry.
 * @param retrans_max Maximum retry count after which error is received.
 *
 */
void dhcp_service_set_retry_timers(uint32_t msg_tr_id, uint16_t timeout_init, uint16_t timeout_max, uint8_t retrans_max);

/**
 * \brief Stops transactions for a message (retransmissions).
 *
 * Clears off sending retransmissions for a particular message transaction by finding it via its message transaction id.
 *
 * @param msg_tr_id Message transaction id.
 *
 */
void dhcp_service_req_remove(uint32_t msg_tr_id);

#define TX_OPT_NONE 0x00
#define TX_OPT_USE_SHORT_ADDR 0x01
#define TX_OPT_MULTICAST_HOP_LIMIT_64   0x02

/**
 * \brief Timer tick function for retransmissions.
 *
 * should be called in 100ms intervals if more time passes before call amount in 100ms units.
 *
 */
bool dhcp_service_timer_tick(uint16_t ticks);


#endif //DHCP_SERVICE_API_H_
