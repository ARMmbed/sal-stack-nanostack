/*
 * Copyright (c) 2013-2015 ARM Limited. All rights reserved.
 */

#ifndef DHCP_SERVICE_API_H_
#define DHCP_SERVICE_API_H_

#include <ns_types.h>
/*
 * DHCP server connection interfaces
 */

#define DHCP_SERVICE_API_TRACE_STR "DHcS"

/*
 * Return values for callbacks
 */
#define RET_MSG_NOT_MINE 0
#define RET_MSG_ACCEPTED 1
#define RET_MSG_WAIT_ANOTHER -1
#define RET_MSG_CORRUPTED -2

/*
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

/*
 * When dhcp service receives dhcp message response this message is called
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


/*
 * Initialise server instance.
 * Creates and shares socket for other dhcp services.
 *
 * Return values
 * instance id that is used to identify service.
 */
uint16_t dhcp_service_init(int8_t interface_id, dhcp_service_receive_req_cb *receive_req_cb);
/*
* Deletes server instance.
* Removes all data related to this instance
*/
void dhcp_service_delete(uint16_t instance);
/*
* Sends DHCP response message.
*
* @param instance_id Instance of registered server.
* @param msg_tr_id Message transaction id.
* @param options Options for this request
* @param msg_ptr Allocated message pointer should not deallocate unless returned RET_MSG_ACCEPTED then responsibility of client.
* @param msg_len Length of message.
*/
int dhcp_service_send_resp(uint32_t msg_tr_id, uint8_t options, uint8_t *msg_ptr, uint16_t msg_len);


/*
 * Sends DHCP request message. Service takes care of retransmissions.
 *
 * @param instance_id Instance of registered server.
 * @param options Options for this request
 * @param ptr void pointer to client object
 * @param addr address of the server.
 * @param msg_ptr Allocated message pointer this pointer is the responsibility of service after this call.
 * @param msg_len Length of message.
 *
 * return message transaction id of dhcp transaction. if 0 error happened.
 */
uint32_t dhcp_service_send_req(uint16_t instance_id, uint8_t options, void *ptr, const uint8_t addr[static 16], uint8_t *msg_ptr, uint16_t msg_len, dhcp_service_receive_resp_cb *receive_resp_cb);

/*
 * Sets the retransmission parameters for this transaction.
 *
 * @param msg_tr_id Message transaction id.
 * @param timeout_init Initial timeout value.
 * @param timeout_max Maximum timeout value when initial timeout is doubled with every retry.
 * @param retrans_max Maximum retry count after which error is received.
 *
 * return.
 */
void dhcp_service_set_retry_timers(uint32_t msg_tr_id, uint16_t timeout_init, uint16_t timeout_max, uint8_t retrans_max);

/*
 * Clears sending of retransmissions.
 *
 * @param msg_tr_id Message transaction id.
 *
 * return.
 */
void dhcp_service_req_remove(uint32_t msg_tr_id);

#define TX_OPT_NONE 0x00
#define TX_OPT_USE_SHORT_ADDR 0x01
#define TX_OPT_MULTICAST_HOP_LIMIT_64   0x02

/*
 * Timer tick function.
 *
 * should be called in 100ms intervals if more time passes before call amount in 100ms units.
 *
 */
bool dhcp_service_timer_tick(uint16_t ticks);


#endif //DHCP_SERVICE_API_H_
