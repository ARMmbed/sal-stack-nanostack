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

#ifndef NET_SLEEP_H_
#define NET_SLEEP_H_

#include "ns_types.h"

/**
 * \brief Check network stack enter deep sleep possibility and max sleep time.
 *
 * \return Time in milliseconds for sleep 0 means not possibility to enter deep sleep.
 *
 */
uint32_t arm_net_check_enter_deep_sleep_possibility(void);
/**
 * \brief Set Stack to sleep.
 *
 * \return 0 Stack stopped.
 * \return -1 Not supported action at moment
 *
 */
int arm_net_enter_sleep(void);
/**
 * \brief Restart stack after sleep.
 *
 *  Stack enable and synch timers after sleep
 *
 *  \param sleeped_time_in_ms time sleeped time in milli seconds what stack need to synch
 *
 * \return 0 Stack Restarted.
 * \return 1 Stack can continue sleep for sleeped_time_in_ms define time
 * \return -1 Stack already active
 *
 */
int arm_net_wakeup_and_timer_synch(uint32_t sleeped_time_in_ms);


#endif /* NET_SLEEP_H_ */
