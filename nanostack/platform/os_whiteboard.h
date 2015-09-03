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
#ifndef OS_WHITEBOARD_H
#define OS_WHITEBOARD_H

#include "ns_types.h"

/*
 * API to modify Operating System routing table
 * and neighbor cache.
 * Required on Linux. Not needed on embedded systems.
 */

#ifndef __linux__
/* Use DUMMY functions on not Linux platforms */
#define whiteboard_os_modify(x,y) ((void) 0)

#else /* LINUX */

enum add_or_remove {ADD = 1, REMOVE = 0};

/** Modify operating system whiteboard.
 * Add or remove entries to OS routing table
 * \param IPv6 address
 * \add to add or remove address.
 */
void whiteboard_os_modify(const uint8_t address[static 16], enum add_or_remove mode);

#endif


#endif /* OS_WHITEBOARD_H */
