/**reachability.h
 * File              : reachability.c
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 15.09.2021
 * Last Modified Date: 23.02.2022
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */
#include "reachability.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#if defined _WIN32 || defined _WIN64
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>  //for sleep

bool k_lib_ipAddressIsReachable(const char *address, int port)
{
	char error[BUFSIZ];
#if defined _WIN32 || defined _WIN64
    //----------------------
    // Initialize Winsock
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != NO_ERROR) {
        sprintf(error,"WSAStartup function failed with error: %d\n", iResult);
		perror(error);
        return false;
    }
    //----------------------
    // Create a SOCKET for connecting to server
    SOCKET ConnectSocket;
    ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ConnectSocket == INVALID_SOCKET) {
        sprintf(error,"socket function failed with error: %ld\n", WSAGetLastError());
		perror(error);
        WSACleanup();
        return false;
    }
    //----------------------
    // The sockaddr_in structure specifies the address family,
    // IP address, and port of the server to be connected to.
    struct sockaddr_in clientService;
    clientService.sin_family = AF_INET;
    clientService.sin_addr.s_addr = inet_addr(address);
    clientService.sin_port = htons(port);

    //----------------------
    // Connect to server.
    iResult = connect(ConnectSocket, (SOCKADDR *) & clientService, sizeof (clientService));
    if (iResult == SOCKET_ERROR) {
        sprintf(error, "connect function failed with error: %ld\n", WSAGetLastError());
		perror(error);
        iResult = closesocket(ConnectSocket);
        if (iResult == SOCKET_ERROR)
            sprintf(error, "closesocket function failed with error: %ld\n", WSAGetLastError());
			perror(error);
        WSACleanup();
        return false;
    }

    iResult = closesocket(ConnectSocket);
    if (iResult == SOCKET_ERROR) {
        sprintf(error, "closesocket function failed with error: %ld\n", WSAGetLastError());
		perror(error);
        WSACleanup();
        return false;
    }

    WSACleanup();

#else
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port   = htons(port);  // Could be anything
	inet_pton(AF_INET, address, &sin.sin_addr);

	if (connect(sockfd, (struct sockaddr *) &sin, sizeof(sin)) == -1)
	{
		sprintf(error, "Error connecting %s:%d - %d (%s)\n", address, port, errno, strerror(errno));
		perror(error);
		return false;
	}
#endif

	return true;
}

//struct to pass arguments from thread to function
struct k_lib_reachability_params{
	const char *address;
	int port;
	int seconds_to_sleep;
	int (*callback)(bool isReachable, void *user_data);
	void *user_data;
};

void *k_lib_checkAddressIsReachable(void *param) 
{
	struct k_lib_reachability_params *reachability_params = param;

	const char *address = reachability_params -> address; 
	int port = reachability_params -> port;
	void *user_data = reachability_params->user_data;
	int seconds_to_sleep = reachability_params -> seconds_to_sleep; 
	int (*callback)(bool isReachable, void *user_data) = reachability_params -> callback;
	
	while (1) {
		if (callback(k_lib_ipAddressIsReachable(address, port), user_data)) {
			//stop function if callback returned not zero
			perror("Reachability function stoped as callback returned non zero");
			break;
		}
		sleep(seconds_to_sleep);
	}

	free(reachability_params);
	reachability_params=NULL;

	pthread_exit(0);
}


void k_lib_reachability(const char *address, int port, int seconds_to_sleep, void *user_data, int (*callback)(bool isReachable, void *user_data)){
	int err;

	pthread_t tid; //?????????????????????????? ????????????
	pthread_attr_t attr; //???????????????? ????????????

	//???????????????? ?????????????????? ???????????????? ??????????????????
	err = pthread_attr_init(&attr);
	if (err) {
		perror("Can't set THREAD attributes");
		exit(EXIT_FAILURE);
	}	

	//allocate Reachability_params
	struct k_lib_reachability_params reachability_params;
	reachability_params.address = address;
	reachability_params.port = port;
	reachability_params.seconds_to_sleep = seconds_to_sleep;
	reachability_params.callback = callback;
	reachability_params.user_data = user_data;

	//?????????????? ?????????? ??????????
	err = pthread_create(&tid, &attr, k_lib_checkAddressIsReachable, &reachability_params);
	if (err) {
		perror("Can't create THREAD");
		exit(EXIT_FAILURE);
	}

	//???????? ???????????????????? ???????????????????? ????????????
	/*err = pthread_join(tid,NULL);*/
	/*if (err) {*/
		/*fprintf(stderr, "Error in THREAD: %d, errno: %d (%s)\n", err, errno,  strerror(errno));*/
		/*exit(err);*/
	/*}	*/
}

