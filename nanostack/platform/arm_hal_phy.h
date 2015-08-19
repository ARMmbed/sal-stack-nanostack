/* Permissive Binary License
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
/*
 * Copyright (c) 2014-2015 ARM Limited. All rights reserved.
 */

#ifndef ARM_HAL_PHY_H_
#define ARM_HAL_PHY_H_

#include "ns_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    PHY_INTERFACE_RESET,            /**< Reset PHY driver and set to idle */
    PHY_INTERFACE_DOWN,             /**< Disable PHY Interface driver (RF radio disable) */
    PHY_INTERFACE_UP,               /**< Enable PHY Interface driver (RF radio receiver ON) */
    PHY_INTERFACE_RX_ENERGY_STATE, /**< Enable wirless interface ED scan mode */
    PHY_INTERFACE_SNIFFER_STATE     /**< Enable Sniffer mode */
} phy_interface_state_e;


typedef enum {
    PHY_LINK_TX_DONE,           /**< TX process Ready and ACK RX */
    PHY_LINK_TX_DONE_PENDING,   /**< TX process OK with ACK pending flag*/
    PHY_LINK_TX_SUCCESS,        /**< MAC TX complete MAC will make decission to enter wait ack or TX Done state*/
    PHY_LINK_TX_FAIL,           /**< Link TX process fail*/
    PHY_LINK_CCA_FAIL,          /**< RF Link CCA process fail */
} phy_link_tx_status_e;


typedef enum {
    PHY_EXTENSION_CTRL_PENDING_BIT, /**< Control MAC Pending Bit for INDirect data */
    PHY_EXTENSION_READ_LAST_ACK_PENDING_STATUS, /**< Control MAC Pending Bit for INDirect data */
    PHY_EXTENSION_SET_CHANNEL,  /**< Net librarys channel set */
    PHY_EXTENSION_READ_CHANNEL_ENERGY, /**< RF interfacen ED scan energy read */
    PHY_EXTENSION_READ_LINK_STATUS, /**< Net library could read link status*/
    PHY_EXTENSION_CONVERT_SIGNAL_INFO, /**< Convert signal info */
} phy_extension_type_e;

typedef enum {
    PHY_MAC_48BIT, /**< IPv4/IPv6/BLE link layer address for ethernet this optional */
    PHY_MAC_64BIT, /**< RF / PLC link layer address*/
    PHY_MAC_16BIT, /**< RF interface short address*/
    PHY_MAC_PANID, /**< RF interface 16-Bit PAN-ID*/
} phy_address_type_e;

typedef enum phy_link_type_e {
    PHY_LINK_ETHERNET_TYPE,         /**< Standard IEEE 802 Ethernet */
    PHY_LINK_15_4_2_4GHZ_TYPE,      /**< Standard 802.15.4 2.4GHz radio */
    PHY_LINK_15_4_SUBGHZ_TYPE,      /**< Standard 802.15.4 subGHz radio 868 /915MHz */
    PHY_LINK_SW_RADIO_TYPE,         /**< Proprietary radio Frequency controlled by SW */
    PHY_LINK_PLC_TYPE,              /**< PLC PHY link */
    PHY_LINK_TUN,                   /**< Tunnel interface for Linux TUN, rf network driver over serial bus or just basic application to application data flow */
} phy_link_type_e;

typedef enum data_protocol_e {
    LOCAL_SOCKET_DATA = 0,          /**< 6LoWPAN library Local Socket Data */
    INTERFACE_DATA = 1,             /**< 6LoWPAN library Interface Internal used protocol*/
    PHY_LAYER_PAYLOAD = 2,          /**< Phy Layer Data selection or handler */
    IPV6_DATAGRAM = 3,              /**< IP layer data or tun driver request data */
    UNKNOWN_PROTOCOL = 4            /**< Not supported Protocol ID */
} data_protocol_e;


typedef enum driver_data_request_e {
    PHY_LAYER_PAYLOAD_DATA_FLOW,    /**< Phy Layer Data  */
    IPV6_DATAGRAMS_DATA_FLOW,       /**< IP layer data or tun driver request data */
} driver_data_request_e;

typedef struct phy_device_channel_info_s {
    uint8_t channel_count;      /**< Define how many channels is available */
    uint32_t channel_mask;      /**< Define 27 channels for 802.15.4 radios */
} phy_device_channel_info_s;

/**
 * Types of signal quality indication desired by various link protocols. Some are
 * really statistical, but a driver should ideally be able to create an estimate
 * based on its lqi/dbm numbers, eg to bootstrap a statistic calculation.
 */
typedef enum phy_signal_info_type_e {
    PHY_SIGNAL_INFO_ETX,            /**< Expected Transmissions, unsigned 16-bit fixed-point ETX*128 [1..512), eg Zigbee IP + RFC 6719 */
    PHY_SIGNAL_INFO_IDR,            /**< Inverse Delivery Ratio, unsigned 16-bit fixed-point IDR*32*256 [1..8), eg MLE draft 06 */
    PHY_SIGNAL_INFO_LINK_MARGIN,    /**< Link margin, unsigned 16-bit fixed-point dB*256, [0..255), eg Thread routing draft */
} phy_signal_info_type_e;

typedef struct phy_signal_info_s {
    phy_signal_info_type_e type;    /**< Signal info type desired */
    uint8_t lqi;                    /**< Quality passed to arm_net_phy_rx */
    int8_t dbm;                     /**< Strength passed to arm_net_phy_rx */
    uint16_t result;                /**< Resulting signal information */
} phy_signal_info_s;

typedef struct phy_device_driver_s {
    phy_link_type_e link_type;                                      /**< Define driver types */
    driver_data_request_e data_request_layer;                       /**< Define interface Data OUT protocol */
    uint8_t *PHY_MAC ;                                              /**< Pointer to 64-bit or 48-bit MAC address */
    char *driver_description;                                       /**< Short driver platform description. Need end to zero */
    uint16_t phy_MTU;                                               /**< Define MAX PHY layer MTU size */
    uint8_t phy_tail_length;                                        /**< Define PHY driver needed TAIL Length */
    uint8_t phy_header_length;                                      /**< Define PHY driver needed Header length before PDU */
    int8_t (*state_control)(phy_interface_state_e, uint8_t);        /**< Function pointer for control PHY driver state */
    int8_t (*tx)(uint8_t *, uint16_t, uint8_t, data_protocol_e);    /**< Function pointer for  PHY driver write operation */
    int8_t (*address_write)(phy_address_type_e , uint8_t *);        /**< Function pointer for  PHY driver address write */
    int8_t (*extension)(phy_extension_type_e, uint8_t *);           /**< Function pointer for  PHY driver extension control */
    phy_device_channel_info_s *link_channel_info;                   /**< Pointer for wireless channel info */
} phy_device_driver_s;


/** Net Library PHY Interface API*/
/**
 * \brief This function register device driver to stack.
 *
 * \param phy_driver pointer to device driver structure
 *
 * \return >= 0 Device driver ID
 * \return < 0 Means register Fail
 *
 */
extern int8_t arm_net_phy_register(phy_device_driver_s *phy_driver);

/**
 * \brief This function is API for DATA RX to stack
 *
 * \param Define received DATA type
 * \param data_ptr pointer to data
 * \param data_len data len
 * \param link_quality radio link quality
 * \param dbm received signal strength
 * \param interface_id device driver ID. That define stack handler
 * \return >= 0 Data RX OK
 * \return < 0 Data push Fail
 *
 */
extern int8_t arm_net_phy_rx(data_protocol_e data_type, const uint8_t *data_ptr, uint16_t data_len, uint8_t link_quality, int8_t dbm, int8_t interface_id);

/**
 * \brief This function is API for Indicate device driver write operation is done
 *
 * \param interface_id device driver ID
 * \param tx_handle Unique Handle for packet
 * \param status define TX process status
 * \param cca_retry CCA attemps count for this TX process
 * \param tx_retry TX retry count for this packet
 *
 * \return >= 0 Process OK
 * \return < 0 Process handler Fail
 *
 */
extern int8_t arm_net_phy_tx_done(int8_t driver_id, uint8_t tx_handle, phy_link_tx_status_e status, uint8_t cca_retry, uint8_t tx_retry);

#ifdef __cplusplus
}
#endif
#endif /* ARM_HAL_PHY_H_ */
