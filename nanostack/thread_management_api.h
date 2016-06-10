/*
 * Copyright (c) 2015 ARM Limited. All rights reserved.
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

/**
 * \file thread_management_api.h
 * \brief Public API for managing Thread network configuration.
 *
 * This interface enables modification and reading the Thread network parameters.
 *
 * Thread management can modify the following values in the Thread network:
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

/** \brief Register Thread management interface
 *
 * \param interface_id Interface ID of the Thread network.
 *
 * \return A handle for the management interface.
 *
 */
int thread_management_register(int8_t interface_id);

/** \brief Unregister Thread management interface
 *
 * \param instance_id ID of the management session.
 *
 */
int thread_management_unregister(int8_t instance_id);

/** \brief Callback to display the result of management set command.
 *
 * This callback is called when the server responds to the management set command.
 * This can fail if the leader rejects the request or the request times out.
 *
 * \param interface_id The interface ID of the Thread network.
 * \param status Result of the request.
 *
 */
typedef int (management_set_response_cb)(int8_t interface_id, management_state_e status);

 /** \brief Set the Thread security policy
 *
 * \param instance_id The ID of the management session.
 * \param options Security policy options:
 *    bit 8 Out-of-band commissioning restricted.
 *    bit 7 Native commissioner restricted.
 * \param rotation_time Thread key rotation time in hours.
 * \param cb_ptr A callback function indicating the result of the operation. Can be NULL if no result code needed.
 *
 * \return 0 Success.
 * \return <0 Fail.
 */
int thread_management_set_security_policy(int8_t instance_id, uint8_t options, uint16_t rotation_time, management_set_response_cb *cb_ptr);

/** \brief Set the steering data
 *
 * Steering data can be either:
 *  - Empty to decline joining.
 *  - Any length with all bits set to 0 to decline joining.
 *  - Any length with all bits 1 to allow anyone to join.
 *  - Bloom filter to guide which device can join.
 *
 * If a Bloom filter is present it can be any length between 1-16 bytes. The first bit of the Bloom
 * filter indicates whether to use 0 == EUI-64 or 1 == bottom 24 bits of EUI-64.
 *
 * \param instance_id The ID of the management session.
 * \param session_id The commissioning session id that needs to be added
 * \param steering_data_ptr A pointer to new steering data.
 * \param steering_data_len The length of the new steering data.
 * \param cb_ptr A callback function indicating the result of the operation. Can be NULL if no result code needed.
 *
 * \return 0 Success.
 * \return <0 Fail.
 */
int thread_management_set_steering_data(int8_t instance_id, uint16_t session_id, uint8_t *steering_data_ptr, uint8_t steering_data_len, management_set_response_cb *cb_ptr);

/** \brief Set the Thread commissioning data timestamp
 *
 * \param instance_id the ID of the management session.
 * \param time Upper 48 bits is the timestamp in seconds since the start of unix time, lower 16 bits are fractional portion of time. If the last bit is set to 1, the commissioner has accurate time.
 * \param cb_ptr A callback function indicating the result of the operation. Can be NULL if no result code needed.
 *
 * \return 0 Success.
 * \return <0 Fail.
 */
int thread_management_set_commissioning_data_timestamp(int8_t instance_id, uint64_t time, management_set_response_cb *cb_ptr);

/** \brief Callback for reading Thread management information
 *
 * Result message containing the Thread management TLV message.
 * This message can be parsed with thread_meshcop_lib.h.
 *
 * \param instance_id The ID of the management session.
 * \param status Result of the request.
 * \param response_message_ptr A meshcop TLV structure pointer containing the requested TLVs.
 * \param response_message_len The length of the message.
 *
 * \return 0 Success.
 * \return <0 Fail.
 */
typedef int (management_get_response_cb)(int8_t instance_id, management_state_e status, uint8_t *response_message_ptr, uint16_t response_message_len);

/** \brief Get Thread management fields.
 * Comments from PEKKA: please rewrite the parameter descriptions below.
 * Read Thread management field values from the leader of the Thread network.
 *
 * \param interface_id The interface ID where the request was made.
 * \param dst_addr Destination address for remote if address is not given sent to leader of network or if native commissioner sent to Border router.
 * \param uri_ptr The ASCII string for the URI. Can be Active (default if NULL)/Pending/Commissioner URI.
 * \param fields_ptr The fields to be read from the leader from the network. Array of MESHCOP TLV defines from thread_meshcop_lib.h
 * \param fields_len count of fields in the fields_ptr array.
 * \param cb_ptr callback function to inform the result of the operation. Can be NULL if no result code needed.
 *
 * \return 0, Success.
 * \return <0 Fail.
 */
int thread_management_get(int8_t instance_id, uint8_t dst_addr[static 16], char *uri_ptr, uint8_t *fields_ptr, uint8_t fields_count, management_get_response_cb *cb_ptr);

/** \brief Set Thread management fields
 *
 * Set Thread management field values to the leader of the Thread network.
 *
 * \param interface_id Interface id where the request was made.
 * \param dst_addr Destination address for remote if address is not given sent to leader of network or if native commissioner sent to Border router.
 * \param uri_ptr Ascii string for the URI. Can be Active(default if NULL)/Pending/Commissioner URI.
 * \param data_ptr fields wanted to set to the leader. Array of MESHCOP TLV defines from thread_meshcop_lib.h
 * \param data_len length of data in the data_ptr.
 * \param cb_ptr callback function to inform the result of the operation. Can be NULL if no result code needed.
 *
 */
int thread_management_set(int8_t instance_id, uint8_t dst_addr[static 16], char *uri_ptr, uint8_t *data_ptr, uint8_t data_len, management_set_response_cb *cb_ptr);

#ifdef __cplusplus
}
#endif
#endif /* THREAD_MANAGEMENT_API_H_ */
