/*
 * Copyright (c) 2014-2015 ARM. All rights reserved.
 */

#ifndef SOCKET_SECURITY_H_
#define SOCKET_SECURITY_H_

#include "ns_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/** TLS-PSK Cipher Suite */
#define SEC_SOCKET_CIPHERSUITE_PSK 1
/** TLS-ECC Cipher Suite */
#define SEC_SOCKET_CIPHERSUITE_ECC 2


/**
 * \brief SET socket TLS chipher Suite support
 *
 * This function set socket TLS chiphersuite list. Library support PSK & ECC. Default is PSK
 *
 * \param socket socket id
 * \param security_suites Chipher suite list (SEC_SOCKET_CIPHERSUITE_PSK , SEC_SOCKET_CIPHERSUITE_ECC)
 *
 * \return 0 done
 * \return -1 invalid socket id
 * \return -2 Invalid Security Suite
 */
extern int8_t sec_socket_set_cipher_suite_list(int8_t socket_id, uint8_t security_suites);
#ifdef __cplusplus
}
#endif
#endif /* SOCKET_SECURITY_H_ */
