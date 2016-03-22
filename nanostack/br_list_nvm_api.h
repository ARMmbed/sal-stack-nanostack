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
 * \file br_list_nvm_api.h
 * \brief Border Router ZigBeeIP RPL, MLE, Whiteboard & Routing table NVM API
 *
 * \section br-listapi Border Router List NVM API
 * - br_list_nvm_callback_set(), Enable MLE, Whiteboard and Routing Table NVM functionality
 * - mle_entry_store_from_nvm(), Load MLE entry from NVM
 * - WB_entry_store_from_nvm(), Load Whiteboard entry from NVM
 * - route_entry_store_from_nvm(), Load Routing table entry from NVM
 *

 * \section br-rplapi Border Router RPL NVM API
 * - rpl_nvm_callback_set(), Enable RPL Proxy NVM functionality (RPL Proxy Base, Routes, Prefix)
 * - rpl_nvm_base_reload(), Restore RPL Proxy base from NVM
 * - rpl_prefix_store_from_nvm(), Load prefix entry from NVM
 * - rpl_route_store_from_nvm(), Load Route entry from NVM
 *
 * \section br-rpseq Border Router ZigBeeIP RPL NVM reload initialize sequence
 *
 * 1. rpl_nvm_base_reload(), Load RPL base
 * 2. rpl_prefix_store_from_nvm(), Load Prefix(es) from NVM memory
 * 3. rpl_route_store_from_nvm(), Load Route(s) from NVM memory
 *
 */
#ifndef _BR_LIST_NVM_API_H
#define _BR_LIST_NVM_API_H

#include "ns_types.h"

#ifdef __cplusplus
extern "C" {
#endif


/*! \enum br_nvm_update_process_t
 * BR NVM generic List update states.
 */
typedef enum br_nvm_update_process_t {
    BR_MLE_MATERIAL_UPDATE, /**< MLE entry NVM update: Data 17 bytes structure[(Offset 16-bit), Data 15 bytes */
    BR_MLE_MATERIAL_REMOVE,/**< MLE entry NVM remove: Data 2 bytes structure Offset 16-bit pointer to NVM that need to remove*/
    BR_WB_MATERIAL_UPDATE, /**< Whiteboard entry NVM update: Data 27 bytes structure[(Offset 16-bit), Data 25 bytes */
    BR_WB_MATERIAL_REMOVE, /**< Whiteboard entry NVM remove: Data 2 bytes structure Offset 16-bit pointer to NVM that need to remove*/
    BR_ROUTE_MATERIAL_UPDATE, /**< Routing table entry NVM update: Data 38 bytes structure[(Offset 16-bit), Data 36 bytes */
    BR_ROUTE_MATERIAL_REMOVE, /**< Routing table entry NVM remove: Data 2 bytes structure Offset 16-bit pointer to NVM that need to remove*/
} br_nvm_update_process_t;

/** */
/*! \enum br_nvm_update_process_t
 * BR RPL NVM update states.
 */
typedef enum br_rpl_nvm_update_process_t {
    BR_RPL_BASE_UPDATE,/**< RPL Base Proxy NVM update: Data 33 bytes */
    BR_RPL_VER_DTSN_UPDATE, /**< RPL Base Proxy DTSN or DODAG version update NVM update: Data 33 bytes, DTSN & DODAG version offset is 18 bytes from data */
    BR_PREFIX_UPDATE, /**< RPL Prefix NVM update: Data 28 bytes structure[(Offset 16-bit), Data 26 bytes */
    BR_RPL_ROUTE_UPDATE, /**< RPL Route NVM update: Data 28 bytes structure[(Offset 16-bit), Data 26 bytes */
} br_rpl_nvm_update_process_t;

/* NVM List API PART */
/**
 * \brief Enable MLE, Whiteboard and Routing Table NVM functionality from the stack.
 *
 * \param passed_fptr A function pointer to NVM update process.
 * \param nvm_static_buffer A pointer to application allocated static memory, minimum size is 33 bytes.
 *
 * \return 0 NVM operation init OK.
 * \return -1 Null pointer parameter.
 *
 * Reference callback function structure that uses EEPROM:
 * - nvm_static_buffer Application allocated static buffer.
 *
 */
extern int8_t br_list_nvm_callback_set(uint16_t (*passed_fptr)(br_nvm_update_process_t), uint8_t *nvm_static_buffer);
/**
 * \brief Load MLE entry from NVM.
 *
 * \param nvm_data A pointer to the MLE entry.
 *
 * \return 0 NVM entry load OK.
 * \return -1 No memory for NVM entry.
 */
extern int8_t mle_entry_store_from_nvm(const uint8_t *nvm_data);
/**
  * \brief Load Whiteboard entry from NVM.
  *
  * \param nvm_data A pointer to whiteboard entry.
  *
  * \return 0 NVM entry load OK.
  * \return -1 No memory for NVM entry.
  */
extern int8_t WB_entry_store_from_nvm(const uint8_t *nvm_data);
/**
  * \brief Load Routing table entry from NVM.
  *
  * \param nvm_data A pointer to routing table entry.
  *
  * \return 0 NVM entry load OK.
  * \return -1 No memory for NVM entry.
  */
extern int8_t route_entry_store_from_nvm(const uint8_t *nvm_data);

/* NVM RPL API */
/**
 * \brief Enable RPL Proxy NVM functionality (RPL Proxy Base, Routes, Prefix).
 *
 * \param passed_fptr A function pointer to the NVM update process.
 * \param nvm_static_buffer A pointer to application allocated static memory, minimum size is 33 bytes.
 *
 * \return 0 NVM operation Init OK.
 * \return -1 Null pointer parameter.
 *
 * Reference callback function structure that uses EEPROM:
 * - nvm_static_buffer Application allocated static buffer.
 *
 */
extern int8_t rpl_nvm_callback_set(uint16_t (*passed_fptr)(br_rpl_nvm_update_process_t), uint8_t *nvm_static_buffer);

/**
   * \brief Restore RPL Proxy base from NVM.
   *
   * \param nvm_data A pointer to the RPL Proxy base.
   *
   * \return 0 NVM load OK.
   * \return -1 No memory for NVM entry.
   */
extern int8_t rpl_nvm_base_reload(const uint8_t *nvm_data, int8_t interface_id);

/**
   * \brief Load prefix entry from NVM.
   *
   * \param nvm_data A pointer to the RPL Prefix entry.
   *
   * \return 0 NVM load OK.
   * \return -1 No memory for NVM entry.
   */
extern int8_t rpl_prefix_store_from_nvm(const uint8_t *nvm_data, uint8_t instance_id);
/**
   * \brief Load Route entry from NVM.
   *
   * \param nvm_data A pointer to the RPL Route entry.
   *
   * \return 0 NVM load OK.
   * \return -1 No memory for NVM entry.
   */
extern int8_t rpl_route_store_from_nvm(const uint8_t *nvm_data, uint8_t instance_id);
#ifdef __cplusplus
}
#endif
#endif
