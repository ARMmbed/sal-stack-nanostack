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
 * \file br_mle_lib.h
 * \brief Border Router MLE (Mesh Link Establishment) API (wireless interface running).
 *
 * - arm_mle_prepare_update(), Update MLE link parameter (Channel, PAN-ID, allow-join) at run time.
 * - arm_mle_update_beacon_payload(), Update Beacon payload optional data part.
 * - arm_mle_link_request_to_routers(), Load MLE entry from NVM.
 *
 * The recommended minimum delay for the MLE update process is >5000 ms.
 * PAN-ID and Channel update cause problems to the sleepy host nodes.
 * They never get the multicast packet from the parent and loose the parent.
 *
 * MLE link update messages are delivered by trickle multicast forwarding using multicast address FF03::1.
 *
 */
#ifndef _BR_MLE_LIB_H
#define _BR_MLE_LIB_H

#include "ns_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Supported MLE Link layer Parameters changes at run time*/
#define MLE_CHANNEL_UPDATE_TYPE 0       /* Update new link layer channel. */
#define MLE_PANID_UPDATE_TYPE 1         /* Update new link layer PAN ID. */
#define MLE_ALLOW_JOIN_UPDATE_TYPE 2    /* Update Allow Join Bit at beacon payload. */

/**
 * \brief Update MLE link parameter at run time
 *
 * \param nwk_id Network interface ID.
 * \param type MLE_CHANNEL_UPDATE_TYPE, MLE_PANID_UPDATE_TYPE or MLE_ALLOW_JOIN_UPDATE_TYPE.
 * \param time Update interval needs to be 5000 milliseconds or more.
 * \param value Indicate channel, PAN ID or boolean value for allow join.
 *
 * \return 0    Update process OK.
 * \return -1   Old update process still active.
 * \return -2   Unsupported update type.
 * \return -3   Unsupported value (channel not supported or PAN ID is 0xffff or 0xfffe).
 * \return -4   Stack in idle state.
 *
 * List Possible values for specific types:
 * - MLE_CHANNEL_UPDATE_TYPE, Supported values 11-26.
 * - MLE_PAN_ID_UPDATE_TYPE, supported value 0x0000-0xfffd.
 * - MLE_ALLOW_JOIN__UPDATE_TYPE, 16-bit boolean, 0=Disabled and 1=Enabled
 *
 *
 */
extern int8_t arm_mle_prepare_update(int8_t nwk_id, uint8_t type, uint32_t time, uint16_t value);
/**
 * \brief Update beacon payload optional field
 *
 * \param nwk_id Network interface ID.
 * \param optional_fields_data Pointer to given optional data.
 * \param optional_fields_len Length of optional data.
 * \param delay Update interval needs to be 5000 milliseconds or more.
 *
 * \return 0    Update process OK.
 * \return -1   Old update process still active or memory allocation failure.
 * \return -2   Unsupported update type.
 * \return -4   Stack in idle state.
 *
 */
extern int8_t arm_mle_update_beacon_payload(int8_t nwk_id, uint8_t *optional_fields_data, uint8_t optional_fields_len, uint32_t delay);
/**
 * \brief Send multicast MLE link request to all routers
 *
 *  This function should call when Border Router bootup by loaded NVM setups.
 *
 * \param nwk_id Network interface ID
 *
 * \return 0    Update process OK.
 * \return -1   Memory allocation for the packet fails.
 *
 */
extern int8_t arm_mle_link_request_to_routers(int8_t nwk_id);
#ifdef __cplusplus
}
#endif
#endif /*_BR_MLE_LIB_H*/
