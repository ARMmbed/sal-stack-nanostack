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
#ifndef _NET_ADDRESS_EXTENSION_H
#define _NET_ADDRESS_EXTENSION_H

#include "ns_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
  * \file net_address_extension.h
  * \brief API for setting up Link layer short address manually
  *
  * \section set-addr Setting short address
  *  - net_ext_set_short_address_from_app(), sets up a user specified short address and enables or disables DAD
  *
  * NOTE: In case of duplication of address and DAD (duplicate address detection) enabled, i.e., DAD=1, the stack will
  * generate a short address dynamically.
  *
  * - \section reset-addr Resetting short address
  * - net_ext_reset_short_address_from_app(), clears any user specified static or default short address and enables dynamic generation.
  */

/**
  * \brief Function to set short address from application.
  *
  * \param short_id 16-bit user specified short address
  * \param generate_dynamical_at_DAD Set 1 if stack is allowed to generate new short address dynamically in case of DAD otherwise set 0.
  *
  * \return 0xfffe or 0xffff when the stack does not use any short address
  */

extern void net_ext_set_short_address_from_app(uint16_t short_id, uint8_t generate_dynamical_at_DAD);

/**
  * \brief Clears currently being used Static or default short address and enables dynamic generation of new short address.
  */
extern void net_ext_reset_short_address_from_app(void);

/**
  * \brief Get current short address being used in the network for this particular interface.
  *
  * \return 0xfffe or 0xffff if stack is not using any short address
  */
extern uint16_t net_ext_get_short_address(void);
#ifdef __cplusplus
}
#endif
#endif
