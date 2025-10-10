/**
 * File              : curl_transfer.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 21.09.2024
 * Last Modified Date: 22.09.2024
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */
#include "curl_transfer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

struct _curl_transfer_data {
	char *data;
	size_t size;
};

static size_t _curl_transfer_data_writefunc(
		unsigned char  *data, size_t s, size_t n, 
		struct _curl_transfer_data *d)
{
	size_t size;
	char *ptr;

	if (!d)
		return 0;

	size = s * n;
  ptr = realloc(d->data, d->size + size + 1);
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
	curl_off_t size;
	CURLcode res = curl_easy_perform(curl);
	if (res != CURLE_OK) {
		if (error)
			*error = strdup(curl_easy_strerror(res));
		curl_easy_cleanup(curl);
		return 0;
	}
	/* now extract transfer info */
	if (fDownload)
#if LIBCURL_VERSION_NUM >= 0x071904
		curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD_T, &size);
#else
		curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &size);
#endif
	
	else
		
#if LIBCURL_VERSION_NUM >= 0x071904
		curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_UPLOAD_T, &size);
#else
		curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_UPLOAD, &size);
#endif
	
	/* always cleanup */
	curl_easy_cleanup(curl);
	return (size_t)size;
}
		
size_t curl_transfer_download_file(
		FILE *fp, const char *url, char **error, 
		void *ptr, 
		curl_transfer_progress *progress)
{
	CURL *curl;

	if (!fp){
		if (error)
			*error = strdup("fp argument is NULL");
		return 0;
	}
	curl = _curl_transfer_init(
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
	CURL *curl; 
	struct _curl_transfer_data d = {NULL, 0};

	if (!data){
		if (error)
			*error = strdup("data argument is NULL");
		return 0;
	}

	d.data = malloc(1);
	if (!d.data){
		if (error)
			*error = strdup("can't allocate memory");
		return 0;
	}
	curl = _curl_transfer_init(
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
	CURL *curl;

	if (!fp){
		if (error)
			*error = strdup("fp argument is NULL");
		return 0;
	}
	curl = _curl_transfer_init(
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
	CURL *curl; 
	struct _curl_transfer_data d = {data, size};

	if (!data){
		if (error)
			*error = strdup("data argument is NULL");
		return 0;
	}
	curl = _curl_transfer_init(
				0, NULL, &d, size, 
				url, error, ptr, progress);
	if (curl)	
		return _curl_transfer_perfom(0, curl, error);	
	
	return 0;
}
