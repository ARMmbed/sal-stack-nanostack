/*
 * Copyright (c) 2015 ARM. All rights reserved.
 */

/**Public API for managing thread network configuration.
 *
 * This interface enables modification and reading the thread network parameters.
 *
 * Thread management can modify the following values from the thread network
 * * [Network Name] (Read/write)
 * * [Security Policy] (Read/write)
 * * [Steering Data] (Read/write)
 * * [Commissioning Data Timestamp] (Read/write)
 * * [Commissioning Credential] PSKc (Write only)
 * * [Network Master Key] (Read only when policy allows)
 * * [Network Key Sequence] (Read only when policy allows)
 * * [Network Mesh-Local ULA] (Read only)
 * * [Border Router Locator] (Read only)
 * * [Commissioner Session ID] (Read only)
 * * [XPANID] (Read only)
 * * [PANID] (Read only)
 * * [Channel] (Read only)
 */

#ifndef THREAD_MANAGEMENT_API_H_
#define THREAD_MANAGEMENT_API_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ns_types.h"

#define TRACE_GROUP_THREAD_MANAGEMENT_API "TMaA"

/** Thread management state
 */
typedef enum {
    MANAGEMENT_STATE_REJECT,
    MANAGEMENT_STATE_PENDING,
    MANAGEMENT_STATE_ACCEPT
} management_state_e;

/** Register thread management interface
 *
 * \param interface_id Interface id of the thread network.
 *
 */
int thread_management_register(int8_t interface_id);

/** Unregister thread management interface
 *
 * \param interface_id Interface id of the thread network.
 *
 */
int thread_management_unregister(int8_t interface_id);

/** Callback to inform the result of management set command.
 *
 * This callback is called when the server responds the status of management set command.
 * This can fail if leader rejects our request or the request times out.
 *
 * \param interface_id Interface id where the request was made.
 * \param status result status of the request.
 *
 */
typedef int (management_set_response_cb)(int8_t interface_id, management_state_e status);

/** Set the thread network name
 *
 * \param interface_id Interface id where the request was made.
 * \param value_ptr Pointer to new network name.
 * \param length length of the new network name.
 * \param cb_ptr callback function to inform the result of the operation. Can be NULL if no result code needed.
 *
 */
int thread_management_set_network_name(int8_t interface_id, uint8_t *name_ptr, uint8_t name_len, management_set_response_cb *cb_ptr);

/** Set the thread security policy
 *
 * \param interface_id Interface id where the request was made.
 * \param options security policy options
 *    bit 8 Out-of-band commissioning restricted.
 *    bit 7 Native commissioner restricted.
 * \param rotation_time Thread key rotation time in hours.
 * \param cb_ptr callback function to inform the result of the operation. Can be NULL if no result code needed.
 *
 */
int thread_management_set_security_policy(int8_t interface_id, uint8_t options, uint16_t rotation_time, management_set_response_cb *cb_ptr);

/** Set the Steering data
 *
 * Steering data can be either:
 *  - Empty to disallow joining.
 *  - Any length with all bits set to 0 to disallow joining.
 *  - Any length with all bits 1 to allow anyone to join.
 *  - Bloom filter to guide which device can join.
 *
 * if there is Bloom filter present it can be any length between 1-16 bytes. First bit of Bloom
 * filter indicates whether to use 0 == EUI-64 or 1 == bottom 24 bits of EUI-64.
 *
 * \param interface_id Interface id where the request was made.
 * \param steering_data_ptr Pointer to new steering data.
 * \param steering_data_len length of the new steering data.
 * \param cb_ptr callback function to inform the result of the operation. Can be NULL if no result code needed.
 *
 */
int thread_management_set_steering_data(int8_t interface_id, uint8_t *steering_data_ptr, uint8_t steering_data_len, management_set_response_cb *cb_ptr);

/** Set the thread commissioning data timestamp
 *
 * \param interface_id Interface id where the request was made.
 * \param time upper 48 bit is timestamp in seconds since start of unix time lower 16 bits are fractional portion of time. if Last bit is set to 1 then time is accurate and others shall follow it
 * \param cb_ptr callback function to inform the result of the operation. Can be NULL if no result code needed.
 *
 */
int thread_management_set_commissioning_data_timestamp(int8_t interface_id, uint64_t time, management_set_response_cb *cb_ptr);

/** Set commissioning credentials
 *
 * \param interface_id Interface id where the request was made.
 * \param PSKc commissioning credentials to set up on the network.
 * \param PSKc_len commissioning credentials length.
 * \param cb_ptr callback function to inform the result of the operation. Can be NULL if no result code needed.
 *
 */
int thread_management_set_commissioning_credentials(int8_t interface_id, uint8_t PSKc[32], uint8_t PSKc_len, management_set_response_cb *cb_ptr);

/** Callback for reading thread management information
 *
 * Result message containing Thread management TLV message.
 * parsing of this message is done using thread_meshcop_lib.h
 *
 * \param interface_id Interface id where the request was made.
 * \param status result status of the request.
 * \param response_message_ptr Meshcop TLV structure pointer which has the asked TLVs
 * \param response_message_len Length of message
 *
 */
typedef int (management_get_response_cb)(int8_t interface_id, management_state_e status, uint8_t *response_message_ptr, uint16_t response_message_len);

/** Get thread management fields.
 *
 * Read thread management field values from the leader of the thread network.
 *
 * \param interface_id Interface id where the request was made.
 * \param fields_ptr fields wanted to read from the leader from the network. Array of MESHCOP TLV defines from thread_meshcop_lib.h
 * \param fields_len count of fields in the fields_ptr array.
 * \param cb_ptr callback function to inform the result of the operation. Can be NULL if no result code needed.
 *
 */
int thread_management_info_get(int8_t interface_id, uint8_t *fields_ptr, uint8_t fields_count, management_get_response_cb *cb_ptr);

#ifdef __cplusplus
}
#endif
#endif /* THREAD_MANAGEMENT_API_H_ */
