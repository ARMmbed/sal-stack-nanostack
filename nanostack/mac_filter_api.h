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

/*
 * \file mac_filter.h
 * \brief Filter API to allow modification of link qualities and filtering of devices in MAC level.
 *
 * Modifies the link qualities of devices as following.
 * lqi range 0x00 - 0xff
 *   lqi_result = initial_value*lqi_m/256 + lqi_add
 *   if value becomes 0 the packet is ignored
 *
 * dbm range -90dbm - 127
 *   dbm_result = initial_value*dbm_m/256 + dbm_add
 *   If value drops below -90 as result of modification the packet is ignored
 */

#ifndef MAC_FILTER_API_H_
#define MAC_FILTER_API_H_

/**
 * Start MAC level filter
 * This function can be called again if default values are modified.
 *
 *
 * \param interface_id Network Interface
 * \param lqi_m LQI multiplier (8.8 Fixed point multiplier)
 * \param lqi_add Value added to LQI
 * \param dbm_m dbm multiplier (8.8 Fixed point multiplier)
 * \param dbm_add Value added to dbm
 *
 * \return 0, OK
 * \return <0 Not OK
 */

int_fast8_t mac_filter_start(int8_t interface_id, int16_t lqi_m, int16_t lqi_add, int16_t dbm_m, int16_t dbm_add);

/**
 * Stop MAC level filter
 *
 * stops the filtering module and clears the default settings and all the filterings made.
 *
 * \param interface_id Network Interface
 *
 * \return 0, OK
 * \return <0 Not OK
 */

void mac_filter_stop(int8_t interface_id);

/**
 * Helper macros
 * usage:
 * mac_filter_start(interface_id, MAC_FILTER_BLOCKED) //Black list filter
 * mac_filter_start(interface_id, MAC_FILTER_ALLOWED) //White list filter not modifying the qualities
 * mac_filter_start(interface_id, MAC_FILTER_FIXED(0x01, -80)) //Fixed value for default link quality poor quality
 * mac_filter_start(interface_id, MAC_FILTER_FIXED(0xff, -20)) //Fixed value for default link quality good quality
 *
 * mac_filter_add_short(interface_id, 0x0001, MAC_FILTER_BLOCKED)
 * mac_filter_add_short(interface_id, 0x0001, MAC_FILTER_ALLOWED)
 * mac_filter_add_short(interface_id, 0x0001, MAC_FILTER_FIXED(0xff,-30))
 *
 * mac_filter_add_long(interface_id, mac64, MAC_FILTER_BLOCKED)
 * mac_filter_add_long(interface_id, mac64, MAC_FILTER_ALLOWED)
 * mac_filter_add_long(interface_id, mac64, MAC_FILTER_FIXED(0x7f, -60))
 */
#define MAC_FILTER_ALLOWED 0x100, 0, 0x100, 0
#define MAC_FILTER_BLOCKED 0, 0, 0, 0
#define MAC_FILTER_FIXED(lqi,dbm) 0, lqi, 0, dbm

/**
 * Delete all filters.
 *
 * Leaves the default link configuration.
 *
 * \param interface_id Network Interface
 *
 * \return 0, OK
 * \return <0 Not OK
 */
int_fast8_t mac_filter_clear(int8_t interface_id);

/**
 * Map extended address to short address.
 *
 * \param interface_id Network Interface
 * \param mac64 extended address
 * \param[out] mac16 Return the short address
 *
 * \return 0, address resolved
 * \return <0 No mapping found
 */
typedef int_fast8_t (mac_filter_map_extented_to_short_cb)(int8_t interface_id, uint8_t mac64[8], uint16_t *mac16);

/**
 * Map short address to extended address.
 *
 * \param interface_id Network Interface
 * \param mac64[out] return buffer for the extended address
 * \param mac16 short address
 *
 * return 0, address resolved
 * return <0 No mapping found
 */
typedef int_fast8_t (mac_filter_map_short_to_extended_cb)(int8_t interface_id, uint8_t mac64[8], uint16_t mac16);

/**
 * Register address mapping functions.
 *
 * This function should be added for layer in stack that keeps up the list of address mapping functions.
 * If filters are enabled these functions are called if no match from filters was found.
 *
 * when this service is no longer provided call this function with NULL pointers.
 *
 * \param interface_id Network Interface
 * \param long_cb address mapping to resolve long address from short address
 * \param short_cb address mapping to resolve short address from long address
 *
 * \return 0, OK
 * \return <0 Not OK
 */
int_fast8_t mac_filter_set_address_mapping(int8_t interface_id, mac_filter_map_short_to_extended_cb *long_cb, mac_filter_map_extented_to_short_cb *short_cb);

/**
 * Delete filter for device
 *
 * \param interface_id Network Interface
 * \param mac16 Short address.
 *
 * \return 0, OK
 * \return <0 Not OK
 */
int_fast8_t mac_filter_delete_short(int8_t interface_id, uint16_t mac16);

/**
 * Delete filter for device
 *
 * \param interface_id Network Interface
 * \param mac64 long address.
 *
 * \return 0, OK
 * \return <0 Not OK
 */
int_fast8_t mac_filter_delete_long(int8_t interface_id, uint8_t mac64[8]);

/**
 * Add MAC short address filter
 *
 * \param interface_id Network Interface.
 * \param mac16 Short address.
 * \param lqi_m LQI multiplier (8.8 Fixed point multiplier)
 * \param lqi_add Value added to LQI
 * \param dbm_m dbm multiplier (8.8 Fixed point multiplier)
 * \param dbm_add Value added to dbm
 *
 * \return 0, OK
 * \return <0 Not OK
 */
int_fast8_t mac_filter_add_short(int8_t interface_id, uint16_t mac16, int16_t lqi_m, int16_t lqi_add, int16_t dbm_m, int16_t dbm_add);

/**
 * Add MAC long address filter
 *
 * \param interface_id Network Interface.
 * \param mac64 long address.
 * \param lqi_m LQI multiplier (8.8 Fixed point multiplier)
 * \param lqi_add Value added to LQI
 * \param dbm_m dbm multiplier (8.8 Fixed point multiplier)
 * \param dbm_add Value added to dbm
 *
 * \return 0, OK
 * \return <0 Not OK
 */
int_fast8_t mac_filter_add_long(int8_t interface_id, uint8_t mac64[8], int16_t lqi_m, int16_t lqi_add, int16_t dbm_m, int16_t dbm_add);

#endif /* MAC_FILTER_API_H_ */
