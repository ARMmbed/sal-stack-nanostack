/*
 * Copyright (c) 2014-2015 ARM Limited. All rights reserved.
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

#ifndef THREAD_TMF_DATA_IF_H_
#define THREAD_TMF_DATA_IF_H_

#include "ns_types.h"

/**
 * Update network data from TLV
 *
 * Returns the accepted TLV in pointer and updates the length. there can never be more than what was added
 * and some of them can be removed if leader does not accept them.
 *
 * \param interface_id Network Interface
 * \param network_data_ptr pointer to new network data TLV
 * \param network_data_length length of new network data TLV
 * \param routerId ULA16 Router ID
 * return 0, OK
 * return -1 network data corrupted
 * return -2 Not Leader
 * return -3 Not enough room
 */
int thread_tmf_network_data_register(int8_t interface_id, uint8_t *network_data_ptr, uint8_t *network_data_length, uint16_t routerId);

/**
 * Post network data to leader
 *
 * Event which is sent to TMF tasklet when new service is registered to leader.
 * When sending event event_id field is set to TMF_POST_SERVICE_TO_LEADER
 *
 * TODO This event will be obsoleted when tmf_app_if is created
 *
 * \param src_address Address of the leader
 * \param data_ptr Data pointer to network data posted to leader
 * \param data_len length of network data.
 *
 */
#define TMF_POST_SERVICE_TO_LEADER 1
typedef struct {
    uint8_t address[16];
    uint8_t *data_ptr;
    uint8_t data_len;
} tmf_post_to_leader;

#endif /* THREAD_TMF_DATA_IF_H_ */
