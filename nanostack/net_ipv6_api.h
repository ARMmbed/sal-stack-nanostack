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

#ifndef NET_IPV6_API_H_
#define NET_IPV6_API_H_

#include "ns_types.h"

/**
 * \brief Set maximum IPv6 fragmented datagram reception size
 *
 * Set the maximum size limit for fragmented datagram reception.
 *
 * RFC 2460 requires this to be at least 1500. It should also be at least
 * as large as the MTU of each attached link.
 *
 * \param frag_mru fragmented Maximum Receive Unit in octets
 * \return 0 Change ok - actual MRU is at least requested value
 * \return <0 Change invalid - unable to set the specified MRU
 */
int8_t arm_nwk_ipv6_frag_mru(uint16_t frag_mru);

/**
 * \brief Configure automatic flow label calculation
 *
 * Enable or disable automatic generation of IPv6 flow labels for outgoing
 * packets.
 *
 * \param auto_flow_label true to enable auto-generation
 */
void arm_nwk_ipv6_auto_flow_label(bool auto_flow_label);

#endif /* NET_IPV6_API_H_ */
