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
#ifndef _NS_RPL_H
#define _NS_RPL_H

#include "ns_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \file net_rpl.h
 * \brief ZigBeeIP Router and Border Router RPL API.
 *
 * This API is for primary use for Border Router. Router Side is possible to use but it will eat extra memory resource then.
 *
 * - arm_nwk_6lowpan_rpl_dodag_init(), Allocate and Init RPL Dodag Proxy
 * - arm_nwk_6lowpan_rpl_dodag_remove(), Remove by selected Instance ID
 * - arm_nwk_6lowpan_rpl_dodag_start(), Activate RPL Dodag Instance
 * - arm_nwk_6lowpan_rpl_dodag_poison(), Poison current RPL Instance and remove from Active list. Proxy is still allocated arm_nwk_6lowpan_rpl_dodag_remove() function remove.
 * - arm_nwk_6lowpan_rpl_dodag_prefix_update(), Update prefix info to Proxy.
 * - arm_nwk_6lowpan_rpl_dodag_route_update(), Update Route info to Proxy.
 * - arm_nwk_6lowpan_rpl_dodag_dao_trig(), Update Trig DAO's from network by DTSN value increment and DIO multicasting
 * - arm_nwk_6lowpan_rpl_dodag_version_increment(), Update Reset DODAG and Trig DAO's from network by DODAG version number value increment and DIO multicasting
 *
 * \section rpl-recommend RECOMMEND API for Router and Border Router:
 * - rpl_instance_list_read(), Read Active RPL instance List
 * - rpl_read_dodag_info(), Read RPL Dodag information to rpl_dodag_info_t structure by selected RPL Instance ID.
 *
 * \section rpl-dodag-init Instruction step How to define new RPL DODAG Instance
 *  1. Allocate RPL Proxy base by  arm_nwk_6lowpan_rpl_dodag_init()
 *  2. Set 1 Prefix to to proxy by arm_nwk_6lowpan_rpl_dodag_prefix_update()
 *  3. Set Route(s) to Proxy by arm_nwk_6lowpan_rpl_dodag_route_update()
 *  4. Start Activate RPL DODAG
 *   * arm_nwk_6lowpan_rpl_dodag_start() if RPL is generated after arm_nwk_interface()
 *
 */

/* DoDag Root setups */
/* DODAGPreference (Prf): A 3-bit unsigned integer that defines how
         preferable the root of this DODAG is compared to other DODAG
         roots within the instance.  DAGPreference ranges from 0x00
         (least preferred) to 0x07 (most preferred).  The default is 0
         (least preferred). */
#define RPL_DODAG_PREF_MASK         0x07
#define RPL_DODAG_PREF(n)           ((n) & RPL_DODAG_PREF_MASK)

/* Mode of Operation (MOP): The Mode of Operation (MOP) field identifies
   the mode of operation of the RPL Instance as administratively
   provisioned at and distributed by the DODAG root.  All nodes
   who join the DODAG must be able to honor the MOP in order to
   fully participate as a router, or else they must only join as a leaf.
*/
#define RPL_MODE_MASK               0x38
#define RPL_MODE_NO_DOWNWARD        0x00
#define RPL_MODE_NON_STORING        0x08
#define RPL_MODE_STORING            0x10
#define RPL_MODE_STORING_MULTICAST  0x18

/* Grounded (G): The Grounded 'G' flag indicates whether the DODAG
   advertised can satisfy the application-defined goal.  If the
   flag is set, the DODAG is grounded.  If the flag is cleared,
   the DODAG is floating.
*/
#define RPL_GROUNDED                0x80

/** FOR BACKWARDS COMPATIBILITY **/
#define BR_DODAG_PREF_0             RPL_DODAG_PREF(0)
#define BR_DODAG_PREF_1             RPL_DODAG_PREF(1)
#define BR_DODAG_PREF_2             RPL_DODAG_PREF(2)
#define BR_DODAG_PREF_3             RPL_DODAG_PREF(3)
#define BR_DODAG_PREF_4             RPL_DODAG_PREF(4)
#define BR_DODAG_PREF_5             RPL_DODAG_PREF(5)
#define BR_DODAG_PREF_6             RPL_DODAG_PREF(6)
#define BR_DODAG_PREF_7             RPL_DODAG_PREF(7)
#define BR_DODAG_MOP_NON_STORING    RPL_MODE_NON_STORING
#define BR_DODAG_MOP_STORING        RPL_MODE_STORING
#define BR_DODAG_FLOATING           0
#define BR_DODAG_GROUNDED           RPL_GROUNDED

/* Compatibility for even older misspellings */
#define BR_DODAG_MOP_NON_STRORING   BR_DODAG_MOP_NON_STORING
#define BR_DODAG_MOP_STRORING       BR_DODAG_MOP_STORING
#define BR_DODAG_FLOATIN            BR_DODAG_FLOATING

/** RPL ROOT Parent flag */
#define RPL_ROOT_PARENT             0
/** RPL Primary active primary_parent primary_parent_rank information is valid*/
#define RPL_PRIMARY_PARENT_SET      1
/** RPL Secondary active secondary_parent secondary_parent_rank information is valid*/
#define RPL_SECONDARY_PARENT_SET    2

/** RPL Prefix update Flags for A-flag AUTONOUS address generation*/
#define RPL_PREFIX_AUTONOMOUS_ADDRESS_FLAG 0x40
/** RPL Prefix update Flags for R-Flag */
#define RPL_PREFIX_ROUTER_ADDRESS_FLAG 0x20

/*!
 * \struct rpl_dodag_info_t
 * \brief RPL Intance Dodag Info structure for rpl_read_dodag_info.
 */
typedef struct rpl_dodag_info_t {
    uint8_t dodag_id[16];           /**< RPL Dodag ID */
    uint8_t instance_id;            /**< RPL Instance ID */
    uint8_t flags;                  /**< RPL DODAG Flags: (MOP,Grounded, Router Pref) */
    uint8_t version_num;            /**< RPL DODAG version number */
    uint8_t DTSN;                   /**< RPL DODAG Dao trig version number */
    //Private Part
    uint16_t curent_rank;           /**< RPL DODAG node current Rank */
    uint8_t parent_flags;           /**< RPL DODAG parent Flags: RPL_ROOT_PARENT or RPL_PRIMARY_PARENT_SET, RPL_SECONDARY_PARENT_SET*/
    uint8_t primary_parent[16];     /**< Primary Parent GP address if RPL_PRIMARY_PARENT_SET flag is active*/
    uint16_t primary_parent_rank;   /**< Primary Parent Rank if RPL_PRIMARY_PARENT_SET flag is active*/
    uint8_t secondary_parent[16];   /**< Secondary Parent GP address if RPL_SECONDARY_PARENT_SET flag is active*/
    uint16_t secondary_parent_rank; /**< Secondary Parent rank if RPL_SECONDARY_PARENT_SET flag is active*/
} rpl_dodag_info_t;

/*!
 * \struct dodag_config_t
 * \brief RPL Dodag Config will be used when allocate rpl base arm_nwk_6lowpan_rpl_dodag_init().
 */
typedef struct dodag_config_t {
    uint8_t DAG_SEC_PCS;        /**< Define Possible parents for node. Recommended and max value is 1 means 2 parent */
    uint8_t DAG_DIO_INT_DOUB;   /**< RPL Trigle DIOIntervalDoublings , Should use 12 */
    uint8_t DAG_DIO_INT_MIN;    /**< RPL Trigle DIOIntervalMin , Should use 9 */
    uint8_t DAG_DIO_REDU;       /**< RPL Trigle DIORedundancyConstant , Should use 3 */
    uint16_t DAG_MAX_RANK_INC;  /**< RPL MaxRankIncrease , Should use 16 */
    uint16_t DAG_MIN_HOP_RANK_INC;  /**< RPL MinHopRankIncrease, Should use 0x80 */
    uint16_t DAG_OCP;           /**< Objective Code Point must use 1 */
    uint8_t LIFE_IN_SECONDS;    /**< Life in seconds  = LIFE_IN_SECONDS * LIFETIME_UNIT */
    uint16_t LIFETIME_UNIT;     /**< Defines how long is 1 tick LIFE_IN_SECONDS in seconds, 60 means 1 minute*/
} dodag_config_t;

/**
  * \brief RPL DODAG proxy base allocate.
  *
  * \param dodag_id pointer to unique DODAGID, This must be Node GP address at ZigBeeIP network
  * \param config pointer to Dodag Configure structure
  * \param instace_id instance ID for RPL DODAG
  * \param flags define RPL MOP and Dodag Pref, ZigBeeIP should use (BR_DODAG_MOP_NON_STORING | BR_DODAG_PREF_7)
  *
  * \return 0, Allocate OK
  * \return -1, Allocate Fail(Node connected Already to same Instance or memory allocate fail)
  *
  */
extern int8_t arm_nwk_6lowpan_rpl_dodag_init(int8_t interface_id, const uint8_t *dodag_id, const dodag_config_t *config, uint8_t instace_id, uint8_t flags);
/**
  * \brief RPL DODAG remove by given instance ID.
  *
  * \param instace_id instance ID for Removed DODAG
  *
  * \return 0, Remove OK
  * \return -1, Remove Fail
  *
  */
extern int8_t arm_nwk_6lowpan_rpl_dodag_remove(int8_t interface_id);

/**
  * \brief Activate RPL DODAG by given Interface ID.
  *
  * \param interface_id Interface ID for Start DODAG root
  *
  * \return 0, Start OK
  * \return <0, Start Fail
  *
  */
extern int8_t arm_nwk_6lowpan_rpl_dodag_start(int8_t interface_id);

/**
  * \brief RPL prefix information Update.
  *
  * \param nwk_interface_id Proxy Interface ID
  * \param prefix_ptr pointer to IPv6 prefix (16-bytes)
  * \param prefix_len prefix length should be 64
  * \param flags  define R-flag (RPL_PREFIX_ROUTER_ADDRESS_FLAG), A-flag (RPL_PREFIX_AUTONOMOUS_ADDRESS_FLAG)
  * \param lifetime prefix lifetime
  *
  *
  * \return 0, Update OK
  * \return <0, Update Fail
  *
  */
extern int8_t arm_nwk_6lowpan_rpl_dodag_prefix_update(int8_t nwk_interface_id, uint8_t *prefix_ptr, uint8_t prefix_len, uint8_t flags, uint32_t lifetime);
/**
  * \brief RPL Route information Update.
  *
  * \param nwk_interface_id Proxy Interface ID
  * \param route_ptr pointer to IPv6 prefix (16-bytes)
  * \param prefix_len prefix length should be 64
  * \param flags  define R-flag (RPL_PREFIX_ROUTER_ADDRESS_FLAG)
  * \param lifetime route lifetime
  *
  *
  * \return 0, Update OK
  * \return <0, Update Fail
  *
  */
extern int8_t arm_nwk_6lowpan_rpl_dodag_route_update(int8_t nwk_interface_id, uint8_t *route_ptr, uint8_t prefix_len, uint8_t flags, uint32_t lifetime);

/**
  * \brief RPL Dodag Poison.
  *
  * Function Poison current RPL instance and after few seconds arm_nwk_6lowpan_rpl_dodag_remove() could clean Proxy also.
  *
  * \param nwk_interface_id interface ID which define RPL instance will be Poisoned
  *
  *
  * \return 0, Poison OK
  * \return <0, Poison Fail
  *
  */
extern int8_t arm_nwk_6lowpan_rpl_dodag_poison(int8_t nwk_interface_id);
/**
  * \brief RPL Dodag Dao trig by DTSN increment.
  *
  *
  * \param nwk_interface_id Interface ID which RPL instance will update DTSN
  *
  *
  * \return 0, DAO trig OK
  * \return <0, DAO trig Fail
  *
  */
extern int8_t arm_nwk_6lowpan_rpl_dodag_dao_trig(int8_t nwk_interface_id);
/**
  * \brief RPL Dodag version update.
  *
  * Network devices should reset current RPL Instance and Do Unicast DIS/DIO and DAO/DAO ACK handshake.
  *
  * \param nwk_interface_id Interface ID which define RPL instance will update DODAG version
  *
  *
  * \return 0, Version Update OK
  * \return <0, Version Update Fail
  *
  */
extern int8_t arm_nwk_6lowpan_rpl_dodag_version_increment(int8_t nwk_interface_id);
/**
  * \brief Read nodes RPL instance list.
  *
  * \param cache_ptr pointer to where stack put Instance ID's
  * \param cache_size instance list cache size
  *
  * \return RPL Instance count
  *
  */
extern uint8_t rpl_instance_list_read(uint8_t *cache_ptr, uint8_t cache_size);
/**
  * \brief Read Dodag Information by given RPL Instance ID.
  *
  * \param dodag_ptr pointer to DODAG information structure
  * \param instance_id readed instance id
  *
  * \return 1, Read OK
  * \return 0, Read Fail
  *
  */
extern uint8_t rpl_read_dodag_info(rpl_dodag_info_t *dodag_ptr, uint8_t instance_id);

#ifdef __cplusplus
}
#endif
#endif /*_NS_RPL_H*/
