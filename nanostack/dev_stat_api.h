/*
 * Copyright (c) 2014-2015 ARM Limited. All rights reserved.
 */
/**
 * \file dev_stat_api.h
 * \brief 6LowPAN library Dev stat API
 *
 * - dev_stat_internal_init(), Enable stats Update.
 * - dev_stat_get_longest_heap_sector(), Get Longest free sector.
 * - dev_stat_get_runtime_seconds(), Get Sytem on time in seconds.
 *
 */
#ifndef _DEV_STAT_API_H
#define _DEV_STAT_API_H

#include "ns_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * /struct dev_stat_t
 * /brief Struct for Device stats Buffer structure
 */
typedef struct dev_stat_t {
    /*Heap stats*/
    int16_t heap_sector_size;               /**< Heap total Sector len. */
    int16_t heap_sector_alloc_cnt;      /**< Reserved Heap sector cnt. */
    int16_t heap_sector_allocated_bytes;    /**< Reserved Heap data in bytes. */
    int16_t heap_sector_allocated_bytes_max;    /**< Reserved Heap data in bytes max value. */
    uint32_t heap_alloc_total_bytes;        /**< Total Heap allocated bytes. */
    uint32_t heap_alloc_fail_cnt;           /**< Counter for Heap allocation fail. */
} dev_stat_t;

/**
  *  \brief Enable device stats collection.
  *
  * \param info_ptr static pointer where stack update device stat
  *
  * \return 0, Init OK
  * \return -1, Null pointer parameter detected
  *
  */
extern int8_t dev_stat_internal_init(dev_stat_t *info_ptr);
/**
  *  \brief Get current longest free sector.
  *
  *
  * \return signed 16-bit length for longest sector size
  *
  */
extern int16_t dev_stat_get_longest_heap_sector(void);
/**
  *  \brief Get Sytem runtime in seconds.
  *
  *
  * \return signed 16-bit length for longest sector size
  *
  */
extern uint32_t dev_stat_get_runtime_seconds(void);
#ifdef __cplusplus
}
#endif
#endif
