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
/**
 * \file net_thread_test.h
 * \brief Thread Library Test API.
 *
 */

#ifndef NET_THREAD_TEST_H_
#define NET_THREAD_TEST_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ns_types.h"

/**
  * \brief Push Test Router ID to Leader
  *
  * Test API: add preconfigured Router ID selected device
  *
  * \param interface_id Interface to attach to
  * \param euid64 Allocated EUID64
  * \param router_id Define Router ID(0-62)

  *
  * \return  0 Success
  * \return <0 Failure
  *
  */
int_fast8_t arm_nwk_6lowpan_thread_test_router_id_push(
    int8_t interface_id,
    const uint8_t *euid64,
    uint8_t router_id);

/**
  * \brief Add a static neighbour
  *
  * Test API: Add a neighbour. Adds, or updates, a non-expiring neighbour to the
  * Thread Link Set, and updates the Route Set accordingly. Basically equivalent
  * to receiving an MLE advertisement with a Route TLV, except it doesn't expire.
  *
  * See draft-kelsey-thread-routing-00 for more info on data format.
  *
  * \param interface_id Interface to modify
  * \param neighbour_addr Neighbour short address (must be a router address)
  * \param link_margin_db Notional link margin for reception from this neighbour.
  * \param id_sequence Sequence number for id_mask
  * \param id_mask Bit mask of router IDs
  * \param route_data One octet per set bit in id_mask
  *
  * \return  0 Success
  * \return <0 Failure
  */
int_fast8_t arm_nwk_6lowpan_thread_test_add_neighbour(
    int8_t interface_id,
    uint16_t neighbour_short_addr,
    uint_fast8_t link_margin_db,
    uint8_t id_sequence,
    const uint8_t *id_mask,
    const uint8_t *route_data);

/**
  * \brief Remove a static neighbour
  *
  * Test API: Remove a neighbour. Remove a neighbour from the Thread Link Set
  * (whether added through arm_nwk_6lowpan_thread_test_add_neighbour, or
  * automatically through MLE).
  *
  * \param interface_id Interface to modify
  * \param neighbour_addr Neighbour short address (must be a router address)
  *
  * \return  0 Success
  * \return -1 Not an existing neighbour
  * \return <-1 Other errors
  */
int_fast8_t arm_nwk_6lowpan_thread_test_remove_neighbour(
    int8_t interface_id,
    uint16_t neighbour_short_addr);

/**
  * \brief Set up address filtering
  *
  * Test API: Supply a list of valid addresses. The passed lists must be
  * static - they won't be copied. If a list is supplied, a src address of
  * that size be on it, or the incoming packet will be dropped
  *
  * \param interface_id Interface to modify
  * \param num_short_addrs Number of short addresses
  * \param valid_short_addrs Valid short addresses
  * \param num_long_addrs Number of long addresses
  * \param valid_long_addrs Valid long addresses
  *
  * \return  0 Success
  * \return <0 Failure
  */
int_fast8_t arm_nwk_6lowpan_thread_test_set_addr_filter(
    int8_t interface_id,
    uint8_t num_short_addrs,
    const uint16_t valid_short_addrs[num_short_addrs],
    uint8_t num_long_addrs,
    const uint8_t valid_long_addrs[num_long_addrs][8]);

/**
  * \brief Artificially adjust link margins.
  *
  * Test API: Adjust reported link margins by a
  * specified number of decibels. Acts as if you
  * adjusted the sensitivity of your receiver.
  *
  * This would normally be used with a negative number
  * to make reported signal strength lower, to make it
  * easier to trigger routing changes.
  *
  * \param interface_id Interface to modify
  * \param adjustment link margin adjustment in dB.
  *
  * \return  0 Success
  * \return <0 Failure
  */
int_fast8_t arm_nwk_6lowpan_thread_adjust_link_margins(
    int8_t interface_id,
    int8_t adjustment);


/**
  * \brief Print routing database
  *
  * Test API: Dump the routing data base (Router ID Set, Link Set and Route
  * Set) to the debug channel.
  *
  * \param interface_id Interface to interrogate
  */
void arm_nwk_6lowpan_thread_test_print_routing_database(int8_t interface_id);

/**** RCC 20140918: MLE Test interface ****/
extern uint8_t mle_tlv_request(uint16_t addr, uint8_t req_tlv);

int8_t arm_nwk_key_update_trig(int8_t interface_id);
/**
  * \brief Set Thread network id timeout
  *
  * \param interface_id Interface to modify
  * \param network_id_timeout network timeout as seconds
  *
  * \return  0 Success
  * \return -1 Invalid interface id
  * \return -2 Thread not active
  * \return -3 invalid timeout value
  */
int8_t thread_routing_set_network_id_timeout(int8_t interface_id, uint16_t network_id_timeout);
/**
  * \brief Get Thread network id timeout
  *
  * \param interface_id Interface to modify
  * \param network_id_timeout pointer, where current network id timeout will be saved as seconds
  *
  * \return  0 Success
  * \return -1 Invalid interface id
  * \return -2 Thread not active
  * \return -3 invalid pointer
  */
int8_t thread_routing_get_network_id_timeout(int8_t interface_id, uint16_t *network_id_timeout);

#ifdef __cplusplus
}
#endif

#endif /* NET_THREAD_TEST_H_ */
