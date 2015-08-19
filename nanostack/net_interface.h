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

#ifndef NET_INTERFACE_H_
#define NET_INTERFACE_H_

#include "ns_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/** Platform is not valid for ZigBeeIP library */
#define ZIGBEE_IP_INVALID_PART -2

typedef enum arm_nwk_interface_status_type_e {
    ARM_NWK_BOOTSTRAP_READY = 0, /**< Interface configured Bootstrap is ready*/
    ARM_NWK_RPL_INSTANCE_FLOODING_READY, /**< RPL instance have been flooded */
    ARM_NWK_SET_DOWN_COMPLETE, /**< Interface DOWN command successfully */
    ARM_NWK_NWK_SCAN_FAIL,  /**< Interface have not detect any valid network*/
    ARM_NWK_IP_ADDRESS_ALLOCATION_FAIL, /*!*< IP address allocation fail(ND, DHCPv4 or DHCPv6 */
    ARM_NWK_DUPLICATE_ADDRESS_DETECTED, /*!*< User specific GP16 was not valid */
    ARM_NWK_AUHTENTICATION_START_FAIL, /**< No valid Authentication server detected behind access point */
    ARM_NWK_AUHTENTICATION_FAIL,    /**< Network authentication fail by Handshake */
    ARM_NWK_NWK_CONNECTION_DOWN, /*!*< No connection between Access point or Default Router */
    ARM_NWK_NWK_PARENT_POLL_FAIL, /*!*< Sleepy host poll fail 3 time */
    ARM_NWK_PHY_CONNECTION_DOWN, /*!*< Interface PHY cable off or serial port interface not respond anymore */
} arm_nwk_interface_status_type_e;

typedef enum arm_library_event_type_e {
    ARM_LIB_TASKLET_INIT_EVENT = 0, /**< Tasklet Init come always when generate tasklet*/
    ARM_LIB_NWK_INTERFACE_EVENT,    /**< Interface Bootstrap  or state update event */
    ARM_LIB_SYSTEM_TIMER_EVENT, /*!*< System Timer event */
    APPLICATION_EVENT, /**< Application specific event */
} arm_library_event_type_e;


/**
 *  Socket event description:
 *
 *  8-bit variable where four MSB bits describes the event type and
 *  four LSB bits describes the socket that has received the event.
 *
 *      Type   Socket ID
 *      ----   ----
 *      xxxx   xxxx
 *
 */


/** Socket type exceptions */
/** Socket event Mask */
#define SOCKET_EVENT_MASK                   0xF0
/** Data received*/
#define SOCKET_DATA                         0
/** TCP connection ready */
#define SOCKET_BIND_DONE                    1 << 4
/** TCP connection failed */
#define SOCKET_BIND_FAIL                    2 << 4
/** TCP connection authentication failed */
#define SOCKET_BIND_AUTH_FAIL               3 << 4
/** TCP state from listen to establishment */
#define SOCKET_SERVER_CONNECT_TO_CLIENT     4 << 4
/** Socket data send fail */
#define SOCKET_TX_FAIL                      5 << 4
/** TCP connection closed */
#define SOCKET_CONNECT_CLOSED               6 << 4
/** TCP connection closed because no ACK received */
#define SOCKET_CONNECT_FAIL_CLOSED          7 << 4
/** No route available to the destination */
#define SOCKET_NO_ROUTE                     8 << 4
/** Socket TX Done */
#define SOCKET_TX_DONE                      9 << 4

/*!
 * \enum net_interface_type_e
 * \brief Interface type definition.
 */
typedef enum {
    NET_INTERFACE_ETHERNET,     /**< IPv4 or IPv6*/
    NET_INTERFACE_WIFI,         /**< WIFI RF interface*/
    NET_INTERFACE_RF_6LOWPAN,   /**< RF 6LoWPAN interface*/
    NET_INTERFACE_VIRTUAL,  /**< IPv6 over anyserial interface */
} net_interface_type_e;

/** Network Interface IDs */
typedef enum {
    IF_6LoWPAN,
    IF_IPV6,
    IF_LOCAL_SOCKET,
} nwk_interface_id;

/*!
 * \enum net_security_t
 * Network Security Levels
 * */
typedef enum net_security_t {
    NW_NO_SECURITY = 0,                       /**< No Security*/
    NW_SECURITY_LEVEL_MIC32 = 1,              /**< 32-bit Mic verify no Encoding. */
    NW_SECURITY_LEVEL_MIC64 = 2,              /**< 64-bit Mic verify no Encoding. */
    NW_SECURITY_LEVEL_MIC128 = 3,             /**< 128-bit Mic verify no Encoding. */
    NW_SECURITY_LEVEL_ENC = 4,                /**< AES Encoding without MIC. */
    NW_SECURITY_LEVEL_ENC_MIC32 = 5,          /**< 32-bit Mic verify with Encoding. */
    NW_SECURITY_LEVEL_ENC_MIC64 = 6,          /**< 64-bit Mic verify with Encoding. */
    NW_SECURITY_LEVEL_ENC_MIC128 = 7          /**< 128-bit Mic verify with Encoding. */
} net_security_t;

/*!
 * \enum net_address_t
 * \brief addresses for arm_net_address_get().
 */
typedef enum net_address_t {
    ADDR_IPV6_GP,             /**< Node Default Global address */
    ADDR_IPV6_GP_SEC,         /**< Node Secondary Global address */
    ADDR_IPV6_LL              /**< Node Default Link Local address */
} net_address_t;

typedef enum net_mac_address_t {
    ADDR_MAC_SHORT16,             /**< Nodes 16-bit Short Address */
    ADDR_MAC_LONG64,              /**< IP layer EUID64 which based on MAC layer 64-bit long Addressafter  U/I -bit conversion  */
} net_mac_address_t;

/*!
 * \enum net_tls_cipher_e
 * \brief TLS cipher mode enumeration types.
 */
typedef enum {
    NET_TLS_PSK_CIPHER,         /**< Network Authentication support only PSK */
    NET_TLS_ECC_CIPHER,         /**< Network Authentication support only ECC */
    NET_TLS_PSK_AND_ECC_CIPHER, /**< Network Authentication support PSK & ECC */
} net_tls_cipher_e;

/*!
 * \enum net_pana_session_mode_e
 * \brief PANA session cache support.
 */
typedef enum {
    NET_PANA_SINGLE_SESSION,        /**< Client keep track only 1 Pana session data, Default use case */
    NET_PANA_MULTI_SESSION,         /**< Client support many Start network coordinator session data */
} net_pana_session_mode_e;

/*!
 * \enum net_6lowpan_link_layer_sec_mode_e
 * \brief 6Lowpan network Security & authentication modes.
 */
typedef enum {
    NET_SEC_MODE_NO_LINK_SECURITY,      /**< Security Disabled at Link Layer � DEFAULT */
    NET_SEC_MODE_PSK_LINK_SECURITY,     /**< Link Security by PSK key*/
    NET_SEC_MODE_PANA_LINK_SECURITY, /**< PANA network authentication define link KEY */
} net_6lowpan_link_layer_sec_mode_e;


/*!
 * \enum net_6lowpan_mode_e
 * \brief 6LoWPAN bootstrap modes.
 */
typedef enum {
    NET_6LOWPAN_BORDER_ROUTER,  /**< Root device for 6LoWPAN ND */
    NET_6LOWPAN_ROUTER,         /**< Router device */
    NET_6LOWPAN_HOST,           /**< Host device � DEFAULT setting */
    NET_6LOWPAN_SLEEPY_HOST,    /**< Sleepy Host device */
    NET_6LOWPAN_NETWORK_DRIVER, /**< 6Lowpan Radio Host device no bootstrap */
    NET_6LOWPAN_SNIFFER         /**< Sniffer device no bootstrap */
} net_6lowpan_mode_e;

/*!
 * \enum net_6lowpan_mode_extension_e
 * \brief 6LoWPAN Extension modes.
 */
typedef enum {
    NET_6LOWPAN_ND_WITHOUT_MLE,         /**< 6LoWPAN ND without MLE */
    NET_6LOWPAN_ND_WITH_MLE,            /**< 6LoWPAN ND with MLE */
    NET_6LOWPAN_THREAD,                 /**< 6LoWPAN Thread with MLE Attached */
    NET_6LOWPAN_ZIGBEE_IP               /**< 6LoWPAN ZigBeeIP setup */
} net_6lowpan_mode_extension_e;


/*!
 * \enum net_ipv6_mode_e
 * \brief IPv6 bootstrap modes.
 */
typedef enum {
    NET_IPV6_BOOTSTRAP_STATIC,  /**< Application defines the IPv6 prefix*/
    NET_IPV6_BOOTSTRAP_AUTONOMOUS
}
net_ipv6_mode_e;

/*!
 * \struct link_layer_setups_s
 * \brief Network Cordinator Parameter List.
 * Structure is used to read network parameter for warm start.
 */
typedef struct link_layer_setups_s {
    uint16_t PANId;            /**< Network PAN-ID */
    uint8_t LogicalChannel;    /**< Network Logical Channel */
    net_mac_address_t addr_mode;   /**< Coordinator Address mode */
    uint8_t address[8];        /**< Coordinator Address */
    uint8_t sf;                /**< Network super frame setup */
} link_layer_setups_s;

/*!
 * \struct link_layer_address_s
 * \brief Network MAC address info.
 * Structure is used to read link layer Adress.
 */
typedef struct link_layer_address_s {
    uint16_t PANId;            /**< Network PAN-ID */
    uint16_t mac_short;        /**< MAC short address if <0xfffe then is valid */
    uint8_t mac_long[8];       /**< MAC long address (EUI-48 for Ethernet; EUI-64 for IEEE 802.15.4) */
    uint8_t iid_eui64[8];      /**< IPv6 Interface Identifier based on EUI-64 */
} link_layer_address_s;

/*!
 * \struct network_layer_address_s
 * \brief Network layer parent address info.

 */
typedef struct network_layer_address_s {
    uint8_t border_router[16]; /**< ND Border Router Address */
    uint8_t prefix[8];        /**< Long64-bit Network ID */
} network_layer_address_s;

/*!
 * \enum net_6lowpan_gp_address_mode_e
 * \brief 6LoWPAN stack address modes.
 */

typedef enum {
    NET_6LOWPAN_GP64_ADDRESS,       /**< Interface will register only GP64 Address � DEFAULT */
    NET_6LOWPAN_GP16_ADDRESS,       /**< Interface will register only GP16 Address */
    NET_6LOWPAN_MULTI_GP_ADDRESS,   /**< Interface will register GP16 & GP64 Address�s */
} net_6lowpan_gp_address_mode_e;


/*!
 * \struct net_tls_psk_info_s
 * \brief TLS PSK info structure.
 */
typedef struct net_tls_psk_info_s {
    uint32_t key_id;    /**< PSK Key ID can be 0x01-0xffff, storage size is intentionally 32 bits */
    uint8_t key[16];    /**< 128-bit PSK Key  */
} net_tls_psk_info_s;

/*!
 * \struct net_link_layer_psk_security_info_s
 * \brief NETWORK PSK link key structure.
 */
typedef struct {
    uint8_t key_id;             /**< Link Layer PSK Key ID can be 0x01-0xff */
    uint8_t security_key[16];   /**< Link layer 128-bit PSK Key */
} net_link_layer_psk_security_info_s;


/*!
 * \struct arm_certificate_chain_entry_s
 * \brief Certificate chain structure.
 */
typedef struct {
    uint8_t chain_length;           /**< Certificate Chain length, indicate chain length */
    const uint8_t *cert_chain[4];   /**< Certificate Chain pointer List */
    uint16_t cert_len[4];           /**< Certificate length */
    const uint8_t *key_chain[4];    /**< Certificate private key */
} arm_certificate_chain_entry_s;

/**
* /struct ns_keys_t
* /brief Struct for the network keys  used by net_network_key_get
*/
typedef struct ns_keys_t

{
    uint8_t previous_active_network_key[16];    /**<  the key that is currently active when a new key is generated and activated */
    uint8_t previous_active_key_index;           /**<  the index associated to the current_active_network_key */
    uint8_t current_active_network_key[16];     /**<  last generated and activated key */
    uint8_t current_active_key_index;           /**<  the index associated to the current_active_network_key */
} ns_keys_t;


/*!
 * \struct border_router_setup_s
 * \brief 6LoWPAN Border Router information structure.
 */
typedef struct {
    uint8_t channel;                /**< Channel 1-26 802.15.4 Radios */
    uint16_t mac_panid;             /**< Link Layer PAN-id accepts only < 0xfffe  */
    uint16_t mac_short_adr;         /**<  Define 802.15.4 short address if value is <0xfffe that indicate that GP16 is activated at */
    uint8_t beacon_protocol_id;     /**< ZigBeeIP uses always 2 */
    uint8_t network_id[16];         /**< network id 16-bytes, will be used at Beacon payload */
    uint8_t lowpan_nd_prefix[8];    /**< Define ND Default prefix, ABRO, DODAG ID, GP address*/
    uint16_t ra_life_time;          /**< Define ND router lifetime in seconds Recommend value 180+. */
    uint32_t abro_version_num;      /**< ND ABRO version number (0 when starting new ND setup) */
} border_router_setup_s;

/*!
 * \struct network_driver_setup_s
 * \brief 6LoWPAN Radion interface setup.
 */
typedef struct {
    uint16_t mac_panid;                 /**< Link Layer PAN-id accepts only < 0xfffe  */
    uint16_t mac_short_adr;             /**<  Define 802.15.4 short address if value is <0xfffe that indicate that GP16 is activated at */
    uint8_t beacon_protocol_id;         /**< ZigBeeIP uses always 2 */
    uint8_t network_id[16];             /**< network id 16-bytes, will be used at Beacon payload */
    uint8_t beacon_payload_tlv_length; /**< Optional Steering parameters length */
    uint8_t *beacon_payload_tlv_ptr;  /**< Optional Steering parameters */
} network_driver_setup_s;

/**
  * \brief Init 6LoWPAN library
  *
  * \return 0, Init OK
  * \return ZIGBEE_IP_INVALID_PART, Platform is not valid to Run ZigBeeIP Library
  */
extern int8_t net_init_core(void);



/**
 * \brief Create network interface base to IDLE state.
 * \param type Generates network interface type.
 * \param phy_driver_id Unique PHY device driver ID. PHY driver needs to be allocated first.
 * \param interface_name_ptr String pointer to interface name. Need to end to �\0� character. Max length 32 characters including NULL at end.
 *
 * \return >=0 Interface ID (0-127). Application needs to save this information.
 * \return -1 Unsupported interface type.
 * \return -2 Driver is already associated to other interface.
 * \return -3 No memory for interface.
 */
extern int8_t arm_nwk_interface_init(net_interface_type_e type, uint8_t phy_driver_id, char *interface_name_ptr);

/**
 * \brief Set ipv6 interface setup.
 *
 * \param nwk_interface_id Network interface ID
 * \param bootstrap_mode Selected Bootstrap mode:
 *      * NET_IPV6_BOOTSTRAP_STATIC, Application defines the IPv6 prefix
 * \param ipv6_prefix_pointer Pointer to 64 bit IPv6 prefix
 *
 * \return >=0 Bootstrap mode set OK.
 * \return -1 Unknown network ID.
 */
extern int8_t arm_nwk_interface_configure_ipv6_bootstrap_set(int8_t nwk_interface_id, net_ipv6_mode_e bootstrap_mode, uint8_t *ipv6_prefix_pointer);

/**
 * \brief Set network interface bootstrap setup.
 *
 * \param nwk_interface_id Network interface ID
 * \param bootstrap_mode Selected Bootstrap mode:
 *      * NET_6LOWPAN_BORDER_ROUTER, Initialize Border router basic setup
 *      * NET_6LOWPAN_ROUTER, Enable normal 6LoWPAN ND and RPL to bootstrap
 *      * NET_6LOWPAN_HOST, Enable normal 6LoWPAN ND only to bootstrap
 *      * NET_6LOWPAN_SLEEPY_HOST, Enable normal 6LoWPAN ND only to bootstrap
 *      * NET_6LOWPAN_NETWORK_DRIVER, 6Lowpan Radio Host device no bootstrap
 *      * NET_6LOWPAN_SNIFFER, 6Lowpan Sniffer device no bootstrap
 *
 * \param net_6lowpan_mode_extension Define 6LoWPAN MLE and Mode like ZigBeeIP or Thread
 *
 * \return >=0 Bootstrap mode set OK.
 * \return -1 Unknown network ID.
 * \return -2 Unsupported bootstrap type in this library.
 * \return -3 No Memory for 6LoWPAN stack.
 * \return -4 Null pointer parameter
 */
extern int8_t arm_nwk_interface_configure_6lowpan_bootstrap_set(int8_t nwk_interface_id, net_6lowpan_mode_e bootstrap_mode, net_6lowpan_mode_extension_e net_6lowpan_mode_extension);

/**
 * \brief Set network interface link layer parameters.
 *
 * \param nwk_interface_id Network interface ID
 * \param tun_driver_id Driver id FOR PHY data IN & OUT
 * \param channel define network link channel
 * \param link_setup Link layer parameters for NET_6LOWPAN_NETWORK_DRIVER defines NetworkID, PAN-ID Short Address
 *
 * \return >=0 Configure set OK.
 * \return -1 Unknown network ID.
 * \return -2 Interface is active, Bootsrap mode not selected or is not NET_6LOWPAN_NETWORK_DRIVER or NET_6LOWPAN_SNIFFER.
 * \return -3 No Memory for 6LoWPAN stack.
 * \return -4 Null pointer parameter
 */
extern int8_t arm_nwk_interface_network_driver_set(int8_t nwk_interface_id, int8_t tun_driver_id, uint8_t channel, network_driver_setup_s *link_setup);

/**
 * \brief Set configured network interface Global address mode (Border router bootstrap mode can't set this).
 *
 * \param nwk_interface_id Network interface ID
 * \param mode efine 6LoWPAN Global Address register mode::
 *      * NET_6LOWPAN_GP64_ADDRESS, Interface register only GP64
 *      * NET_6LOWPAN_GP16_ADDRESS, Interface register only GP16
 *      * NET_6LOWPAN_MULTI_GP_ADDRESS, Interface register GP16 and GP64 address�s. GP16 is primary address and GP64 is secondary.
 *
 * \param short_address_base Short address base. If application defines value 0-0xfffd 6LoWPAN try to register GP16 address using that address. 0xfffe and 0xffff will generate random 16-bit short address.
 *
 * \param define_new_short_address_at_DAD This parameter is only checked when mode is not NET_6LOWPAN_GP64_ADDRESS and short_address_base is 0-0xfffd. Recommend value is 1 that will enable automatic new address definition at Duplicate Address Detection(DAD). Value 0 will generate Duplicate Adreress Detection error for interface bootstrap.
Border Router Device will not check that part.
 *
 * \return >=0 Bootstrap mode set OK.
 * \return -1 Unknown network ID.
 * \return -2 Illegal for Border Router
 * \return -3 No Memory for 6LoWPAN stack.
 */
extern int8_t arm_nwk_6lowpan_gp_address_mode(int8_t nwk_interface_id, net_6lowpan_gp_address_mode_e mode, uint16_t short_address_base, uint8_t define_new_short_address_at_DAD);

extern int8_t arm_nwk_6lowpan_link_scan_paramameter_set(int8_t nwk_interface_id, uint32_t channel_mask, uint8_t scan_time);

extern int8_t arm_nwk_6lowpan_link_panid_filter_for_nwk_scan(int8_t nwk_interface_id, uint16_t pan_id_filter);

/**
  *  \brief Get current used channel.
  *
  * \return 11-26 = Active channel
  * \return 0 = Radio is closed
  */
extern uint8_t arm_net_get_current_channel(int8_t nwk_interface_id);

/**
 * \brief A function to read pan ID filter.
 * \return 16-bit value indicating a pan ID filter.
 */
extern uint16_t arm_net_get_nwk_pan_id_filter(int8_t nwk_interface_id);
/**
  * \brief Enable/Disable Network ID filter.
  *
  * \param nwk_interface_id Network interface ID.
  * \param nwk_id is a pointer a new network id filter NULL Disable filter.
  *
  * \return 0 on success.
  * \return -1 Unknown network ID.
  * \return -2 Interface Active.
  */
extern int8_t arm_nwk_6lowpan_link_nwk_id_filter_for_nwk_scan(int8_t nwk_interface_id, const uint8_t *nwk_id_filter);
/**
  * \brief Enable/Disable Network protocol ID filter.
  *
  * \param nwk_interface_id Network interface ID.
  * \param protocol_ID is value which filter only supported network protocols (0= Zigbee1.x, 2= ZigBeeIP).
  *
  * \return 0 on success.
  * \return -1 Unknown network ID.
  * \return -2 Interface Active.
  */
extern int8_t arm_nwk_6lowpan_link_protocol_id_filter_for_nwk_scan(int8_t nwk_interface_id, uint8_t protocol_ID);

/**
  * \brief Init & Configure interface security mode.
  *
  * \param nwk_interface_id Network interface ID.
  * \param mode define Link layer security mode.
  *  NET_SEC_MODE_NO_LINK_SECURITY, No security
  *  NET_SEC_MODE_PSK_LINK_SECURITY, Predefined PSK link layer key and ID
  *  NET_SEC_MODE_PANA_LINK_SECURITY, Define Pana Bootstrap Network Authentication
  *
  * \param sec_level Define security level only will be checked when mode is not NET_SEC_MODE_NO_LINK_SECURITY
  * \param psk_key_info pointer for PSK link layer keys only checked when mode is NET_SEC_MODE_PSK_LINK_SECURITY
  *
  * \return 0 on success.
  */

extern int8_t arm_nwk_link_layer_security_mode(int8_t nwk_interface_id, net_6lowpan_link_layer_sec_mode_e  mode, uint8_t sec_level, net_link_layer_psk_security_info_s *psk_key_info);

/**
  * \brief Init & Configure interface PANA network Client.
  *
  * \param nwk_interface_id Network interface ID.
  * \param cipher_mode define TLS 1.2 Cipher mode PSK, ECC or both.
  * \param psk_key_id PSK KEY id for PSK Setup
  *
  * \return 0 on success.
  * \return -1 Unknown network ID or PANA is not supported at this library.
  * \return -2 Interface Active.
  */
extern int8_t arm_pana_client_library_init(int8_t nwk_interface_id, net_tls_cipher_e cipher_mode, uint32_t psk_key_id);

/**
  * \brief Init & Configure interface PANA network Server.
  *
  * \param nwk_interface_id Network interface ID.
  * \param cipher_mode define TLS 1.2 Cipher mode PSK, ECC or both.
  * \param key_material pointer
  * \param time_period_before_activate_key Quard period after succesfully key delivery phase before key will be activated by server.
  *
  * \return 0 on success.
  * \return -1 Unknown network ID.
  * \return -2 Interface Active.
  */
extern int8_t arm_pana_server_library_init(int8_t nwk_interface_id, net_tls_cipher_e cipher_mod, uint8_t *key_material, uint32_t time_period_before_activate_key);

/**
  * \brief Client Manually Pana client key Pull for test purpose.
  *
  * \param nwk_interface_id Network interface ID.
  *
  * \return 0 on success.
  * \return -1 Unknown network ID.
  */
extern int8_t arm_pana_client_key_pull(int8_t nwk_interface_id);


/**
 * \brief Trig Network Key Update process
 *
 * Function will deliver new network key to all ZigBee Routers which have registered GP address to server.
 * Function call always trig new key-id. Key delivery will be started between 300ms interval between nodes.
 * This function does not cause any traffic  if Server does not have any Routers device sessions.
 *
 * \param nwk_interface_id Network interface ID.
 * \param network_key_material pointer to new 128-bit key material or NULL generate random key.
 *
 * \return 0 Key Update process OK
 * \return -1 Pana server is not Initialized yet
 * \return -2 Old Key Update still active
 * \return -3 Memory allocation fail
 */
extern int8_t arm_pana_server_key_update(int8_t nwk_interface_id, uint8_t *network_key_material);

/**
 * \brief Activate new key material quicker before standard timeout
 *
 * This function main usecase is only for testing
 *
 * \param nwk_interface_id Network interface ID.
 *
 * \return 0 Key activate process OK
 * \return -1 No Pending key update
 * \return -2 PANA server is not initialized or Pana server api is disabled with this library
 */
extern int8_t arm_pana_activate_new_key(int8_t nwk_interface_id);


/**
 * \brief Read Pana server security key material
 *
 * previous_active_network_key is information is only valid when current_active_key_index is bigger than 1.
 *
 *\param key pointer for store keymaterial information.
 *
 * \return 0 Key Read OK
 * \return -1 Pana server key material not available
 */
extern int8_t arm_network_key_get(int8_t interface_id, ns_keys_t *key);

/**
 * \brief Start network interface bootstrap.
 *
 * \param nwk_interface_id Network interface ID
 *
 *
 * \return >=0 Bootstrap Start OK.
 * \return -1 Unknown network ID.
 * \return -2 Not configured.
 * \return -3 Active.
 */
extern int8_t arm_nwk_interface_up(int8_t nwk_interface_id);
/**
 * \brief Stop and set interface to idle.
 *
 * \param nwk_interface_id Network interface ID
 *
 *
 * \return >=0 Process OK.
 * \return -1 Unknown network ID.
 * \return -3 Not ActiveActive.
 */
extern int8_t arm_nwk_interface_down(int8_t nwk_interface_id);

/**
 * \brief Define Border Router MAC & 6LoWPAN ND setup for selected interface
 *
 * \param nwk_interface_id Network interface ID
 * \param border_router_setup_ptr Pointer to MAC & 6LoWPAN ND setup
 *
 */
extern int8_t arm_nwk_6lowpan_border_router_init(int8_t nwk_interface_id, border_router_setup_s *border_router_setup_ptr);
/**
 * \brief Add context at 6Lowpan interface configure state
 *
 *
 * \param Network Interface ID
 * \param c_id_flags bit 4 indicate compress support and bit 0-3 context ID
 * \param context_len Context length in bits need to be 64-128
 * \param ttl contexts time to live value in minutes
 * \param context_ptr pointer to full 128-bit memory area.
 *
 * \return 0 context update OK
 * \return -1 No memory for new Context
 * \return -2 Border Router base not allocated
 * \return -3 Given parameter fail (c_id_flags > 0x1f or contex_len < 64)
 * \
 */
extern int8_t arm_nwk_6lowpan_border_router_context_update(int8_t nwk_interface_id, uint8_t c_id_flags, uint8_t context_len, uint16_t ttl, uint8_t *context_ptr);
/**
 * \brief Update Run time configured context
 *
 * Function can change contexts Compress state or Time to live value.
 * Function will trig New ABRO version number, which indicates that ND parameters are updated.
 *
 * \param Network Interface ID
 * \param c_id context ID stack will check first 4 bits , supported values 0-15
 * \param compress_mode 0=Compress disabled otherwise compress enabled
 * \param ttl contexts time to live value in minutes
 *
 * \return 0 Update OK
 * \return -1 Update fail by Router state
 *
 */
extern int8_t arm_nwk_6lowpan_border_router_context_parameter_update(int8_t nwk_interface_id, uint8_t c_id, uint8_t compress_mode, uint16_t ttl);
/**
 * \brief Delete Allocated Context by id
 *
 * \param Network Interface ID
 * \param c_id 4-bit context id which will be deleted
 *
 * \return 0 Context Delete OK.
 * \return -1 Delete process fail.
 */
extern int8_t arm_nwk_6lowpan_border_router_context_remove_by_id(int8_t nwk_interface_id, uint8_t c_id);
/**
 * \brief Update ND ABRO version number
 *
 * \param Network Interface ID
 *
 * \return 0 ABRO version update OK
 * \return -1 ABRO update fail( Interface is not Up yet or Border Router base is not allocated)
 */
extern int8_t arm_nwk_6lowpan_border_router_configure_push(int8_t nwk_interface_id);

extern int8_t arm_nwk_6lowpan_border_route_nd_default_prefix_timeout_set(int8_t nwk_interface_id, uint32_t time);

/**
 * \brief A function to read network layer configurations.
 * \param network_params is a pointer to the structure to where the network layer configs are written to.
 * \return 0 on success.
 * \return -1 if PAN coordinator is not known.
 */
int8_t arm_nwk_param_read(int8_t interface_id, link_layer_setups_s *network_params);


/**
 * \brief A function to read MAC PAN-ID, Short address & EUID64
 * \param mac_params is a pointer to the structure to where the mac address are written to.
 * \return 0 on success.
 * \return -1 .
 */
int8_t arm_nwk_mac_address_read(int8_t interface_id, link_layer_address_s *mac_params);


/**
 * \brief A function to read 6LoWPAN ND border router address and NWK prefix
 * \param mac_params is a pointer to the structure to where the mac address are written to.
 * \return 0 on success.
 * \return -1 .
 */
int8_t arm_nwk_nd_address_read(int8_t interface_id, network_layer_address_s *nd_params);


/**
 * \brief A function to read networking address informations.
 * \param addr_id identifies the address information type to be read.
 * \param address is a pointer to a structure to where the address information is written to.
 * \return 0 on success, -1 on failure.
 */
extern int8_t arm_net_address_get(int8_t nwk_interface_id, net_address_t addr_id, uint8_t *address);

/**
 * \brief A function to read networking address informations one by one.
 * \param addr_id identifies the address information type to be read.
 * \param integer pointer, which will be incremented every call. Start looping with n=0.
 * \param address_buffer[16] there will be address copied
 * \return 0 on success, 
 * \return -1 no more addresses available.
 */
extern int8_t arm_net_address_list_get_next(int8_t nwk_interface_id, int *n, uint8_t address_buffer[16]);

/**
 * \brief A function to read network Interface address count.
 * \param nwk_interface_id Id to interface.
 * \param address_count Pointer where address count will be saved.
 *
 * \return zero on success, -1 on errors.
 */
extern int8_t arm_net_interface_address_list_size(int8_t nwk_interface_id, uint16_t *address_count);

/**
 * \brief A function to read network Interface.
 * \param nwk_interface_id Id to interface.
 * \param address_buf_size Indicate buffer size in bytes minimal is 16 bytes.
 * \param address_buffer pointer where stack save address one by one.
 * \param writed_address_count pointer where stack save how many address is writed behind address_buffer.
 *
 * \return zero on success, -1 on errors.
 */
extern int8_t arm_net_address_list_get(int8_t nwk_interface_id, uint8_t address_buf_size, uint8_t *address_buffer, int *writed_address_count);

/** Border Router ND NVM update types */
#define ND_PROXY_CONTEXT_NVM_UPDATE         0 /* ND Context Update, 20  bytes data behind pointer */
#define ND_PROXY_CONTEXT_FLAGS_NVM_UPDATE   1 /* ND Context Update flags update */
#define ND_PROXY_CONTEXT_NVM_REMOVE         2 /* ND Context Removed */
#define ND_PROXY_PREFIX_NVM_UPDATE          3 /* ND Prefix Update */
#define ND_PROXY_ABRO_VERSION_NVM_UPDATE    4 /* ND ABRO version updated */
/**
 * \brief Border Router ND setup NVM update callback set
 *
 * \param passed_fptr function pointer to ND NVM update process
 *
 * \return 0 ND NVM operation Init OK.
 * \return -1 No memory for NVM buffer.
 */
extern int8_t border_router_nd_nvm_callback_set(void (*passed_fptr)(uint8_t , uint8_t *));
/**
 * \brief Load context from NVM at ZigBeeIP interface configure state
 *
 *
 * \param Network Interface ID
 * \param context_data pointer to proper builded 20 bytes update array.
 *
 * \return 0 context reload OK
 * \return <0 Load fail.
 */
extern int8_t arm_nwk_6lowpan_border_router_nd_context_load(int8_t nwk_interface_id, uint8_t *contex_data);  //NVM

extern int8_t arm_bootstrap_certificate_chain_set(arm_certificate_chain_entry_s *chain_info);
extern int8_t arm_network_certificate_chain_set(arm_certificate_chain_entry_s *chain_info);
extern int8_t arm_transport_certificate_chain_set(arm_certificate_chain_entry_s *chain_info);

/**
 * \brief Add PSK key to TLS library
 *
 * \param key_ptr pointer to 16 byte long key array
 * \param key_id PSK key ID
 *
 * \return 0 = success
 * \return -1 = failure
 */
extern int8_t arm_tls_add_psk_key(uint8_t *key_ptr, uint16_t key_id);

/**
 * \brief Remove PSK key from TLS library
 *
 * \param key_id PSK key ID
 *
 * \return 0 = success
 * \retun -1 = failure
 */
extern int8_t arm_tls_remove_psk_key(uint16_t key_id);

/**
 * \brief Check if PSK key id exists.
 *
 * \param key_id PSK key ID
 *
 * \return 0 = success
 * \retun -1 = failure
 */
extern int8_t arm_tls_check_key(uint16_t key_id);

void arm_print_routing_table(void);

void arm_ncache_flush(void);

void arm_print_neigh_cache(void);

/**
  * \brief Get the library version information.
  *
  * \param *ptr Pointer where the data is stored. Required size is 20 bytes.
  *
  *
  * The array containing the version information has the following structure.
  *
  *  | Platform type | Version | Build ID |
  *  | :-----------: | :----------------: |
  *  | 1 byte        |  1 byte | 4 bytes  |
  *
  */
extern void net_get_version_information(uint8_t *ptr);
#ifdef __cplusplus
}
#endif
#endif /* NET_INTERFACE_H_ */
