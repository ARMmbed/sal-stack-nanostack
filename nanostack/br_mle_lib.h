/* Permissive Binary License
 * Redistribution.  Redistribution and use in binary form, without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * * Redistributions must reproduce the above copyright notice and the
 * following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 * * No reverse engineering, decompilation, or disassembly of this software
 * is permitted.
 * * In case of redistribution as part of a development kit, the
 * accompanying DEPENDENCIES file, including all dependencies specified
 * therein, are included in the development kit.
 */
/*
 * Copyright (c) 2014-2015 ARM Limited. All rights reserved.
 */
/**
 * \file br_mle_lib.h
 * \brief Border Router MLE (Mesh Link Establishment) API when wireless interface is running.
 *
 * - arm_mle_prepare_update(), Update MLE link parameter(Channel, PAN-ID, allow-join) at Run time
 * - arm_mle_update_beacon_payload(), Update Beacon payload optional data part
 * - arm_mle_link_request_to_routers(), load MLE entry from NVM
 *
 * MLE update process recommend minimum delay should be >5000 ms.
 * PAN-ID and Channel update cause problems to sleepy Host Node.
 * Those nodes never get Multicast packet from parent and loose parent.
 *
 * MLE link update messages will be delivered by trickle multicast forwarding using multicast address FF03::1.
 *
 */
#ifndef _BR_MLE_LIB_H
#define _BR_MLE_LIB_H

#include "ns_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Supported MLE Link layer Parameters changes at Run time*/
#define MLE_CHANNEL_UPDATE_TYPE 0       /* Update new link layer channel */
#define MLE_PANID_UPDATE_TYPE 1         /* Update new link layer PAN-ID */
#define MLE_ALLOW_JOIN_UPDATE_TYPE 2    /* Update Allow Join Bit at Beacon payload */

/**
 * \brief Update MLE link parameter at Run time
 *
 * \param nwk_id Network interface ID
 * \param type MLE_CHANNEL_UPDATE_TYPE, MLE_PANID_UPDATE_TYPE or MLE_ALLOW_JOIN_UPDATE_TYPE
 * \param time Interval to Update in milliseconds need to 5000 or bigger.
 * \param value indicate channel, pan-id or boolean value for allow join
 *
 * \return 0    Update process OK
 * \return -1   Old update process still active
 * \return -2   Unsupported Update Type
 * \return -3   Un supported value (channel not between 11-26 or pan id was 0xffff or 0xfffe)
 * \return -4 Stack at idle state
 *
 * List Possible values for specific types:
 * - MLE_CHANNEL_UPDATE_TYPE, supported values 11-26
 * - MLE_PAN_ID_UPDATE_TYPE ,supported value 0x0000 -0xfffd
 * - MLE_ALLOW_JOIN__UPDATE_TYPE ,16-bit Boolean 0=Disbaled and 1=Enabled
 *
 *
 */
extern int8_t arm_mle_prepare_update(int8_t nwk_id, uint8_t type, uint32_t time, uint16_t value);
/**
 * \brief Update Beacon Payload optional field
 *
 * \param nwk_id Network interface ID
 * \param optional_fields_data pointer to given Optional data
 * \param optional_fields_len length of optional data
 * \param delay Interval to Update in milliseconds need to 5000 or bigger.
 *
 * \return 0    Update process OK
 * \return -1   Old update process still active or Memory allocation fail
 * \return -2   Unsupported Update Type
 * \return -4 Stack at idle state
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
 * \return 0    Update process OK
 * \return -1   Memory allocation fail for packet
 *
 */
extern int8_t arm_mle_link_request_to_routers(int8_t nwk_id);
#ifdef __cplusplus
}
#endif
#endif /*_BR_MLE_LIB_H*/
