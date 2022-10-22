/**
 * File              : strio.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 27.05.2022
 * Last Modified Date: 22.10.2022
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */

#ifndef k_lib_striio_h__
#define k_lib_striio_h__

#ifdef __cplusplus
extern "C"{
#endif

#include <stdio.h>
#include <stdlib.h>

#define strio_read(str, src)\
	_Generic((src), \
			char*: _strio_read_file, \
			int:   _strio_read_fd, \
			FILE*: _strio_read_fp \
	)((str), (src))


#define strio_read_range(str, src, start, end)\
	_Generic((src), \
			char*: _strio_read_range_file, \
			int:   _strio_read_range_fd, \
			FILE*: _strio_read_renge_fp \
	)((str), (src))	

#define strio_write(str, dst)\
	_Generic((dst), \
			char*: _strio_write_file, \
			int:   _strio_write_fd, \
			FILE*: _strio_write_fp \
	)((str), (src))	

#define strio_write_to(str, dst, pos)\
	_Generic((dst), \
			char*: _strio_write_file_to, \
			int:   _strio_write_fd_to, \
			FILE*: _strio_write_fp_to \
	)((str), (src))	

#define strio_write_append(str, dst)\
	_Generic((dst), \
			char*: _strio_write_append_file, \
			int:   _strio_write_append_fd, \
			FILE*: _strio_write_append_fp \
	)((str), (src))	
/*
 * IMP
 */

static 
int _strio_read_fp  (char ** str, FILE * fp)
{
	if (!fp)
		return -1;
    
	size_t size = 0;
	int len = 0;
        
	if (fseek (fp, 0, SEEK_END))
		return -1;

    if ((size = ftell(fp)) < 0)
		return -1;
    
	if (fseek (fp, 0, SEEK_SET))
		return -1;

    char *buffer = malloc(size + sizeof(char));
	if (!buffer)
		return -1;

	while(1){
		char ch = fgetc(fp);
		if (ch == EOF) { 
			break; 
		}
		
		buffer[len++] = ch;
	}
	buffer[len] = 0;

	if (str)
		*str = buffer;
	else
		free(buffer);

	return len;
}

static 
int _strio_read_fd  (char ** str, const int fd){
	FILE *fp = fdopen(fd, "r");
	if(!fp)
		return -1;

	return _strio_read_fp(str, fp);
}

static int _strio_read_file(char ** str, const char * filename)
{
	FILE *fp = fopen(filename, "r");
	if(!fp)
		return -1;

	int len = _strio_read_fp(str, fp);
	fclose(fp);

	return len;
}


static 
int _strio_read_range_fp  (char ** str, FILE * fp, int start, int end)
{
	if (!fp)
		return -1;

	if (end <= start)
		return -1;
    
	int len = 0;
        
    char *buffer = malloc((end - start + 1)*sizeof(char));
	if (!buffer)
		return -1;

	if (fseek (fp, start * sizeof(char), SEEK_SET))
		return -1;

	while(1){
		char ch = fgetc(fp);
		if (ch == EOF) { 
			break; 
		}
		
		buffer[len++] = ch;

		if (len == end - start)
			break;
	}
	buffer[len] = 0;

	if (str)
		*str = buffer;
	else
		free(buffer);

	return len;
}

static 
int _strio_read_range_fd  (char ** str, const int fd, int start, int end){
	FILE *fp = fdopen(fd, "r");
	if(!fp)
		return -1;

	return _strio_read_range_fp(str, fp, start, end);
}

static int _strio_read_range_file(char ** str, const char * filename, int start, int end)
{
	FILE *fp = fopen(filename, "r");
	if(!fp)
		return -1;

	int len = _strio_read_range_fp(str, fp, start, end);
	fclose(fp);

	return len;
}


static 
int _strio_write_fp  (const char * str, FILE * fp)
{
	if (!fp)
		return -1;
    
	char * ptr = (char *)str; 
	while(ptr)
		fputc(*ptr++, fp);

	return 0;
}

static 
int _strio_write_fd  (const char * str, const int fd){
	FILE *fp = fdopen(fd, "w");
	if(!fp)
		return -1;

	return _strio_write_fp(str, fp);
}

static int _strio_write_file(const char * str, const char * filename)
{
	FILE *fp = fopen(filename, "w");
	if(!fp)
		return -1;

	int res = _strio_write_fp(str, fp);
	fclose(fp);

	return res;
}

static 
int _strio_write_fp_to  (const char * str, FILE * fp, size_t pos)
{
	if (!fp)
		return -1;

	if (fseek (fp, pos * sizeof(char), SEEK_SET))
		return -1;	
    
	char * ptr = (char *)str; 
	while(ptr)
		fputc(*ptr++, fp);

	return 0;
}

static 
int _strio_write_fd_to  (const char * str, const int fd, size_t pos){
	FILE *fp = fdopen(fd, "w");
	if(!fp)
		return -1;

	return _strio_write_fp_to(str, fp, pos);
}

static int _strio_write_file_to(const char * str, const char * filename, size_t pos)
{
	FILE *fp = fopen(filename, "w");
	if(!fp)
		return -1;

	int res = _strio_write_fp_to(str, fp, pos);
	fclose(fp);

	return res;
}

static 
int _strio_write_append_fp  (const char * str, FILE * fp)
{
	if (!fp)
		return -1;
    
	if (fseek (fp, 0, SEEK_END))
		return -1;
	
	char * ptr = (char *)str; 
	while(ptr)
		fputc(*ptr++, fp);

	return 0;
}

static 
int _strio_write_append_fd  (const char * str, const int fd){
	FILE *fp = fdopen(fd, "w");
	if(!fp)
		return -1;

	return _strio_write_append_fp(str, fp);
}

static int _strio_write_append_file(const char * str, const char * filename)
{
	FILE *fp = fopen(filename, "w");
	if(!fp)
		return -1;

	int res = _strio_write_append_fp(str, fp);
	fclose(fp);

	return res;
}

#ifdef __cplusplus
}
#endif

#endif //k_lib_string_h__

