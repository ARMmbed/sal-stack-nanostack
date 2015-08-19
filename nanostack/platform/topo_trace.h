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
#ifndef TOPO_TRACE_H
#define TOPO_TRACE_H

#include "ns_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * API to trace network topology
 */

#ifndef HAVE_TOPO_TRACE
/* Use DUMMY functions if not required */
#define topo_trace(type, address, update_type) ((void) 0)

#else /* HAVE_TOPO_TRACE */

enum topo_type {
    TOPOLOGY_RPL,
    TOPOLOGY_MLE,
    TOPOLOGY_THREAD,
};

enum topo_update {
    TOPO_CLEAR = 2, // Clear all links of specified type to any address
    TOPO_ADD = 1,   // Add link of specified type to specified address
    TOPO_REMOVE = 0 // Remove link of specified type to specified address
};

/** Modify topology tables.
 *
 * Add or remove entries to external topology tracer.
 *
 * \param topo_type Topology link type
 * \param address EUI-64 address
 * \param update_type add to add or remove address.
 */
void topo_trace(enum topo_type type, const uint8_t address[static 8], enum topo_update update_type);

#endif

#ifdef __cplusplus
}
#endif

#endif /* TOPO_TRACE_H */
