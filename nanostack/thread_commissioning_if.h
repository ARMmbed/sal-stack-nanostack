/*
 * Copyright (c) 2015 ARM. All rights reserved.
 */
/*
 *
 * Copyrigth ARM Ltd 2014
 *
 */

#ifndef NANOSTACK_THREAD_COMMISSIONING_IF_H_
#define NANOSTACK_THREAD_COMMISSIONING_IF_H_

#include "ns_types.h"

/**
 * This interface is used in thread leader role when Thread commissioning interface is enabled.
 */

/**
 * Register commissioner. This data is then registered as commissioner adverticements in thread network.
 *
 * \param interface_id Network Interface
 * \param border_router_address Border router locator address
 * \param commissioner_id_ptr commissioner id that is registering
 * \param session_id created session id for this commissioner
 *
 * return 0, OK
 * return -1 fail
 */
int thread_commissioning_register(int8_t interface_id, uint8_t border_router_address[static 16], char *commissioner_id_ptr, uint16_t *session_id);

/**
 * Refresh commissioner keep alive timer
 *
 * \param interface_id Network Interface
 * \param session_id Session id for this commissioner
 * \param state true refresh commissioner, false remove commissioner
 *
 * return 0, OK
 * return -1 failure the commissioner is dropped
 */

int thread_commissioning_session_refresh(int8_t interface_id, uint16_t session_id, bool state);

/**
 * Update steering data. This might also affect permit join bit if that is available
 *
 * \param interface_id Network Interface
 * \param buf_ptr New steering data that is adverticed.
 * \param length length of new steering data
 *
 * return 0, OK
 * return -1 fail
 */
int thread_commissioning_steering_data_set(int8_t interface_id, uint8_t *buf_ptr, uint16_t length);

/**
 * Add pairwise key to allow transfer of network credentials. This can be done by joiner and joiner router.
 * After a timeout the key is deleted.
 *
 * \param interface_id Network Interface
 * \param valid_life_time time in seconds that this key is valid.
 * \param eui64 eui64 of device that key is valid for.
 * \param key fixed key used for this device
 *
 * return 0, OK
 * return -1 fail
 */
int thread_commissioning_pairwise_key_add(int8_t interface_id, uint32_t valid_life_time, uint8_t eui64[static 8], uint8_t key[static 16]);

/**
 * Delete all pairwise keys from the device.
 *
 * \param interface_id Network Interface
 *
 * return 0, OK
 * return -1 fail
 */
int thread_commissioning_pairwise_key_delete_all(int8_t interface_id);

/**
 * register commissioning tasklet id
 *
 * Register tasklet id where commissioning initialisation event is sent.
 *
 * \param tasklet_id Tasklet id
 * \param event_type event type should be APPLICATION_EVENT
 *
 * return 0, OK
 * return <0 fail
 */
int thread_commissioning_register_app_id(int8_t interface_id, int8_t tasklet_id);

/** @todo
 * This function is interop hack only and should not exists when proper commissioner
 * joiner implementation is made. The Link configuration is learned during commissioning and should be
 * updated using new function in thread management interface thread_managenet_configuration_set.
 * That change would also remove the following functions
 * int thread_managenet_security_material_set(int8_t nwk_interface_id, bool enableSecurity, uint8_t *thrMasterKey, uint32_t thrKeySequenceCounter, uint32_t thrKeyRotation);
 * int thread_management_commission_data_set(int8_t nwk_interface_id);
 * functions
 *
 * */
int thread_commissioning_enable_security(int8_t interface_id);
/**
 * get registered border router locator address
 *
 * \param interface_id interface id
 * \param address_ptr address of the border router where the commissioner is registered.
 *
 * return 0, OK
 * return <0 fail
 */
int thread_commissioning_border_router_locator_get(int8_t interface_id, uint8_t *address_ptr);

/**
 * Event Id for tmf commissioning request event
 */

#define TMF_POST_COMMISSION_REQUEST 2
/**
 * Event data for commissioning request event
 *
 */
typedef struct {
    uint8_t address[16];/**< Link Local address of the joiner router*/
    int8_t interfaceId;/**< nterface identifier*/
} tmf_commision_req;

/**
 * Event Id for tmf commissioning request event
 */

#define TMF_POST_COMMISSION_RELAY 3
/**
 * Event data for commissioning request event
 *
 */
typedef struct {
    uint8_t address[16];/**< Link Local address of the joiner router*/
    bool enableRelay;
    int8_t interfaceId;/**< nterface identifier*/
} tmf_relay_req;

#endif /* NANOSTACK_THREAD_COMMISSIONING_IF_H_ */
