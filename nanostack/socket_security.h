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
