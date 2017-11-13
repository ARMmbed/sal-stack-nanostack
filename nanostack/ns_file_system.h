/*
 * Copyright (c) 2017 ARM Limited. All rights reserved.
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

#ifndef _NS_FILE_SYSTEM_H_
#define _NS_FILE_SYSTEM_H_

/**
 * \file ns_file_system.h
 * \brief Nanostack file system API.
 */

/**
 * \brief Set file system root path.
 *
 *  Allow stack to store information to the location provided.
 *  Setting root path to NULL will prevent file system usage.
 *
 * \param root_path Path to location where networking files can be stored. Path must exists in the file system
 *   and path must end to "/" character. Path can be NULL to disable file system usage.
 *
 * \return 0 in success, negative value in case of error.
 *
 */
int ns_file_system_set_root_path(const char *root_path);

/**
 * \brief Get file system root path.
 *
 * \return Root path to stack storage location.
 *
 */
char *ns_file_system_get_root_path(void);

#endif /* _NS_FILE_SYSTEM_H_ */
