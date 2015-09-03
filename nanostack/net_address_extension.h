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
  * \brief Set manually Link layer short address
  *
  * NOTE: At Duplicated address detection case (DAD) normally generate new short address but when
  * generate_dynamical_at_DAD = 0 Stack will generate user want use static value
  * \param short_id 16-bit user selected short address
  * \param generate_dynamical_at_DAD Set 1 if stack can generate new short address at DAD otherwise set 0.
  *
  * \return 0xfffe or 0xffff when Stack not use any short address
  */
extern void net_ext_set_short_address_from_app(uint16_t short_id, uint8_t generate_dynamical_at_DAD);
/**
  * \brief Clean current used Static or default short address and enable dynamically generation for new short address.
  *
  */
extern void net_ext_reset_short_address_from_app(void);
/**
  * \brief Get current used network short address.
  *
  * \return 0xfffe or 0xffff when Stack not use any short address
  */
extern uint16_t net_ext_get_short_address(void);
#ifdef __cplusplus
}
#endif
#endif
