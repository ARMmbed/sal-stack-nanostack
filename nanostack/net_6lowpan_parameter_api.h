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


#ifndef NET_6LOWPAN_PARAMETER_API_H_
#define NET_6LOWPAN_PARAMETER_API_H_

#include "ns_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * 6LoWPAN Default timer values:
 * - Timer values are specified in ticks
 * - Default 6LoWPAN ND Bootstrap tick is 1 tick = 100ms
 *
 * - Default Random value 0x1f = [0..3.1] seconds
 * - Default RS Retry counter 3
 * - Default RS retry interval is 15 + random --> [1.5..4.6] seconds
 * - Default NS Retry counter 5
 * - Default NS retry interval is 100 + random + backoff --> [10.0..13.1+10.0*retry] seconds
 * - Default NS Retry linear backoff is 100
 *
 * - Default RA transmit interval is 150, exponentially backed off --> 15.0, 30.0, 60.0 seconds
 * - Default RA transmit counter is 5
 *
 * - Default NS forward timeout is 300 --> 30.0 seconds
 *
 *  Note for changing random and interval values:
 *  Random parameter + NS or RS minimum interval must sum to less than 0xFFFF
 *  Random maximums are manipulated as bit masks, so must be (2^n)-1.
 */

/*!
 * \struct nd_parameters_s
 * \brief 6LoWPAN Neighbor Discovery parameters
 */
typedef struct nd_parameters_s {
    uint8_t rs_retry_max;                   /**< Define Bootstrap RS max retry count */
    uint8_t ns_retry_max;                   /**< Define Bootstrap NS max retry count */
    uint16_t timer_random_max;              /**< Define Interval random in 6LoWPAN bootstrap timer ticks for RS, NS and starting NS - NA process  */
    uint16_t rs_retry_interval_min;         /**< Define Retry interval in 6LoWPAN bootstrap timer ticks waiting for RA */
    uint16_t ns_retry_interval_min;         /**< Define Retry interval in 6LoWPAN bootstrap timer ticks waiting for NA */
    uint16_t ns_retry_linear_backoff;       /**< Define Retry interval linear backoff in bootstrap timer ticks */

    uint16_t ra_interval_min;               /**< Define initial transmission interval for Router Advertisements in standard timer ticks */
    uint8_t ra_transmits;                   /**< Define number of RA transmissions */

    uint16_t ns_forward_timeout;            /**< Define timeout when forwarding NS messages - if reached, our own address discovery process is restarted */
} nd_parameters_s;

/**
 * \brief API to change 6LoWPAN ND bootstrap parameters.
 *
 * Note: This function should be called after net_init_core() and definitely
 * before creating any 6LoWPAN interface.
 *
 * For future compatibility, to support extensions to this structure, read
 * the current parameters using net_6lowpan_timer_parameter_read(),
 * modify known fields, then set.
 *
 * \param parameter_ptr Pointer for ND parameters
 *
 * \return 0, Change OK
 * \return -1, Invalid values
 * \return -2, 6LoWPAN interface already active
 *
 */
extern int8_t net_6lowpan_nd_parameter_set(const nd_parameters_s *parameter_ptr);

/**
 * \brief API for change 6LoWPAN bootstrap base tick 100ms multiplier.
 *
 * Note: This function MUST be called after net_init_core(). Do not change this
 * unless you really want 6LoWPAN bootstrap working slower than normally.
 *
 * This only affects the bootstrap timers.
 *
 * \param base_tick_x_100ms Tick resolution in 100ms units.
 *        Max value 10 --> 10 times slower functionality
 *
 * \return 0, Change OK
 * \return -1, Invalid value (<1 or >10)
 *
 */
extern int8_t net_6lowpan_nd_timer_base_tick_set(uint8_t base_tick_x_100ms);

/**
 * \brief API to read 6LoWPAN ND bootstrap parameters.
 *
 * \param parameter_ptr Output pointer for ND parameters
 *
 */
extern void net_6lowpan_nd_parameter_read(nd_parameters_s *parameter_ptr);

#ifdef __cplusplus
}
#endif

#endif /* NET_6LOWPAN_DEFAULT_PARAMETER_API_H_ */
