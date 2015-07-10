/*
 * Copyright (c) 2014-2015 ARM. All rights reserved.
 */
/**
 * \file net_nvm_api.h
 * \brief Library Pana NVM API for Client and Server.
 *
 * \section server-api Server NVM API
 *
 * Pana Server Security material is crypted always and if you change border router RF module
 * decrypt does not work properly.
 *
 *  - pana_server_nvm_callback_set(),  Init Pana Server NVM functionality
 *  - pana_server_restore_from_nvm(), Load crypted Pana server base and security material from NVM
 *  - pana_server_nvm_client_session_load(), Load crypted Client session from NVM
 *
 * \section client-api Client NVM API
 *
 * nw_nvm.c use already this API and Application can just use net_nvm_api.h.
 *
 * - pana_client_nvm_callback_set(), Init Pana session NVM
 * - net_read_persistent_data(), Read NWK ID & MAC 16-bit Address
 * - net_nvm_data_load(), Load Pana session, NWK-ID and short address to stack for re-use.
 * - net_pana_client_session_nvm_data_load(), Load saved pana session with spesific address or not
 *
 */
#ifndef PANA_NVM_API_H_
#define PANA_NVM_API_H_

#include "ns_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define PANA_CLIENT_NVM_SESSION_BUF_SIZE 70
#define PANA_CLIENT_NVM_SESSION_BUF_SIZE_WITH_ADDRESS 86
#define NET_NVM_SESSION_BUF_SIZE_WITH_NET_PARAMS_AND_PANA 88

#define PANA_SERVER_CLIENT_NVM_SESSION_UNCRYPTED_DATA_SIZE 20
#define PANA_SERVER_CLIENT_NVM_SESSION_FULL_ENCRYPTED_DATA_SIZE 96
#define PANA_SERVER_CLIENT_NVM_SESSION_SEQUENCY_STATE_ENCRYPTED_DATA_SIZE 33
#define PANA_SERVER_CLIENT_NVM_SESSION_BUF_SIZE (PANA_SERVER_CLIENT_NVM_SESSION_UNCRYPTED_DATA_SIZE + PANA_SERVER_CLIENT_NVM_SESSION_ENCRYPTED_DATA_SIZE)


#define PANA_SERVER_MATERIAL_BUF_SIZE 90
/*!
 * \enum pana_nvm_update_process_t
 * \brief Pana Server NVM update states.
 */
typedef enum pana_nvm_update_process_t {
    PANA_SERVER_MATERIAL_UPDATE,                /**< Pana Server Security Material Update */
    PANA_SERVER_CLIENT_SESSION_UPDATE,          /**< Pana Client Session Update */
    PANA_SERVER_CLIENT_SESSION_SEQ_UPDATE,      /**< Pana Client Session Sequence number Update */
    PANA_SERVER_CLIENT_SESSION_REMOVE_UPDATE,   /**< Pana Client Session remove */
} pana_nvm_update_process_t;

/*!
 * \enum pana_client_nvm_update_process_t
 * \brief Pana Client NVM update states.
 */
typedef enum pana_client_nvm_update_process_t {
    PANA_CLIENT_SESSION_UPDATE,     /**< Pana Session information Fully Update  */
    PANA_CLIENT_SESSION_SEQ_UPDATE, /**< Pana key Pull or Push Operation update REQ and RES sequence number */
} pana_client_nvm_update_process_t;

/* NVM API PART */
/**
 * \brief Pana Server NVM functionality init
 *
 * \param passed_fptr function pointer to NVM update process
 * \param nvm_static_buffer pointer to Application allocated static memory, Needed minimal size is 115 bytes
 *
 *
 * Reference Callback function structure which use EEPROM:
 * - nvm_static_buffer is application is allocated static buffer
 *
 * \return 0, Init OK
 * \return -1, Null parameter detect
 *
 */
extern int8_t pana_server_nvm_callback_set(uint16_t (*passed_fptr)(pana_nvm_update_process_t), uint8_t *nvm_static_buffer);
/**
 * \brief Pana Server Base restore form NVM
 *
 * \param nvm_data pointer to Crypted Pana server base data
 *
 * \return 0, Restore OK
 * \return -1, Memory Allocation fail
 *
 */
extern int8_t pana_server_restore_from_nvm(uint8_t *nvm_data, int8_t interface_id);
/**
 * \brief Pana Client session load from NVM API
 *
 * \param nvm_data pointer to Crypted Pana client session
 *
 * \return 0, Restore OK
 * \return -1, Memory Allocation fail
 *
 */
extern int8_t pana_server_nvm_client_session_load(uint8_t *nvm_pointer);

/**
 * \brief Pana Client NVM functionality init
 *
 * \param passed_fptr function pointer to NVM update process
 * \param nvm_static_buffer pointer to Application allocated static memory, Needed minimal size is 88 bytes
 *
 * Reference Callback function structure which use EEPROM:
 * - nvm_static_buffer is application is allocated static buffer
 *
 * \return 0, Init OK
 * \return -1, Null parameter detect
 *
 */
extern int8_t pana_client_nvm_callback_set(void (*passed_fptr)(pana_client_nvm_update_process_t), uint8_t *nvm_static_buffer);
/**
 * \brief Read Network persistent data
 *
 * \param data_buffer pointer to where stack save 18 bytes [NWK-ID 16bytes, Short Address 2bytes]
 *
 *
 * \return 0, Read OK
 * \return -1, Null parameter detect
 * \return -2 Bootstrap not ready yet
 *
 * This function should call when network bootstrap is ready.
 *
 */
extern int8_t net_read_persistent_data(uint8_t *data_buffer, int8_t interface_id);
/**
 * \brief Load ZigBeeIP node persistent data to stack for re-use
 *
 * \param data_buffer pointer to data which should include 88 bytes[NWK-ID 16bytes, Short Address 2bytes, Pana 70 bytes]
 *
 *
 * \return 0, Read OK
 * \return -1, Null parameter detect
 * \return -2, Stack is active
 * \return <-2 Memory allocation fail
 *
 */
extern int8_t net_nvm_data_load(uint8_t *data_buffer, int8_t interface_id);

/**
 * \brief Load Pana Client Session data to stack for re-use
 *
 * \param data_buffer pointer to data which should include encrypted Pana session 70 bytes
 * \param session_address pointer to session specific address, Give address if you want re-use session with specific parent other wise give NULL
 *
 *
 * \return 0, Read OK
 * \return -1, Null parameter detect
 * \return -2, Stack is active
 * \return <-2 Memory allocation fail
 *
 */
extern int8_t net_pana_client_session_nvm_data_load(uint8_t *data_buffer, uint8_t *session_address, int8_t interface_id);
/**
 * \brief Clean node persistent data and All Pana Client sessions from stack
 *
 * Function disable Network ID filter, set EUID-16 to 0xffff and remove Pana client sessions. Function is only for client purpose.
 *
 *
 * \return 0, Clean OK
 * \return -1, Stack is active
 *
 */
extern int8_t net_nvm_data_clean(int8_t interface_id);
#ifdef __cplusplus
}
#endif
#endif /* PANA_NVM_API_H_ */
