/*
 * Copyright (c) 2014-2015 ARM Limited. All rights reserved.
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
    int8_t                          interface_index;
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
