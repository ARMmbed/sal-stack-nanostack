/*
 * Copyright (c) 2015 ARM Limited. All rights reserved.
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
#ifndef CCA_API_H_
#define CCA_API_H_

#include "ns_types.h"

#define CCA_IDLE            0
#define CCA_RANDOM_IN_RANGE 1

void cca_start(uint8_t mode, uint16_t min, uint16_t max, int (*cca_check_fptr)(void), void (*cca_done_fptr)(int));
void cca_timer_interrupt(int8_t timer_id, uint16_t slots);

#endif /* CCA_API_H_ */
