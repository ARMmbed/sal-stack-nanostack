/*
 * Copyright (c) 2014-2015 ARM Limited. All rights reserved.
 */
#ifndef _NS_LIB_ADDRESS_H
#define _NS_LIB_ADDRESS_H
/**
 * \file ns_address.h
 * \brief 6LowPAN library Address format description
 *
 */
#include "ns_types.h"

/**
 * /enum address_type_t
 * /brief Address types
 */
typedef enum address_type_t {
    ADDRESS_IPV6,                 /**< IPv6 Address type. Must be used always at socket interface. */
    ADDRESS_IPV4,                 /**< IPv4 Address type. Must be used always at socket interface. */
    ADDRESS_TUN_DRIVER_ID         /**< Local socket Address type address only have 1 byte length which is driver ID for packet Source. */
} address_type_t;


/**
 * /struct ns_address_t
 * /brief Struct for the addresses used by socket interface functions and net_address_get()
 */
typedef struct ns_address_t {
    address_type_t type;          /**< Address type. See above. */
    uint8_t address[16];          /**< Address */
    uint16_t identifier;          /**< TCP/UDP Port number */
} ns_address_t;

#endif /* _NS_ADDRESS_H */
