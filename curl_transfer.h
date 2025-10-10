/**
 * File              : curl_transfer.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 21.09.2024
 * Last Modified Date: 22.09.2024
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */
/**
 * curl_transfer.h
		
 * Copyright (c) 2024 Igor V. Sementsov <ig.kuzm@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
		
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
		
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
		
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CURL_TRANSFER_H
#define CURL_TRANSFER_H

#ifdef __cplusplus
extern "C" {
#endif
		
/* CURL functions to download/upload files */

#include <stdio.h>

#define CURL_TRANSFER_VERIFY_SSL 1L

/* function to handle download/upload progress 
 * %ptr - pointer to data for progress function
 * %dltotal - total downloaded size
 * %dlnow - current downloaded size
 * %ultotal - total uploaded size
 * %ulnow - current uploaded size */
typedef int 
curl_transfer_progress(void *ptr, 
		double dltotal, double dlnow, 
		double ultotal, double ulnow); 

/* download url to FILE stream
 * %fp - pointer to FILE stream
 * %url - null-terminated string with url address
 * %error - pointer to allocated error string or NULL;
 * %ptr - pointer to data for progress function or NULL
 * %progress - pointer to progress function or NULL */
size_t curl_transfer_download_file(
		FILE *fp, const char *url, char **error, 
		void *ptr, curl_transfer_progress *progress);

/* download url to memory
 * %data - pointer to allocated memory with downloaded data
 * %url - null-terminated string with url address
 * %error - pointer to allocated error string or NULL;
 * %ptr - pointer to data for progress function or NULL
 * %progress - pointer to progress function or NULL */
size_t curl_transfer_download_data(
		void **data, const char *url, char **error, 
		void *ptr, curl_transfer_progress *progress);

/* upload FILE stream to url
 * %fp - pointer to FILE stream
 * %url - null-terminated string with url address
 * %error - pointer to allocated error string or NULL;
 * %ptr - pointer to data for progress function or NULL
 * %progress - pointer to progress function or NULL */
size_t curl_transfer_upload_file(
		FILE *fp, const char *url, char **error, 
		void *ptr, curl_transfer_progress *progress);

/* upload data to memory
 * %data - data to upload
 * %size - data size
 * %url - null-terminated string with url address
 * %error - pointer to allocated error string or NULL;
 * %ptr - pointer to data for progress function or NULL
 * %progress - pointer to progress function or NULL */
size_t curl_transfer_upload_data(
		void *data, size_t size, const char *url, char **error, 
		void *ptr, curl_transfer_progress *progress);

#ifdef __cplusplus
}
#endif

#endif // CURL_TRANSFER_H
