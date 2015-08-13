/*
 * Copyright (c) 2014-2015 ARM Limited. All rights reserved.
 */
#ifndef NWK_STATS_API_H
#define NWK_STATS_API_H
/**
 * \file nwk_stats_api.h
 * \brief 6LowPAN library Network stats API
 *
 * - protocol_stats_start(), Enable stats Update
 * - protocol_stats_stop(), Stop Stack stats Update
 * - protocol_stats_reset(), Reset all Stats information to null.
 *
 *
 */

#include "ns_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * /struct nwk_stats_t
 * /brief Struct for Network stats Buffer structure
 */
typedef struct nwk_stats_t {
    /*mac stats*/
    uint16_t mac_tx_buffer_overflow; /**< Mac TX queue oferflow count. */
    uint32_t mac_rx_count;          /**< Mac RX packet count. */
    uint32_t mac_tx_count;          /**< Mac TX packet count. */
    uint32_t mac_rx_drop;           /**< Mac RX packet drop count. */
    /* Mac Payload Flow */
    uint32_t mac_tx_bytes;          /**< Mac TX bytes count. */
    uint32_t mac_rx_bytes;          /**< Mac RX bytes count. */
    uint32_t mac_tx_failed;         /**< Mac TX failed count. */
    uint32_t mac_tx_retry;          /**< Mac TX retry count. */
    uint32_t mac_tx_cca_cnt;        /**< Mac TX CCA count. */
    uint32_t mac_tx_failed_cca;     /**< Mac Failed CCA count. */
    uint32_t mac_security_drop;     /**< Mac Security Packet drops count. */
    /* 6Lowpan */
    uint32_t ip_rx_count;           /**< IP RX Packet count. */
    uint32_t ip_tx_count;           /**< IP TX Packet count. */
    uint32_t ip_rx_drop;            /**< IP RX Packet drops count. */
    uint32_t ip_cksum_error;        /**< IP Checksum error count. */
    /* IP Payload Flow */
    uint32_t ip_tx_bytes;           /**< IP TX bytes count. */
    uint32_t ip_rx_bytes;           /**< IP RX bytes count. */
    uint32_t ip_routed_up;          /**< IP Routed UP bytes count. */
    /* Fragments */
    uint32_t frag_rx_errors;        /**< Fragmentation RX error count. */
    uint32_t frag_tx_errors;        /**< Fragmentation TX error count. */
    /*RPL stats*/
    uint16_t rpl_route_routecost_better_change; /**< RPL Parent Change count. */
    uint16_t ip_routeloop_detect;               /**< RPL Route Loop detection count. */
    uint16_t ip_no_route;                       /**< RPL No Route count. */
    /* Buffers */
    uint32_t buf_alloc;             /**< Buffer allocation count. */
    uint32_t buf_headroom_realloc;  /**< Buffer headroom realloc count. */
    uint32_t buf_headroom_shuffle;  /**< Buffer headroom shuffle count. */
    uint32_t buf_headroom_fail;     /**< Buffer headroom failure count. */
} nwk_stats_t;

/**
  *  \brief Enable stats collection.
  *
  * \param stats_ptr static pointer where stack update stack
  *
  */
extern void protocol_stats_start(nwk_stats_t *stats_ptr);
/**
  *  \brief Disable stats collection.
  *
  */
extern void protocol_stats_stop(void);
/**
  *  \brief Reset stats info fiels.
  *
  */
extern void protocol_stats_reset(void);
#ifdef __cplusplus
}
#endif
#endif
