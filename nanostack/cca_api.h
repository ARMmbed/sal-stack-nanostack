/*
 * Copyright (c) 2015 ARM. All rights reserved.
 */
#ifndef CCA_API_H_
#define CCA_API_H_

#include "ns_types.h"

#define CCA_IDLE            0
#define CCA_RANDOM_IN_RANGE 1

void cca_start(uint8_t mode, uint16_t min, uint16_t max, int (*cca_check_fptr)(void), void (*cca_done_fptr)(int));
void cca_timer_interrupt(int8_t timer_id, uint16_t slots);

#endif /* CCA_API_H_ */
