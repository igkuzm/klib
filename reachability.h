/**
 * File              : reachability.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 15.09.2021
 * Last Modified Date: 22.02.2022
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */

#ifndef k_lib_reachability_h__
#define k_lib_reachability_h__
#endif

#ifdef __cplusplus
extern "C"{
#endif

#include <stdio.h>
#include <stdbool.h>

//function returns true if address and port is reachable, otherwise - false
bool k_lib_ipAddressIsReachable(const char *address, int port);

//function starts in additional THREAD, checks reachability of adress and port every seconds_to_sleep
//seconds and executes callback function
//to stop reachability function and close THREAD - retun no zero in callback
void k_lib_reachability(const char *address, int port, int seconds_to_sleep, void *user_data, int (*callback)(bool isReachable, void *user_data));

#ifdef __cplusplus
}
#endif

