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

#ifndef THREAD_MANAGEMENT_IF_H_
#define THREAD_MANAGEMENT_IF_H_

#include "ns_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Current protocol version for Thread implementation.
 */
#define THREAD_BEACON_PROTOCOL_ID       3
#define THREAD_BEACON_PROTOCOL_VERSION  1
/**
 * Thread network configuration.
 *
 * This structure can be given in startup if static configuration is used.
 * This data can also be read after joining to thread network is done.
 * if this data is not given to the stack the device will start commissioning process to Join in thread network.
 *
 * */
typedef struct link_configuration {
    uint8_t name[16]; /**< Name of the Thread network*/
    uint8_t master_key[16]; /**< Master key of the thread network*/
    uint8_t PSKc[16]; /**< PSKc value that is calculated from commissioning credentials credentials,XPANID and network name*/
    uint8_t mesh_local_ula_prefix[8]; /**< Mesh local ula prefix*/
    uint8_t mesh_local_eid[8]; /**< Mesh local extented id*/
    uint8_t extented_pan_id[8]; /**< Extended pan id*/
    uint8_t extended_random_mac[8]; /**< Extended random mac which is generated during commissioning*/
    uint8_t steering_data[16]; /**< Dynamic steering data set by commissioning device max length 16*/
    uint8_t steering_data_len; /**< Length of current steering data*/
    char *PSKc_ptr; /**< Commissioning credentials @TODO think if we need the actual credentials*/
    uint8_t PSKc_len;
    uint64_t time_stamp;/**< commissioning data set timestamp*/
    uint32_t key_rotation; /**< Key rotation time in hours*/
    uint32_t key_sequence; /**< Key sequence counter */
    uint16_t panId; /**< network id*/
    uint8_t Protocol_id; /**< current protocol id*/
    uint8_t version; /**< current protocol version*/
    uint8_t rfChannel; /**< current rf channel*/
    uint8_t securityPolicy; /**< Commission Security Policy*/
} link_configuration_s;

/*
 * Mandatory Device information
 *
 * This information is required if commissioning is enabled for this device.
 */
typedef struct {
    uint8_t eui64[8];/**< eui64 of the device. This field is used to identify device when joining to network Mandatory*/
    char *PSKd_ptr;/**< Device credentials used to authenticate device to commissioner Mandatory  length 6-32*/
    char *provisioning_uri_ptr;/**< Provisioning url max 64 bytes*/
    char *vendor_name_ptr;/**< Vendor name optional max 32 bytes*/
    char *vendor_model_ptr;/**< Vendor model optional max 32 bytes*/
    char *vendor_sw_version_ptr;/**<  Vendor SW version optional max 16 bytes*/
    uint8_t vendor_stack_version[6];/**< Vendor stack version optional all 0 indicates not set*/
    uint8_t *vendor_data_ptr;/**<  optional Array max 64 bytes*/
    uint8_t vendor_data_len;/**<  optional Array length max 64 bytes*/
    bool leaderCap; /**< Define Leader Capability */
} device_configuration_s;

/**
 * Set Thread Security Material. Terms are defined in Thread security specification
 *
 * \param interface_id Network Interface
 * \param enableSecurity Boolean for enable security or disable
 * \param thrMasterKey Master Key material which will be used for generating new key
 * \param thrKeySequenceCounter Periodic counter used to generate new MAC and MLE keys
 * \param thrKeyRotation Update period (in seconds) for thrKeySequenceCounter
 *
 * return 0, ADD OK
 * return <0 Add Not OK
 */
int thread_managenet_security_material_set(int8_t nwk_interface_id, bool enableSecurity, uint8_t *thrMasterKey, uint32_t thrKeySequenceCounter, uint32_t thrKeyRotation);

/**
 * Increment Thread key sequence counter
 *
 * \param nwk_interface_id Network Interface
 *
 * return 0, OK
 * return <0 Error
 */
int thread_managenet_increment_key_sequence_counter(int8_t nwk_interface_id);

/**
 * Set new Thread key sequency counter
 *
 * Call define new key and next key same time
 *
 * \param nwk_interface_id Network Interface
 * \param thrKeySequenceCounter this need to be bigger than current sequence
 *
 * return 0, OK
 * return <0 Error
 */
int thread_managenet_key_sequence_counter_update(int8_t nwk_interface_id, uint32_t thrKeySequenceCounter);

//Leader API to add route, DHCPv6 or 6LoWPAN Contexts This API is called also by TMF if Thread service register Command type have been RX.

/**
 * Add new route information to Network Data
 *
 * \param interface_id Network Interface
 * \param route_prefix_ptr pointer to route prefix
 * \param prefix_length indicate prefix pointer valid information in bits
 * \param stable_service Boolean true generate stable service, false temporarily
 *
 * return 0, ADD OK
 * return <0 Add Not OK
 */
int thread_managenet_add_route(int8_t interface_id, uint8_t *route_prefix_ptr, uint8_t prefix_length, bool stable_service);

/**
 * Delete route information to Network Data
 *
 * \param interface_id Network Interface
 * \param route_prefix_ptr pointer to route prefix
 * \param prefix_length indicate prefix pointer valid information in bits
 *
 * return 0, DEL OK
 * return <0 DEL Not OK
 */
int thread_managenet_del_route(int8_t interface_id, uint8_t *route_prefix_ptr, uint8_t prefix_length);

/**
 * Add new DHCPV6 server information to Network Data
 *
 * \param interface_id Network Interface
 * \param prefix_ptr pointer DHCPv6 Server Given Prefix
 * \param prefix_length indicate prefix pointer valid information in bits
 * \param stable_service Boolean true generate stable service, false temporarily
 *
 * return 0, ADD OK
 * return <0 Add Not OK
 */
int thread_managenet_add_dhcpv6_server(int8_t interface_id, uint8_t *prefix_ptr, uint8_t prefix_length, bool stable_service);

/**
 * Del DHCPV6 server information to Network Data
 *
 * \param interface_id Network Interface
 * \param prefix_ptr pointer DHCPv6 Server Given Prefix
 * \param prefix_length indicate prefix pointer valid information in bits
 *
 * return 0, Del OK
 * return <0 Del Not OK
 */
int thread_managenet_del_dhcpv6_server(int8_t interface_id, uint8_t *prefix_ptr, uint8_t prefix_length);

/**
 * Push DHCPV6 local server information to Network Data
 *
 * \param interface_id Network Interface
 *
 * return 0, Push OK
 * return <0 Push Not OK
 */
int thread_managenet_publish_local_services(int8_t interface_id);

/**
 * Delete DHCPV6 local server information to Network Data
 *
 * \param interface_id Network Interface
 *
 * return 0, Push OK
 * return <0 Push Not OK
 */
int thread_managenet_delete_local_services(int8_t interface_id);


/**
 * Add new 6LoWPAN contexts information to Network Data
 *
 * \param interface_id Network Interface
 * \param prefix_ptr pointer 6LoWPAN Contexts
 * \param prefix_length indicate prefix pointer valid information in bits
 * \param cid Define 4-bit contexts ID
 * \param compress_supported true enable contexts compress, false disable
 *
 * return 0, ADD OK
 * return <0 Add Not OK
 */
int thread_managenet_add_contexts(int8_t interface_id, uint8_t *prefix_ptr, uint8_t prefix_length, uint8_t cid, bool compress_supported);

/**
 * Node can read leader ULA16 address
 *
 * \param interface_id Network Interface
 * \param address_buffer pointer where address will be copied
 * return 0, Read OK
 * return <0 Read fail
 */
int thread_managenet_read_leader_ula(int8_t interface_id, uint8_t *address_buffer);

/**
 * Leader Kick Router out from Network
 *
 * \param interface_id Network Interface
 * \param
 * return 0, Remove OK
 * return <0 Remove fail
 */
int thread_managenet_remove_router_by_id(int8_t interface_id, uint8_t routerId);


int thread_managenet_print_network_data(int8_t interface_id);

int thread_management_get_my_ml64(int8_t interface_id, uint8_t *addressPtr);
int thread_management_get_my_ml16(int8_t interface_id, uint8_t *addressPtr);
int thread_management_get_my_ml_prefix(int8_t interface_id, uint8_t *prefix);
int thread_management_get_parent_address(int8_t interface_id, uint8_t *addressPtr);
int thread_management_get_my_ml_prefix_112(int8_t interface_id, uint8_t *prefix);

/**
 * Get configuration of thread network settings.
 *
 * Configuration is pointer to static configuration and only valid in current context.
 *
 * \param interface_id Network Interface
 *
 * return pointer to link configuration
 * return NULL Failure
 */
link_configuration_s *thread_managenet_configuration_get(int8_t interface_id);

/**
 * Get device configuration of thread device settings.
 *
 * Configuration is pointer to static device configuration and only valid in current context.
 *
 * \param interface_id Network Interface
 *
 * return pointer to link configuration
 * return NULL Failure
 */
device_configuration_s *thread_managenet_device_configuration_get(int8_t interface_id);

/**
 * Initialize Thread stack to Node mode
 *
 * \param interface_id Network Interface
 * \param ula_prefix Pointer to Thread network local ULA -prefix
 *
 * return 0, Init OK
 * return <0 Init Fail
 */
int thread_managenet_node_init(
    int8_t interface_id,
    uint32_t channelMask,
    device_configuration_s *device_configuration,
    link_configuration_s *static_configuration);


/**
 * Thread router select threshold values set
 *
 * \param interface_id Network Interface
 * \param upgradeThreshold Set REED up grade to router threshold
 * \param downgradeThreshold Set Router down grade to REED threshold
 *
 * return 0, Set OK
 * return <0 Set Fail
 */
int thread_management_router_select_threshold_values_set(
    int8_t interface_id,
    uint8_t upgradeThreshold,
    uint8_t downgradeThreshold);

/**
 * Thread Leader max router Id limit set
 *
 * This function should use just for test purpose Thread define this by default to 32
 *
 * \param interface_id Network Interface
 * \param maxRouterLimit Min Accepted value is 1 and max 32
 *
 * return 0, Set OK
 * return <0 Set Fail
 */
int thread_management_max_accepted_router_id_limit_set(
    int8_t interface_id,
    uint8_t maxRouterLimit);

/**
 * Thread Leader max child count set
 *
 * This function should be used to set limited amount of children allowed for parent default to 32
 *
 * \param interface_id Network Interface
 * \param maxChildCount Min Accepted value is 0 and max 32
 *
 * return 0, Set OK
 * return <0 Set Fail
 */
int thread_management_max_child_count(
    int8_t interface_id,
    uint8_t maxChildCount);

/**
 * Thread Leader Context ID re use timeout set
 *
 * This function should be used to change default 48 hours to shorter one
 *
 * \param interface_id Network Interface
 * \param timeout Min Accepted value is 60 seconds and max  48 hours (48*3600)
 *
 * return 0, Set OK
 * return <0 Set Fail
 */
int thread_management_set_context_id_resuse_timeout(
    int8_t interface_id,
    uint32_t timeout);

/** Interfaces needed for Native commissioner
 * current design is:
 *  - application configures interface to scan available thread networks where to join
 *      - time passes if we connect it should be good if we connect on-mesh commissioning is used
 *  - application configures interface to begin native commissioner interfaces scans
 *  - application selects some network where to connect
 *  - stack connects to that network -> interface UP event sent
 *  - application starts using commissioning API to send COMM_PET.req message triggering DTLS handshake
 *      - commission api queries leader address and native info and uses the one that works.
 *
 *
 */

typedef struct commissioning_link_configuration {
    uint8_t name[16]; /**< Name of the Thread network utf8 string nul terminated if shorter than 16*/
    uint8_t extented_pan_id[8]; /**< Extented pan id*/
    uint16_t panId; /**< network id*/
    uint8_t Protocol_id; /**< current protocol id*/
    uint8_t version; /**< current protocol version*/
    uint8_t rfChannel; /**< current rf channel*/
} commissioning_link_configuration_s;

/** Native commissioner network scan result callback
 *
 * This callback is called when found some networks that allow native commissioner to join.
 * pointers are valid during this call.
 *
 * \param interface interface id of this thread instance.
 *
 */
typedef void thread_management_native_commissioner_select_cb(int8_t interface_id, uint8_t count, commissioning_link_configuration_s *link_ptr );

/** Native commissioner network scan start
 *
 * starts network scan mode to find networks where device can become as native commissioner.
 * this stops the normal thread joining process and informs the application the list of available networks.
 *
 * \param interface interface id of this thread instance.
 *
 */
int thread_management_native_commissioner_start(int8_t interface_id, thread_management_native_commissioner_select_cb *cb_ptr);

/** Native commissioner network scan stop
 *
 * stops network scan mode and continues the normal joining process.
 *
 * \param interface interface id of this thread instance.
 *
 */
int thread_management_native_commissioner_stop(int8_t interface_id);

/** native commissioner connect
 *
 * Connects to specific thread network to become active native commissioner.
 *
 * This function can be called in any time. when network scan is made the available native commissioner networks are informed
 * using the callback.
 *
 * if connection fails we continue network scans to find new network. After successful connection the interface up event is sent.
 * TODO do we need backup timers or blacklist if PSKc fails. who is responsible for triggering new scans?
 *
 * Matching of thread network is made using Network name, Xpanid, panid, TODO channel?? or not? gives channel flexibility
 *
 * \param interface interface id of this thread instance.
 *
 */
int thread_management_native_commissioner_connect(int8_t interface_id, commissioning_link_configuration_s *link_ptr, uint8_t *PSKc_ptr);

/**
 * gets the address of native commissioner parent and the commissioning port for that connection
 *
 * \param interface_id Network Interface
 * \param address_ptr address buffer (16 bytes) where to send commission messages
 * \param port return the port for commissioner
 * \param PSKc_ptr return buffer for the PSKc (16 bytes) for this network instance
 *
 * return 0, address OK
 * return <0 fail
 */
int thread_management_native_commissioner_get_connection_info(int8_t interface_id, uint8_t *address_ptr, uint16_t *port, uint8_t *PSKc_ptr);

/**
 * Get Thread device link timeout
 *
 * \param interface_id Network Interface
 * \link_timeout New timeout value in seconds
 *
 * return 0, Set OK
 * return <0 Set Fail
 */
int8_t thread_management_set_link_timeout(int8_t interface_id, uint32_t link_timeout);

/**
 * Set link timeout for Thread device
 *
 * \param interface_id Network Interface
 * \link_timeout[out] pointer where timeout will be writted
 *
 * return 0, Get OK
 * return <0 Get Fail
 */
int8_t thread_management_get_link_timeout(int8_t interface_id, uint32_t *link_timeout);

#ifdef __cplusplus
}
#endif

#endif /* THREAD_MANAGEMENT_IF_H_ */
