/**reachability.h
 * File              : reachability.c
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 15.09.2021
 * Last Modified Date: 02.08.2022
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
#include <unistd.h>  //for sleep
#include <netdb.h>
#include <ctype.h>  //for isalpha


bool ip_address_is_reachable(const char *address, int port, char * error)
{
	char _error[BUFSIZ];
	if (!error) error = _error; 

#if defined _WIN32 || defined _WIN64
    //----------------------
    // Initialize Winsock
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != NO_ERROR) {
        sprintf(error,"reachability: WSAStartup function failed with error: %d\n", iResult);
		perror(error);
        return false;
    }
    //----------------------
    // Create a SOCKET for connecting to server
    SOCKET ConnectSocket;
    ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ConnectSocket == INVALID_SOCKET) {
        sprintf(error,"reachability: socket function failed with error: %ld\n", WSAGetLastError());
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
        sprintf(error, "reachability: connect function failed with error: %ld\n", WSAGetLastError());
		perror(error);
        iResult = closesocket(ConnectSocket);
        if (iResult == SOCKET_ERROR)
            sprintf(error, "reachability: closesocket function failed with error: %ld\n", WSAGetLastError());
			perror(error);
        WSACleanup();
        return false;
    }

    iResult = closesocket(ConnectSocket);
    if (iResult == SOCKET_ERROR) {
        sprintf(error, "reachability: closesocket function failed with error: %ld\n", WSAGetLastError());
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
		sprintf(error, "reachability: error connecting %s:%d - %d (%s)\n", address, port, errno, strerror(errno));
		perror(error);
		return false;
	}
#endif

	return true;
}

//struct to pass arguments from thread to function
struct reachability_params{
	const char *address;
	bool find_ip;
	int port;
	int seconds_to_sleep;
	int (*callback)(void *user_data, bool isReachable, char * error);
	void *user_data;
};

void *check_address_is_reachable(void * params) 
{
	struct reachability_params * p = params;
	
	while (1) {
		const char *address;
		if (p->find_ip){
			struct hostent * hp = gethostbyname(p->address);
			struct in_addr ** p = (struct in_addr **)hp->h_addr_list;
			char ip_address[INET_ADDRSTRLEN];
			int i;
			for (i = 0; p[i]!=NULL; ++i) {
				inet_ntop(AF_INET, &p[i], ip_address, INET_ADDRSTRLEN);
			}
			address = ip_address;
		}
		char error[BUFSIZ];
		if (p->callback(p->user_data, ip_address_is_reachable(address, p->port, error), error)) {
			//stop function if callback returned not zero
			perror("Reachability function stoped as callback returned non zero");
			break;
		}
		sleep(p->seconds_to_sleep);
	}

	free(p);
	p=NULL;

	pthread_exit(0);
}

pthread_t dispatch(const char *address, bool find_ip, int port, int seconds_to_sleep, void *user_data, int (*callback)(void * user_data, bool isReachable, char * error)){

	int err;

	pthread_t tid; //идентификатор потока
	pthread_attr_t attr; //атрибуты потока

	//получаем дефолтные значения атрибутов
	err = pthread_attr_init(&attr);
	if (err){
		if (callback)
			callback(user_data, false, "reachability: can't set THREAD attributes");
		return 0;
	}

	//allocate Reachability_params
	struct reachability_params * p = malloc(sizeof(struct reachability_params));
	if (p == NULL) {
		if (callback)
			callback(user_data, false, "reachability: can't allocate memory");
		return 0;
	}
	p->address = address;
	p->port = port;
	p->seconds_to_sleep = seconds_to_sleep;
	p->callback = callback;
	p->user_data = user_data;
	p->find_ip = find_ip;

	//создаем новый поток
	err = pthread_create(&tid, &attr, check_address_is_reachable, p);
	if (err){
		if (callback)
			callback(user_data, false, "reachability: can't create THREAD");
		return 0;
	}

	return tid;	
}

pthread_t reachability(const char *address, int port, int seconds_to_sleep, void *user_data, int (*callback)(void *user_data, bool isReachable, char * error)){
	if (isalpha(address[0])) {
		return dispatch(address, false, port, seconds_to_sleep, user_data, callback);
	} else {
		return dispatch(address, true,  port, seconds_to_sleep, user_data, callback);
	}
}
