#ifndef KLIB_THREAD_H
#define KLIB_THREAD_H

#ifdef __cplusplus
extern "C"{
#endif

#include <pthread.h>

/* run function on thread */
static pthread_t run_on_thread(void *data, 
                               void (*function)(void *data));
	
/* IMPLIMATION */
#include <stdio.h>
#include <stdlib.h>

struct thr{
	void *data;
	void (*function)(void *data);
};

static void * _routine(void * data)
{
	struct thr *thr = (struct thr *)data;
	if (thr->function)
		thr->function(thr->data);
	pthread_exit(NULL);
}

static pthread_t _thread_create(struct thr *thr)
{
	int err;
	pthread_t tid;
	//create new thread
	err = pthread_create(
			&tid, 
			NULL, 
			_routine, 
			thr);
	if (err) 
		return err;

	pthread_detach(tid);

	return tid;
}

pthread_t 
run_on_thread(void *data,
              void (*function)(void *data))
{
	pthread_t tid = 0;
	struct thr *thr = 
		(struct thr *)malloc(sizeof(struct thr));
	if (thr == NULL)
		return tid;

	thr->data = data;
	thr->function = function;

	return _thread_create(thr);
}

#ifdef __cplusplus
}
#endif

#endif /* ifndef KLIB_THREAD_H */
// vim:ft=c
