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

/**
 * \file net_polling_api.h
 * \brief ZigBeeIP Sleepy Host Data Poll API.
 *
 *  - arm_nwk_host_mode_set(), Set Sleepy Host new State
 *  - arm_nwk_host_mode_get(), Read Current Host State
 *  - net_host_enter_sleep_state_set(), Enable deep sleep state.
 *
 * Sleepy Host default state is NET_HOST_FAST_POLL_MODE after bootstrap.
 * Stack disable radio always automatically between data polls.
 * Stack could also enable deep sleep when application net_host_enter_sleep_state_set().
 *
 * Sleepy Host could change host state to normal host which saves time and resources when Client is waiting for large amount data.
 *  - nwk_host_mode_set(NET_HOST_RX_ON_IDLE,0) function call trig MLE Handshake update automatically.
 * From NET_HOST_RX_ON_IDLE state to back to cause new MLE Handshake with Parent:
 *  - nwk_host_mode_set(NET_HOST_FAST_POLL_MODE,0) , enter fast mode
 *  - nwk_host_mode_set(NET_HOST_FAST_POLL_MODE,10) , enter slow poll mode by 10 seconds max data poll period.
 *
 * Stack will tell max sleepy time selecting min values from next cases:
 *  - Application allocates system timer trig
 *  - ND protocol next state trig
 *  - MLE Handshake trig
 *  - Pana Key Pull trig
 *  - Next Data Poll period
 *
 */

#ifndef NET_POLLING_H_
#define NET_POLLING_H_

#include "ns_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \enum net_host_mode_t
 * \brief Sleepy Host states.
 */
typedef enum net_host_mode_t {
    NET_HOST_FAST_POLL_MODE,    /**< Sleepy Host Fast Poll State, max Poll period 400ms */
    NET_HOST_SLOW_POLL_MODE,    /**< Sleepy Host Slow Poll State, User define max period in seconds */
    NET_HOST_RX_ON_IDLE,        /**< Sleepy Host --> HOST, Parent could use now Direct Data Send and Host not poll*/
    NET_HOST_MODE_NOT_ACTIVE,   /**< Host mode is not active */
} net_host_mode_t;

/**
  * \brief Set new Host state.
  *
  * \param mode new host state
  * \param poll_time poll time in seconds only handled when NET_HOST_SLOW_POLL_MODE is enabled
  *
  * Valid poll time for NET_HOST_SLOW_POLL_MODE is 0 < poll_time poll_time < 864001 (1 Day)
  *
  * \return 0, State update OK
  * \return -1, unknown state
  * \return -2, invalid time
  * \return -3 MLE handshake trig Fail
  *
  */
extern int8_t arm_nwk_host_mode_set(int8_t nwk_interface_id, net_host_mode_t mode, uint32_t poll_time);
/**
  * \brief Read Current Host State.
  *
  * \param mode pointer where host state will be saved

  * \return 0, State Read update OK
  * \return -1, Net Role is Router or stack is idle
  *
  */
extern int8_t arm_nwk_host_mode_get(int8_t nwk_interface_id, net_host_mode_t *mode);



/**
  * \brief Host Sleep state control.
  *
  * \param state >0 Enables power saving between Data Polling, 0therwise only radio will be only disabled
  *
  * When application wants to save more power it can call net_host_enter_sleep_state_set(1). After call stack idle callback event is EV_READY_TO_SLEEP indicate sleep time in milliseconds.
  * When application wants to disable sleep it just calls net_host_enter_sleep_state_set(0).
  */
extern void arm_net_host_enter_sleep_state_set(int8_t nwk_interface_id, uint8_t state);
#ifdef __cplusplus
}
#endif

#endif /* NET_POLLING_H_ */
