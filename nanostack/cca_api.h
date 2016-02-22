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
/**
 * \file cca_api.h
 * \brief CCA API
 *
 * - cca_start(), Start CCA timeout.
 *   Usage:
 *   cca_start(CCA_RANDOM_IN_RANGE, 15, 30, rf_check_cca, rf_tx_start);
 *   Starts timeout in (random) range 750us - 1500us, and calls rf_check_cca() to check CCA status and rf_tx_start() to start (or abandon) transmission.
 */
#ifndef CCA_API_H_
#define CCA_API_H_

/* CCA modes */
#define CCA_RANDOM_IN_RANGE 1
#define CCA_FHSS            2

typedef enum
{
    CHANNEL_IDLE,
    CHANNEL_NOT_IDLE,
} channel_status_e;

/**
 * \brief API to start new CCA timeout.
 *
 * \param device_id Device driver ID.
 * \param mode CCA timeout mode, only CCA_RANDOM_IN_RANGE supported.
 * \param min Minimum time for random timeout (50us slots).
 * \param max Maximum time for random timeout (50us slots).
 * \param cca_check_fptr Pointer to CCA status check function (return values: CHANNEL_IDLE, CHANNEL_NOT_IDLE).
 * \param cca_done_fptr Pointer to CCA done function (parameter values: CHANNEL_IDLE, CHANNEL_NOT_IDLE).
 *
 * \return 0, Success
 * \return -1, Failure
 *
 */
int cca_start(int8_t device_id, uint8_t mode, uint16_t min, uint16_t max, channel_status_e (*cca_check_fptr)(void), void (*cca_done_fptr)(channel_status_e));

#endif /* CCA_API_H_ */
