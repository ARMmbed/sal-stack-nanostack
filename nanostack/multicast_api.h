/*
 * Copyright (c) 2014-2015 ARM Limited. All rights reserved.
 * Permissive Binary License
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
#ifndef MULTICAST_API_H_
#define MULTICAST_API_H_
/**
 * \file multicast_api.h
 * \brief Multicast Trickle Forwarding API.
 * \section multi-init Init API:
 * - multicast_set_parameters(), Set Trickle Parameters
 * \section multi-cnf Configure API:
 * - multicast_add_address(), Add new address to multicast group and control Trickle forwarding
 * - multicast_free_address(), Remove supported Multicast address from List
 *
 * \section ZigBeeIP Trickle Setups for Multicast Init
 *
 * | Parameter         | VALUE |
 * | :---------------: | :---: |
 * | imin              | 10    |
 * | imax              | 0     |
 * | k                 | 20    |
 * | timer_expirations | 3     |
 * | window_expiration | 75    |
 *
 */

#include "ns_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Set new parameters for Trickle Multicast.
 *
 * \param i_min Minimum Trickle timer interval in 50ms resolution: Imin = i_min * 50ms.
 * \param i_max Maximum Trickle timer interval as number if doubling of minimum interval.
 * \param k Redundancy constant.
 * \param timer_expirations Number if Trickle timer expirations before terminating Trickle process.
 * \param window_expiration Time window state is kept after Trickle process has ended in 50ms resolution.
 *        NOTE: If window_expiration value is set too small an infinite retransmission loop may occur when using the trickle multicast.
 */
extern void multicast_set_parameters(uint8_t i_min, uint8_t i_max, uint8_t k, uint8_t timer_expirations, uint8_t window_expiration);


/**
 * \brief Add new address to multicast group.
 *
 * \param address_ptr Pointer to a 16-byte array that includes the address to be added.
 * \param use_trickle 0 = no trickle multicast forwarding, all other values = trickle multicast forwarding will be used with this address.
 *
 * \return 0 general error.
 * \return 1 address updated.
 * \return 2 address added.
 * \return 255 link local not allowed when using multicast.
*
 */
extern uint8_t multicast_add_address(const uint8_t *address_ptr, uint8_t use_trickle);



/**
 * \brief Free address from multicast group.
 *
 * \param address_ptr Pointer to a 16-byte array that includes the address to be removed.
 *
 * \return 0 will be returned on successful execution, other values indicate an error on removing the address.
 */
extern uint8_t multicast_free_address(uint8_t *address_ptr);
#ifdef __cplusplus
}
#endif
#endif /* MULTICAST_API_H_ */
