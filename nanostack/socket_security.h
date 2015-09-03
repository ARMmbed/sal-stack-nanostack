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
