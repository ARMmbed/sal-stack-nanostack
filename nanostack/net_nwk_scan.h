/*
 * Copyright (c) 2014-2015 ARM. All rights reserved.
 */
#ifndef _NS_NWK_SCAN_H
#define _NS_NWK_SCAN_H
#ifdef __cplusplus
extern "C" {
#endif

#include "ns_types.h"

/**
 * \file net_nwk_scan.h
 * \brief Link layer Scan API for Active and Energy Detection Scan API.
 *
 * - arm_net_energy_scan(), Energy scan start (Use only with ZIP node library)
 * - arm_net_nwk_scan(), Active scan for network (Use only with ZIP node library)
 * - arm_net_get_scanned_nwk_list(), Active scan result read
 *
 * Scan API works only when stack is at idle state.
 *
 * Scan operation result will be handled at call back function which is defined by scan function call.
 * Scan result format is as follows:
 *
 * | Data pointer | VALUE                                                 |
 * | :----------: | :---------------------------------------------------: |
 * | 1.Byte       | Scan Type: NET_NWK_ENERGY_SCAN or NET_NWK_ACTIVE_SCAN |
 * | 2.Byte       | Result length                                         |
 * | 3+n Bytes    | Payload :Read Only at Energy Detection type           |
 *
 * Result length indicates scan response size as follows:
 *  - NET_NWK_ENERGY_SCAN payload length is result length *2 bytes after length field
 *   * 1.Byte Channel
 *   * 2.Byte Energy Level
 *  - NET_NWK_ACTIVE_SCAN result indicate network count
 *   * Network list need to read by net_get_scanned_nwk_list()
 *
 */

/** Network Energy Detection Scan Type  */
#define NET_NWK_ENERGY_SCAN 0
/** Network Active Scan Type  */
#define NET_NWK_ACTIVE_SCAN 1

/** Network Parent Address Type 16-bit Short */
#define NET_PARET_SHORT_16_BIT 2
/** Network Parent Address Type 64-bit Long */
#define NET_PARET_LONG_64_BIT 3

/* Active scan level definition */
/** List only PAN-networks at channels */
#define NET_ACTIVE_SCAN_ONLY_NWK 0
/** List PAN-networks with any beacon payload */
#define NET_ACTIVE_SCAN_ACCEPT_ANY_BEACON_PAYLOAD 1
/** List PAN-networks with ZIP specific payload which accept join */
#define NET_ACTIVE_SCAN_ACCEPT_ONLY_ZIP_SPESIFIC  2
/*!
 * \struct nwk_pan_alternative_parent_t
 * \brief Network Alternative Parent structure.
 */
typedef struct nwk_pan_alternative_parent_t {
    uint8_t CoordAddrMode; /**< Parent Address Mode NET_PARET_SHORT_16_BIT or NET_PARET_LONG_64_BIT*/
    uint8_t CoordAddress[8]; /**< Parent Address based on  CoordAddrMode */
    uint8_t LinkQuality; /**< LQI to parent */
} nwk_pan_alternative_parent_t;

/*!
 * \struct nwk_pan_descriptor_t
 * \brief Linked Network response list.
 */
typedef struct nwk_pan_descriptor_t {
    uint8_t CoordAddrMode;                              /**< Parent Address Mode NET_PARET_SHORT_16_BIT or NET_PARET_LONG_64_BIT*/
    uint16_t CoordPANId;                                /**< Network PAN-ID */
    uint8_t CoordAddress[8];                            /**< Parent Address based on  CoordAddrMode */
    uint8_t LogicalChannel;                             /**< Network Channel */
    uint8_t ChannelPage;                                /**< Channel Page 0 at 2.4Ghz */
    uint8_t SuperframeSpec[2];                          /**< Network SuperFrame setup */
    uint8_t LinkQuality;                                /**< LQI to parent */
    uint8_t *beacon_payload;                            /**< Beacon Payload pointer */
    uint8_t beacon_length;                              /**< Beacon Payload length */
    nwk_pan_alternative_parent_t *alternative_parent;   /**< Alternative Parent information pointer */
    struct nwk_pan_descriptor_t *next;                   /**< Link to next network result */
} nwk_pan_descriptor_t;

/**
 * \brief Energy Detection scan start for configured channel with application specific threshold.
 *
 * \param scan_list Channel List for scan operation.
 * \param passed_fptr function pointer for scan result notify
 * \param energy_tresshold scan response will list all channels which level is smaller or equal
 *
 * \return 0 Scan operation started OK.
 * \return -1 Stack is Active
 * \return -2 Channel list not valid
 * \return -3 Function not enabled at Border Router
 *
 */
extern int8_t arm_net_energy_scan(int8_t interface_id, uint32_t scan_list, void (*passed_fptr)(uint8_t *), uint8_t energy_tresshold);
/**
 * \brief Active Network scan for configured channels.
 *
 * \param scan_list Channel List for scan operation.
 * \param passed_fptr function pointer for scan result notify
 * \param scan_level NET_ACTIVE_SCAN_ONLY_NWK, NET_ACTIVE_SCAN_ACCEPT_ANY_BEACON_PAYLOAD,NET_ACTIVE_SCAN_ACCEPT_ONLY_ZIP_SPESIFIC
 *
 * \return 0 Scan operation started OK.
 * \return -1 Stack is Active
 * \return -2 Channel list not valid
 * \return -3 Function not enabled at Border Router
 *
 */
extern int8_t arm_net_nwk_scan(int8_t interface_id, uint32_t scan_list, void (*passed_fptr)(uint8_t *), uint8_t scan_level);
/**
 * \brief Active scan result read.
 *
 * Note: Pointer is only valid at Call back function call time. Application need to allocate memory if it wants to save the result.
 *
 * \return >0 Pointer to Scan result.
 * \return 0 When no network result available
 *
 */
extern nwk_pan_descriptor_t *arm_net_get_scanned_nwk_list(int8_t interface_id);
#ifdef __cplusplus
}
#endif
#endif /*_NS_NWK_SCAN_H*/
