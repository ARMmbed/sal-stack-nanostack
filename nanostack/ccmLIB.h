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
#ifndef CCMLIB_H_
#define CCMLIB_H_
/**
 *
 * \file ccmLIB.h
 * \brief CCM Library API.
 *
 *  \section ccm-api CCM Library API:
 *  - ccm_sec_init(), A function to init CCM library.
 *  - ccm_process_run(), A function to run configured CCM process
 *
 *  \section ccm-inctuction CCM process sequence:
 *  1. Init CCM library by , ccm key, ccm_sec_init()
 *      - security level
 *      - 128-bit ccm key
 *      - mode: AES_CCM_ENCRYPT or AES_CCM_DECRYPT
 *      - CCM L parameter: 2 or 3 depends nonce legth (802.15.4 use 2 and TLS security use 3)
 *  2. Define ADATA pointer and length, if returned global structure mic_len field is > 0
 *  3. Set Data pointer and length
 *  4. Do configured CCM process ccm_process_run()
 *  5. Check Return value:
 *      -If 0 Process ok
 *      -< 0 MIC fail or parameter fail
 *
 *  \section ccm-mutex CCM Mutex for Multi Thread System
 *  If you running multi thread system and CCM library will be used for multiple thread Folow next Intsruction:
 *  1. Add compiler flag to library build process CCM_USE_MUTEX
 *  2. Define OS specific Mutex at Application
 *  3. Implement arm_ccm_mutex_lock() arm_ccm_mutex_unlock() function for using gerated and initialized mutex.
 */
#ifdef __cplusplus
extern "C" {
#endif
#define AES_NO_SECURITY         0x00
#define AES_SECURITY_LEVEL_MIC32    0x01
#define AES_SECURITY_LEVEL_MIC64    0x02
#define AES_SECURITY_LEVEL_MIC128   0x03
#define AES_SECURITY_LEVEL_ENC      0x04
#define AES_SECURITY_LEVEL_ENC_MIC32    0x05
#define AES_SECURITY_LEVEL_ENC_MIC64    0x06
#define AES_SECURITY_LEVEL_ENC_MIC128   0x07

#define AES_CCM_ENCRYPT     0x00
#define AES_CCM_DECRYPT     0x01


/**
 * \brief A function for lock CCM mutex if OS is multi thread. If using single thread create empty function
 */
extern void arm_ccm_mutex_lock(void);
/**
 * \brief A function for unlock CCM mutex if OS is multi thread. If using single thread create empty function
 */
extern void arm_ccm_mutex_unlock(void);

/*!
 * \struct ccm_globals_t
 * \brief CCM global structure.
 * Structure is used to configure NONCE, adata and data before call ccm_process_run().
 */
typedef struct {
    uint8_t exp_nonce[15];  /**< CCM NONCE buffer Nonce */
    uint8_t *data_ptr;      /**< Pointer to Data IN */
    uint16_t data_len;      /**< Length of Data IN */
    const uint8_t *adata_ptr;   /**< Pointer to Authentication data */
    uint16_t adata_len;     /**< Length of Authentication data */
    uint8_t mic_len;        /**< ccm_sec_init() will set here length of MIC */
    uint8_t *mic;           /**< Encrypt process will write MIC and decode compare given MIC */
} ccm_globals_t;

/**
 * \brief A function to init CCM library.
 * \param sec_level Used CCM security level (0-7).
 * \param ccm_key pointer to 128-key.
 * \param mode AES_CCM_ENCRYPT or AES_CCM_DECRYPT
 * \param ccm_l could be 2 or 3. 2 when NONCE length is 13 and 3 when length is 12. (NONCE Len= (15-ccm_l))
 *
 * \return Pointer to Global CCM parameter buffer.
 * \return 0 When parameter fail or CCM is Busy.
 */
extern ccm_globals_t *ccm_sec_init(uint8_t sec_level, const uint8_t *ccm_key, uint8_t mode, uint8_t ccm_l);
/**
 * \brief A function to run configured CCM process.
 * When AES_CCM_ENCRYPT mode is selected and MIC is needed library will save MIC direct after  when data ends.
 * \param sec_level Used CCM security level (0-7).
 * \param ccm_key pointer to 128-key.
 * \param mode AES_CCM_ENCRYPT or AES_CCM_DECRYPT
 * \param ccm_l could be 2 or 3. 2 when NONCE length is 13 and 3 when length is 12. (NONCE Len= (15-ccm_l))
 *
 * \return 0 CCM process OK and when AES_CCM_DECRYPT mode was selected also MIC was correct.
 * \return -1 Init have not called or data or adata pointers or lengths are zero.
 * \return -2 Null pointer given to function
 */
extern int8_t ccm_process_run(ccm_globals_t *ccm_params);
#ifdef __cplusplus
}
#endif

#endif /* CCMLIB_H_ */
