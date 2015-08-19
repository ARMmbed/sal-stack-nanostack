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
