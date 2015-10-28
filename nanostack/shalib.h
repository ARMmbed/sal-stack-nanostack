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
/**
 * \file shalib.h
 * \brief SHA256 Library API.
 *
 *  \section sha256-api SHA256 Library API:
 *   There is 2 way to calculate SHA256
 *   1.Calc by given 1 data and length pointer
 *      - SHALIB_SHA256_HASH(), A function to calculate SHA256 for given data.
 *   2. Calc by given separately different data pointer sequence
 *      -SHALIB_init_sha256(), Init SHA registers
 *      -SHALIB_push_data_sha256(), Give data sectors(s) one by one
 *      - **** Give data
 *      -SHALIB_push_data_sha256(), Give last data sequence
 *      -SHALIB_finish_sha256(), finnish SHA256 by given data to given buffer
 *
 *   \section sha256res-api SHA256 Register Resume & Save Library API:
 *   SHA256 Operation dataflow cuold came in many different timeslot or packet and between those application need
 *   to calculated more SHA256 then SAVE and Resume operation SHA registers is usefully.
 *   -sha_resume_regs(), Load old HASH sessions SHA registers
 *   -sha_save_regs(), Save current HASH sessions SHA registers
 *
 *  \section hmac256-inctuction HMAC256 process sequency:
 *  1. SHALIB_init_HMAC() , Init HMAC IN process by given Security signature material
 *  2. SHALIB_push_data_sha256(), Give data sectors(s) one by one
 *  3. SHALIB_finish_HMAC(), FInish HMAC and save SHA256 hash to given buffer
 *
 *  \section prf256-inctuction PRF256 process sequency:
 *  1. shalib_prf_param_get() , Init PRF and get configure structure
 *  2. Set to configure structure next parameters:
 *      -HMAC security signity pointer and length
 *      -Label text and length
 *      -Seed data and length
 *      -PRF result pointer
 *  3. shalib_prf_calc(), Calc PRF256 HASH
 *
 */

#ifndef SHALIB_H_
#define SHALIB_H_
#ifdef __cplusplus
extern "C" {
#endif

//Do Not change
#define SHALIB_RING_BUFFER_SIZE 64
/*!
 * \struct prf_sec_param_t
 * \brief PRF 256 stucture
 * Structure is used to configure PRF calc operation:secret, label, seed and buffer before call shalib_prf_calc().
 */
typedef struct {
    uint8_t *secret;        /**< HMAC security signity pointer */
    uint8_t sec_len;        /**< HMAC security signity length */
    uint8_t label[25]; //25 /**< Label text */
    uint8_t lablen;         /**< Label text length*/
    uint8_t *seed;          /**< Seed data */
    uint8_t seedlen;        /**< Seed data length */
    uint8_t *buffer;        /**< Buffer to where 256-BIT hash will be saved*/
} prf_sec_param_t;

/*!
 * \struct sha256_temp_t
 * \brief SHA temporary buffer database for save current hash operation or resume saved
 */
typedef struct {
    uint8_t  m_Data[SHALIB_RING_BUFFER_SIZE];   /**< 64-bytes Ring Buffer for SHA256 Operation */
    uint8_t  m_Read;                            /**< Read pointer to ring buffer */
    uint8_t  m_Write;                           /**< Write pointer to ring buffer */
    uint8_t  m_ReadCount;
    uint16_t SHALIB_pushed_bytes;           /**< Hash Total byte count*/
    uint8_t SHALIB_buffered_bytes;          /**< Ring buffer Data in */
    uint32_t areg_temps[8];                 /**< Shalib operation 8 HREG */
} sha256_temp_t;

// Cumulative static version using a static ring buffer object
//=============================================================================
/**
 * \brief Init SHA registers.
 */
void SHALIB_init_sha256(void);                                  // Call this first...
/**
 * \brief Push data SHALIB.
 *
 * \param data pointer to data
 * \param len length of data
 */
void SHALIB_push_data_sha256(uint8_t *data, uint16_t len);      // ... add data ...
/**
 * \brief Finish SHA56 operation and get result to given buffer by given length.
 *
 * \param buffer pointer to result buffer
 * \param len count of 32-bit register to save to buffer (8= 256 bit and 4= 128-bit)
 */
void SHALIB_finish_sha256(uint8_t *buffer, uint8_t len);        // ... get the sha256 digest.
/**
 * \brief Calc SHA256 by 1 function call.
 *
 * \param data_ptr pointer to data
 * \param data_len length of data
 * \param buffer pointer to 256-bit buffer!!
 */
void SHALIB_SHA256_HASH(uint8_t *data_ptr, uint16_t data_len, uint8_t *buffer); // ... get the sha256 digest.

/* Shalib Registers Resume and Save API */
/**
 * \brief Resume old SHA56 Registers.
 *
 * \param ptr pointer to saved session
 */
void sha_resume_regs(sha256_temp_t *ptr);
/**
 * \brief Save SHA56 Registers.
 *
 * \param ptr pointer to buffer
 */
void sha_save_regs(sha256_temp_t *ptr);

// Use these for cumulativec HMAC
/**
 * \brief Init HMAC256 operation by given security material.
 *
 * \param secret pointer to security material
 * \param sec_len length of security material
 */
void SHALIB_init_HMAC(uint8_t *secret, uint8_t sec_len);        // Call this first...
// ... add data ... by SHALIB_push_data_sha256()
/**
 * \brief Finnish HMAC256 operation and save result given buffer.
 *
 * \param buffer pointer to result buffer
 * \param len count of 32-bit register to save to buffer (8= 256 bit and 4= 128-bit)
 */
void SHALIB_finish_HMAC(uint8_t *buffer, uint8_t len); // ... get the HMAC digest.


/** PRF API */
/**
 * \brief Init PRF library and sha Registers.
 * Function return configure structure where user need to set next items:
 *  -Security material and length
 *  -Label text and length
 *  -Seed data and length
 *  -Buffer for 256 Result
 *
 * \return pointer to PRF Configure structure

 */
prf_sec_param_t *shalib_prf_param_get(void);  // GET PRF structure
/* SET Secret, Label, Seed & buffer to 256 PRF  */
/**
 * \brief Finnish PRF256 operation and save result given buffer.
 *
 */
void shalib_prf_calc(void);// GET 256 PRF
#ifdef __cplusplus
}
#endif
#endif /* SHALIB_H_ */
