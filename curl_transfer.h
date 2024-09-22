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
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

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
static size_t curl_transfer_download_file(
		FILE *fp, const char *url, char **error, 
		void *ptr, curl_transfer_progress *progress);

/* download url to memory
 * %data - pointer to allocated memory with downloaded data
 * %url - null-terminated string with url address
 * %error - pointer to allocated error string or NULL;
 * %ptr - pointer to data for progress function or NULL
 * %progress - pointer to progress function or NULL */
static size_t curl_transfer_download_data(
		void **data, const char *url, char **error, 
		void *ptr, curl_transfer_progress *progress);

/* upload FILE stream to url
 * %fp - pointer to FILE stream
 * %url - null-terminated string with url address
 * %error - pointer to allocated error string or NULL;
 * %ptr - pointer to data for progress function or NULL
 * %progress - pointer to progress function or NULL */
static size_t curl_transfer_upload_file(
		FILE *fp, const char *url, char **error, 
		void *ptr, curl_transfer_progress *progress);

/* upload data to memory
 * %data - data to upload
 * %size - data size
 * %url - null-terminated string with url address
 * %error - pointer to allocated error string or NULL;
 * %ptr - pointer to data for progress function or NULL
 * %progress - pointer to progress function or NULL */
static size_t curl_transfer_upload_data(
		void *data, size_t size, const char *url, char **error, 
		void *ptr, curl_transfer_progress *progress);

/*
 * IMPLIMATION
 */

struct _curl_transfer_data {
	char *data;
	int size;
};

static size_t _curl_transfer_data_writefunc(
		unsigned char  *data, size_t s, size_t n, 
		struct _curl_transfer_data *d)
{
	if (!d)
		return 0;

	size_t size = s * n;
  char *ptr = 
		realloc(d->data, d->size + size + 1);
  if(!ptr)
    return 0;  /* out of memory! */

  d->data = (char *)ptr;
  memcpy(&(d->data[d->size]), data, size);
  d->size += size;
  d->data[d->size] = 0;

  return size;
}

static size_t _curl_transfer_upload_data_readfunc(
		unsigned char *data, size_t s, size_t n, 
		struct _curl_transfer_data *d)
{
	size_t size = s * n;
	
	if (size > d->size)
		size = d->size;

	memcpy(data, d->data, size);
	
	d->data += size;
	d->size -= size;

	return s;
}

static size_t _curl_transfer_upload_file_readfunc(
		char *data, size_t s, size_t n, 
		FILE *fp)
{
	return fread(data, s, n, fp);
}

static CURL * _curl_transfer_init(
		char fDownload,
		FILE *fp, struct _curl_transfer_data *d, size_t size, 
		const char * url, char **error, 
		void *ptr, 
		curl_transfer_progress *progress)
{
	CURL *curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url);
		if (fp && fDownload)
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);		
		if (d && fDownload) {
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, d);		
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, 
					_curl_transfer_data_writefunc);
		}
		
		/* enable verbose for easier tracing */
		/*curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);*/

		if (fDownload)
			curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);		
		else
			curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
		

		if (fp && !fDownload){
			curl_easy_setopt(curl, CURLOPT_READDATA, fp);
			curl_easy_setopt(curl, CURLOPT_READFUNCTION, 
					_curl_transfer_upload_file_readfunc);
		}

		if (d && !fDownload){
			curl_easy_setopt(curl, CURLOPT_READDATA, d);
			curl_easy_setopt(curl, CURLOPT_READFUNCTION, 
					_curl_transfer_upload_data_readfunc);
			curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE,
				 	(curl_off_t)size);
		}
		
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 
				CURL_TRANSFER_VERIFY_SSL);		
		
		if (progress) {
#if LIBCURL_VERSION_NUM < 0x072000
			curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, ptr);
			curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, 
					progress);
#else
			curl_easy_setopt(curl, CURLOPT_XFERINFODATA, ptr);
			curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, 
					progress);
#endif
			curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0);
		}
	}
	return curl;
}

static size_t _curl_transfer_perfom(
		char fDownload, CURL *curl, char **error)
{
	CURLcode res = curl_easy_perform(curl);
	if (res != CURLE_OK) {
		if (error)
			*error = strdup(curl_easy_strerror(res));
		curl_easy_cleanup(curl);
		return 0;
	}
	/* now extract transfer info */
	curl_off_t size;
	if (fDownload)
		curl_easy_getinfo(curl, 
				CURLINFO_CONTENT_LENGTH_DOWNLOAD_T, &size);
	else 
		curl_easy_getinfo(curl, 
				CURLINFO_CONTENT_LENGTH_UPLOAD_T, &size);
	/* always cleanup */
	curl_easy_cleanup(curl);
	return size;
}
		
size_t curl_transfer_download_file(
		FILE *fp, const char *url, char **error, 
		void *ptr, 
		curl_transfer_progress *progress)
{
	if (!fp){
		if (error)
			*error = strdup("\%fp argument is NULL");
		return 0;
	}
	CURL *curl = 
		_curl_transfer_init(
				1, fp, NULL, 0, 
				url, error, ptr, progress);
	if (curl)	
		return _curl_transfer_perfom(1, curl, error);	
	
	return 0;
}

size_t curl_transfer_download_data(
		void **data, const char *url, char **error, 
		void *ptr, 
		curl_transfer_progress *progress)
{
	if (!data){
		if (error)
			*error = strdup("\%data argument is NULL");
		return 0;
	}
	struct _curl_transfer_data d = {NULL, 0};
	d.data = malloc(1);
	if (!d.data){
		if (error)
			*error = strdup("can't allocate memory");
		return 0;
	}
	CURL *curl = 
		_curl_transfer_init(
				1, NULL, &d, 0, 
				url, error, ptr, progress);
	if (curl){
		if (_curl_transfer_perfom(1, curl, error)){
			*data = d.data;
			return d.size;	
		}
	}	
	
	// free data on error
	free(d.data);
	return 0;
}

size_t curl_transfer_upload_file(
		FILE *fp, const char *url, char **error, 
		void *ptr, 
		curl_transfer_progress *progress)
{
	if (!fp){
		if (error)
			*error = strdup("\%fp argument is NULL");
		return 0;
	}
	CURL *curl = 
		_curl_transfer_init(
				0, fp, NULL, 0, 
				url, error, ptr, progress);
	if (curl)	
		return _curl_transfer_perfom(0, curl, error);	
	
	return 0;
}

size_t curl_transfer_upload_data(
		void *data, size_t size, const char *url, char **error, 
		void *ptr, 
		curl_transfer_progress *progress)
{
	if (!data){
		if (error)
			*error = strdup("\%data argument is NULL");
		return 0;
	}
	struct _curl_transfer_data d = {data, size};
	CURL *curl = 
		_curl_transfer_init(
				0, NULL, &d, size, 
				url, error, ptr, progress);
	if (curl)	
		return _curl_transfer_perfom(0, curl, error);	
	
	return 0;
}
#ifdef __cplusplus
}
#endif

#endif // CURL_TRANSFER_H
