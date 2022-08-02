/**
 * File              : reachability.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 15.09.2021
 * Last Modified Date: 02.08.2022
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */

#ifndef k_lib_reachability_h__
#define k_lib_reachability_h__

#ifdef __cplusplus
extern "C"{
#endif

#include <stdio.h>
#include <stdbool.h>

#ifndef __ANDROID__
#include <pthread.h>
#endif

//function returns true if address and port is reachable, otherwise - false
bool ip_address_is_reachable(const char *address, int port, char * error);

//function starts in additional THREAD, checks reachability of adress (hostname or ip4 address) and port 
//every seconds_to_sleep seconds and executes callback function - return thread id (pthread_t)
//to stop reachability function and close THREAD - retun no zero in callback
pthread_t reachability(const char *address, int port, int seconds_to_sleep, void *user_data, int (*callback)(void *user_data, bool isReachable, char * error));

#ifdef __cplusplus
}
#endif

#endif //k_lib_reachability_h__
