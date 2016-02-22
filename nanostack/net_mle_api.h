/*
 * Copyright (c) 2016 ARM Limited. All rights reserved.
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
 * \file net_mle.h
 * \brief 6LoWPAN MLE options control API
 *
 */
#ifndef _NET_MLE_API_H
#define _NET_MLE_API_H

#include "ns_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
  * \brief Set default MLE neighbor lifetime for a router.
  *
  * \param interface_id Interface id.
  * \lifetime Lifetime in seconds. 64 to 2560.
  *
  * \return 0, Lifetime update OK.
  * \return < 0, Lifetime update fail.
  *
  */
int8_t arm_nwk_6lowpan_mle_router_lifetime_set(int8_t interface_id, uint16_t lifetime);

/**
  * \brief Set default MLE neighbor lifetime for a host.
  *
  * \param interface_id Interface id.
  * \param lifetime Lifetime in seconds. 64 to 2560.
  *
  * \return 0, Lifetime update OK.
  * \return < 0, Lifetime update fail.
  *
  */
int8_t arm_nwk_6lowpan_mle_host_lifetime_set(int8_t interface_id, uint16_t lifetime);

/**
  * \brief Set MLE neighbor list limits
  *
  * MLE neighbor limits configuration settings limit the number of neighbors
  * added to MLE neighbor list.
  *
  * If number of neighbors reaches lower threshold MLE starts to limit addition
  * of new neighbors. This is made by starting to ignore multicast MLE messages
  * from unknown neighbors (ignore probability is randomized). Value must be
  * smaller than upper threshold and maximum value.
  *
  * If number of neighbors reaches upper threshold MLE stops to add new neighbors
  * based on multicast MLE messages. Only nodes that select this node for a
  * parent during bootstrap will be accepted. Value must be smaller or same as
  * maximum value.
  *
  * If number of neighbors reaches maximum value no new neighbors are added.
  *
  * If MLE neighbor list limits are not used all values must be set to 0.
  *
  * \param interface_id Interface id.
  * \param lower_threshold Lower threshold. 5 to 499. 0 limits not used.
  * \param upper_threshold. Upper threshold. 5 to 500. 0 limits not used.
  * \param max. Maximum number of neighbors. 5 to 500. 0 limits not used.
  *
  * \return 0, Limits update OK.
  * \return < 0, Limits update fail.
  *
  */
int8_t arm_nwk_6lowpan_mle_neighbor_limits_set(int8_t interface_id, uint16_t lower_threshold, uint16_t upper_threshold, uint16_t max);

/**
  * \brief Set MLE message token bucket settings
  *
  * MLE message token bucket limits MLE message sending rate. Token bucket size
  * controls the bucket size. Token bucket rate controls the rate in which
  * new tokens are added. Count defines how many tokens are added to bucket at a
  * time.
  *
  * Rate is entered in multiplies of 0.1 second minimum interval (e.g. if rate is 3
  * and count is 4 then 4 new tokens are added to bucket every 0.3 seconds).
  *
  * If token bucket is not used all values must be set to 0.
  *
  * \param interface_id Interface id.
  * \param size Bucket size. 1 to 255. 0 token bucket not used.
  * \param rate Token rate. 1 to 255. 0 token bucket not used.
  * \param count Token count. 1 to 255. 0 token bucket not used.
  *
  * \return 0, Token bucket settings update OK.
  * \return < 0, Token bucket settings update fail.
  *
  */
int8_t arm_nwk_6lowpan_mle_token_bucket_settings_set(int8_t interface_id, uint8_t size, uint8_t rate, uint8_t count);

#ifdef __cplusplus
}
#endif
#endif /* _NET_MLE_API_H */
