/*
 * Copyright (c) 2014-2015 ARM. All rights reserved.
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
    uint8_t mesh_local_ula_prefix[8]; /**< Mesh local ula prefix*/
    uint8_t extented_pan_id[8]; /**< Extented pan id*/
    uint8_t steering_data[16]; /**< Dynamic steering data set by commissioning device max length 16*/
    uint8_t steering_data_len; /**< Length of current steering data*/
    char *PSKc_ptr; /**< Commissioning credentials*/
    uint8_t PSKc_len;
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
    uint8_t eui64[8];/**< eui64 of the device. This field is used to identify device when joining to network*/
    char *PSKd_ptr;/**< Device credentials used to authenticate device to commissioner*/
    bool leaderCap; /**< Define Leader Capability */
} device_configuration_s;

typedef struct {
    uint8_t protocol_id;
    uint8_t version;
} node_version_t;

typedef struct {
    uint8_t network_id[16];
    uint8_t extented_pan_id[8];
    uint16_t supported_amount;
    uint16_t pan_id;
    bool extented_filter: 1;
    bool network_filter: 1;
    bool pan_id_filter: 1;
    node_version_t supported[];//Additional protocol id and version combinations that clients want to allow joining
    //allocate more size for the amount of versions sizeof(thread_version_t)
} node_link_filter_t;

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


/*Todo update and rethink not enough parameters*/
int thread_management_commission_data_set(int8_t nwk_interface_id);

int thread_managenet_joiner_attach_start(int8_t nwk_interface_id);

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
    node_link_filter_t *link_filter_ptr,
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

#ifdef __cplusplus
}
#endif

#endif /* THREAD_MANAGEMENT_IF_H_ */
