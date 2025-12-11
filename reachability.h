/**
 * File              : reachability.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 15.09.2021
 * Last Modified Date: 11.12.2025
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */

/*
 * check network is reachable
 */

#ifndef k_lib_reachability_h__
#define k_lib_reachability_h__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/types.h>

#include <errno.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>  //for isalpha

#if defined _WIN32
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> //for sleep
#include <netdb.h>  //gethostbyname
#endif

#define TIMEOUT_SECONDS 2

#include <pthread.h>

/* function returns true if address and port is reachable, 
 * otherwise - false */
static bool ip_address_is_reachable(
		const char *address, int port);

/* function starts in additional THREAD, checks reachability
 * of adress (hostname or ip4 address) and port every
 * seconds_to_sleep seconds and executes callback function -
 * return thread id (pthread_t) to stop reachability
 * function and close THREAD - retun non zero in callback */
static pthread_t
reachability(
		const char *address, int port, int timeout, 
		void *user_data,
		int (*callback)(void *user_data, bool reachable));

/**
 * Implimation
 */

bool ip_address_is_reachable(const char *address, int port) 
{
	char e[BUFSIZ];

#if defined _WIN32
  //----------------------
  // Initialize Winsock
  WSADATA wsaData;
  int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
  if (iResult != NO_ERROR) {
    fprintf(stderr, "reachability: WSAStartup function failed with error: %d\n", iResult);
    return false;
  }
  //----------------------
  // Create a SOCKET for connecting to server
  SOCKET ConnectSocket;
  ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (ConnectSocket == INVALID_SOCKET) {
    fprintf(stderr, 
				"reachability: socket function failed with error: %ld\n",
        WSAGetLastError());
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
  iResult = connect(
			ConnectSocket, 
			(SOCKADDR *)&clientService, 
			sizeof(clientService));
  if (iResult == SOCKET_ERROR) 
	{
    fprintf(stderr, 
				"reachability: connect function failed with error: %ld\n",
        WSAGetLastError());
    iResult = closesocket(ConnectSocket);
		if (iResult == SOCKET_ERROR){
      fprintf(stderr,
              "reachability: closesocket function failed with error: %ld\n",
              WSAGetLastError());
		}
    WSACleanup();
    return false;
  }

  iResult = closesocket(ConnectSocket);
  if (iResult == SOCKET_ERROR) {
    fprintf(stderr,
            "reachability: closesocket function failed with error: %ld\n",
            WSAGetLastError());
  }

  WSACleanup();

#else
  int sockfd; 
	struct timeval tv;
  struct sockaddr_in sin;
  
	sockfd = 
		socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
		return false;

	tv.tv_sec  = TIMEOUT_SECONDS;
	tv.tv_usec = 0;

	setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, 
			&tv, sizeof(tv));
	setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, 
			&tv, sizeof(tv));

  sin.sin_family = AF_INET;
  sin.sin_port = htons(port); // Could be anything
  inet_pton(AF_INET, address, &sin.sin_addr);

  if (connect(
				sockfd,
			 	(struct sockaddr *)&sin, 
				sizeof(sin)) == -1) 
	{
		sprintf(e, 
				"reachability: error connecting %s:%d", 
				address, port);
		perror(e);
    return false;
	}
#endif

  return true;
}

// struct to pass arguments from thread to function
struct reachability_params {
  const char *address;
  bool find_ip;
  int port;
  int timeout;
  int (*callback)(void *user_data, bool reachable);
  void *user_data;
};

static void *_check_address_is_reachable(void *params)
{
  struct reachability_params *p = 
		(struct reachability_params *)params;

  while (1) {
    char e[BUFSIZ];
    const char *address = p->address;
    if (p->find_ip) {
      struct hostent *hp = gethostbyname(p->address);
      if (hp) {
        if (hp->h_addrtype == AF_INET) {
          char ip_address[INET_ADDRSTRLEN];
          inet_ntop(AF_INET, hp->h_addr_list[0],
						 	ip_address, INET_ADDRSTRLEN);
          address = ip_address;
        } else {
          if (p->callback)
            p->callback(p->user_data, false);
        }
      } else {
        if (p->callback)
          p->callback(p->user_data, false);
      }
    }
    if (address && *address != 0) 
		{
			char *error = NULL;
			bool fR = 
				ip_address_is_reachable(address, p->port);
      if (p->callback) {
        if (p->callback(p->user_data, fR)) 
				{
					if (error)
						free(error);
          break;
        }
      }
			if (error)
				free(error);
    } else if (p->callback)
			p->callback(p->user_data, false);
#ifdef _WIN32
    Sleep(p->timeout * 1000);
#else
    sleep(p->timeout);
#endif
  }

  free(p);
  p = NULL;

  pthread_exit(0);
}

static pthread_t 
_reachability_dispatch(
		const char *address, 
		bool find_ip, 
		int port,
    int timeout, 
		void *user_data,
		int (*callback)(
			void *user_data, 
			bool reachable)) 
{
  int err;

  pthread_t tid;       //идентификатор потока
  pthread_attr_t attr; //атрибуты потока
	
	assert(address);
  
	//получаем дефолтные значения атрибутов
  err = pthread_attr_init(&attr);
  if (err) 
    return 0;

  // allocate Reachability_params
  struct reachability_params *p =
      (struct reachability_params *)malloc(
					sizeof(struct reachability_params));
  if (p == NULL) 
    return 0;

  p->address = address;
  p->port = port;
  p->timeout = timeout;
  p->callback = callback;
  p->user_data = user_data;
  p->find_ip = find_ip;

  //создаем новый поток
  err = 
		pthread_create(
				&tid, 
				&attr, 
				_check_address_is_reachable, 
				p);
  if (err) {
    return 0;
  }

  return tid;
}

pthread_t
reachability(
		const char *address, 
		int port, 
		int timeout, 
		void *user_data,
    int (*callback)(
			void *user_data, 
			bool reachable)) 
{
  if (isalpha(address[0])) {
    return _reachability_dispatch(
				address, true, port, 
				timeout, user_data, callback);
  } else {
    return _reachability_dispatch(
				address, false, port, 
				timeout, user_data, callback);
  }		
}

#ifdef __cplusplus
}
#endif

#endif // k_lib_reachability_h__
