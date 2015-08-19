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
