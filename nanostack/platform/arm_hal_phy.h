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

#ifndef ARM_HAL_PHY_H_
#define ARM_HAL_PHY_H_

#include "ns_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    PHY_INTERFACE_RESET,            /**< Reset PHY driver and set to idle. */
    PHY_INTERFACE_DOWN,             /**< Disable PHY interface driver (RF radio disable). */
    PHY_INTERFACE_UP,               /**< Enable PHY interface driver (RF radio receiver ON). */
    PHY_INTERFACE_RX_ENERGY_STATE, /**< Enable wirless interface ED scan mode. */
    PHY_INTERFACE_SNIFFER_STATE     /**< Enable sniffer mode. */
} phy_interface_state_e;


typedef enum {
    PHY_LINK_TX_DONE,           /**< TX process ready and ACK RX. */
    PHY_LINK_TX_DONE_PENDING,   /**< TX process OK with ACK pending flag. */
    PHY_LINK_TX_SUCCESS,        /**< MAC TX complete. MAC will a make decision to enter wait ACK or TX done state. */
    PHY_LINK_TX_FAIL,           /**< Link TX process fail. */
    PHY_LINK_CCA_FAIL,          /**< RF link CCA process fail. */
} phy_link_tx_status_e;


typedef enum {
    PHY_EXTENSION_CTRL_PENDING_BIT, /**< Control MAC pending bit for indirect data. */
    PHY_EXTENSION_READ_LAST_ACK_PENDING_STATUS, /**< Read status if the last ACK is still pending. */
    PHY_EXTENSION_SET_CHANNEL,  /**< Net library channel set. */
    PHY_EXTENSION_READ_CHANNEL_ENERGY, /**< RF interface ED scan energy read. */
    PHY_EXTENSION_READ_LINK_STATUS, /**< Net library could read link status. */
    PHY_EXTENSION_CONVERT_SIGNAL_INFO, /**< Convert signal info. */
    PHY_EXTENSION_SET_CCA_MODE, /** Set the CCA mode to be used for cca_start(), used by FHSS. Value given must be CCA_RANDOM_IN_RANGE or CCA_FHSS. */
} phy_extension_type_e;

typedef enum {
    PHY_MAC_48BIT, /**< IPv4/IPv6/BLE link layer address for Ethernet. This is optional. */
    PHY_MAC_64BIT, /**< RF/PLC link layer address. */
    PHY_MAC_16BIT, /**< RF interface short address. */
    PHY_MAC_PANID, /**< RF interface 16-Bit PAN-ID. */
} phy_address_type_e;

typedef enum phy_link_type_e {
    PHY_LINK_ETHERNET_TYPE,         /**< Standard IEEE 802 Ethernet. */
    PHY_LINK_15_4_2_4GHZ_TYPE,      /**< Standard 802.15.4 2.4GHz radio. */
    PHY_LINK_15_4_SUBGHZ_TYPE,      /**< Standard 802.15.4 subGHz radio 868 /915MHz. */
    PHY_LINK_SW_RADIO_TYPE,         /**< Proprietary radio frequency controlled by SW. */
    PHY_LINK_PLC_TYPE,              /**< PLC PHY link. */
    PHY_LINK_TUN,                   /**< Tunnel interface for Linux TUN, RF network driver over serial bus or just basic application to application data flow. */
} phy_link_type_e;

typedef enum data_protocol_e {
    LOCAL_SOCKET_DATA = 0,          /**< 6LoWPAN library local socket data. */
    INTERFACE_DATA = 1,             /**< 6LoWPAN library interface internal used protocol. */
    PHY_LAYER_PAYLOAD = 2,          /**< PHY layer data selection or handler. */
    IPV6_DATAGRAM = 3,              /**< IP layer data or TUN driver request data. */
    UNKNOWN_PROTOCOL = 4            /**< Non-supported protocol ID. */
} data_protocol_e;


typedef enum driver_data_request_e {
    PHY_LAYER_PAYLOAD_DATA_FLOW,    /**< PHY layer data. */
    IPV6_DATAGRAMS_DATA_FLOW,       /**< IP layer data or TUN driver request data. */
} driver_data_request_e;

/**
 * Types of signal quality indication desired by various link protocols. Some are
 * really statistical, but a driver should ideally be able to create an estimate
 * based on its LQI/DBM numbers, for example to bootstrap a statistic calculation.
 */
typedef enum phy_signal_info_type_e {
    PHY_SIGNAL_INFO_ETX,            /**< Expected transmissions, unsigned 16-bit fixed-point ETX*128 [1..512], for example Zigbee IP + RFC 6719. */
    PHY_SIGNAL_INFO_IDR,            /**< Inverse Delivery Ratio, unsigned 16-bit fixed-point IDR*32*256 [1..8], for example MLE draft 06. */
    PHY_SIGNAL_INFO_LINK_MARGIN,    /**< Link margin, unsigned 16-bit fixed-point dB*256, [0..255], for example Thread routing draft. */
} phy_signal_info_type_e;

typedef struct phy_signal_info_s {
    phy_signal_info_type_e type;    /**< Signal info type desired. */
    uint8_t lqi;                    /**< Quality passed to arm_net_phy_rx. */
    int8_t dbm;                     /**< Strength passed to arm_net_phy_rx. */
    uint16_t result;                /**< Resulting signal information. */
} phy_signal_info_s;

typedef enum phy_modulation_e
{
    M_OFDM,
    M_OQPSK,
    M_BPSK,
    M_GFSK,
    M_UNDEFINED
} phy_modulation_e;

typedef enum
{
    CHANNEL_PAGE_0 = 0,
    CHANNEL_PAGE_1 = 1,
    CHANNEL_PAGE_2 = 2,
    CHANNEL_PAGE_3 = 3,
    CHANNEL_PAGE_4 = 4,
    CHANNEL_PAGE_5 = 5,
    CHANNEL_PAGE_6 = 6,
    CHANNEL_PAGE_9 = 9,
    CHANNEL_PAGE_10 = 10
} channel_page_e;


typedef struct phy_rf_channel_configuration_s
{
    uint32_t channel_0_center_frequency;
    uint32_t channel_spacing;
    uint32_t datarate;
    uint16_t number_of_channels;
    phy_modulation_e modulation;
} phy_rf_channel_configuration_s;

typedef struct phy_device_channel_page_s
{
    channel_page_e channel_page;
    const phy_rf_channel_configuration_s *rf_channel_configuration;
} phy_device_channel_page_s;

typedef struct phy_device_driver_s
{
    phy_link_type_e link_type;                                      /**< Define driver types. */
    driver_data_request_e data_request_layer;                       /**< Define interface data OUT protocol. */
    uint8_t *PHY_MAC ;                                              /**< Pointer to 64-bit or 48-bit MAC address. */
    char *driver_description;                                       /**< Short driver platform description. Needs to end with zero. */
    uint16_t phy_MTU;                                               /**< Define MAX PHY layer MTU size. */
    uint8_t phy_tail_length;                                        /**< Define PHY driver needed TAIL Length. */
    uint8_t phy_header_length;                                      /**< Define PHY driver needed header length before PDU. */
    int8_t (*state_control)(phy_interface_state_e, uint8_t);        /**< Function pointer for control PHY driver state. */
    int8_t (*tx)(uint8_t *, uint16_t, uint8_t, data_protocol_e);    /**< Function pointer for PHY driver write operation. */
    int8_t (*address_write)(phy_address_type_e , uint8_t *);        /**< Function pointer for PHY driver address write. */
    int8_t (*extension)(phy_extension_type_e, uint8_t *);           /**< Function pointer for PHY driver extension control. */
    const phy_device_channel_page_s *phy_channel_pages;
} phy_device_driver_s;


/** Net Library PHY Interface API*/
/**
 * \brief This function registers the device driver to stack.
 *
 * \param phy_driver A pointer to device driver structure.
 *
 * \return >= 0 Device driver ID.
 * \return < 0 Means register fail.
 *
 */
extern int8_t arm_net_phy_register(phy_device_driver_s *phy_driver);

/**
 * \brief This function is API for DATA RX to stack.
 *
 * \param data_type Defines received DATA type.
 * \param data_ptr A pointer to data.
 * \param data_len Data length.
 * \param link_quality Radio link quality.
 * \param dbm Received signal strength.
 * \param interface_id The interface ID where the packet is coming from.
 * \return >= 0 Data RX OK.
 * \return < 0 Data push fail.
 *
 */
extern int8_t arm_net_phy_rx(data_protocol_e data_type, const uint8_t *data_ptr, uint16_t data_len, uint8_t link_quality, int8_t dbm, int8_t interface_id);

/**
 * \brief This function is an API for indicating that the device driver write operation is done.
 *
 * \param interface_id Device driver ID.
 * \param tx_handle Unique handle for a packet.
 * \param status Define TX process status.
 * \param cca_retry Number of CCA attempts for this TX process.
 * \param tx_retry Number of TX retries for this packet.
 *
 * \return >= 0 Process OK.
 * \return < 0 Process handler fail.
 *
 */
extern int8_t arm_net_phy_tx_done(int8_t driver_id, uint8_t tx_handle, phy_link_tx_status_e status, uint8_t cca_retry, uint8_t tx_retry);

#ifdef __cplusplus
}
#endif
#endif /* ARM_HAL_PHY_H_ */
