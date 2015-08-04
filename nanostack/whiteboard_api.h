/*
 * Copyright (c) 2014-2015 ARM Limited. All rights reserved.
 * Permissive Binary License
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

#ifndef WHITEBOARD_API_H_
#define WHITEBOARD_API_H_

#include "net_interface.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct whiteboard_entry_t {
    uint8_t                         address[16];        /*!< registered IP address */
    uint_least24_t                  ttl;                /*!< whiteboard entry TTL (seconds) */
    uint8_t                         eui64[8];           /*!< EUI-64 of node that IP address is registered to */
    nwk_interface_id                interface_index;
#ifdef NVM_BORDER_ROUTER
    uint16_t                        nvm_offset;
#endif
} whiteboard_entry_t;

/** \brief Read whiteboard entries
 * \param cur NULL to read first entry, or get next entry after cur
 */
extern whiteboard_entry_t *whiteboard_get(whiteboard_entry_t *cur);
#ifdef __cplusplus
}
#endif
#endif /* WHITEBOARD_API_H_ */
