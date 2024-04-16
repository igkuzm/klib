/**
 * File              : reachability.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 15.09.2021
 * Last Modified Date: 16.04.2024
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */

/*
 * chech's network is reachable
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
#include <string.h>
#include <unistd.h> //for sleep
#include <netdb.h>  //gethostbyname
#include <ctype.h>  //for isalpha

#if defined _WIN32
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

#include <pthread.h>

// function returns true if address and port is reachable, otherwise - false
static bool ip_address_is_reachable(const char *address, int port, char *error);

// function starts in additional THREAD, checks reachability of adress (hostname
// or ip4 address) and port
// every seconds_to_sleep seconds and executes callback function - return thread
// id (pthread_t)
// to stop reachability function and close THREAD - retun no zero in callback
static pthread_t
reachability(const char *address, int port, int timeout, void *user_data,
             int (*callback)(void *user_data, bool reachable, char *error));

/**
 * Implimation
 */

bool ip_address_is_reachable(const char *address, int port, char *error) {
  char _error[BUFSIZ];
  if (!error)
    error = _error;

#if defined _WIN32
  //----------------------
  // Initialize Winsock
  WSADATA wsaData;
  int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
  if (iResult != NO_ERROR) {
    sprintf(error, "reachability: WSAStartup function failed with error: %d\n",
            iResult);
    perror(error);
    return false;
  }
  //----------------------
  // Create a SOCKET for connecting to server
  SOCKET ConnectSocket;
  ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (ConnectSocket == INVALID_SOCKET) {
    sprintf(error, "reachability: socket function failed with error: %ld\n",
            WSAGetLastError());
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
  iResult =
      connect(ConnectSocket, (SOCKADDR *)&clientService, sizeof(clientService));
  if (iResult == SOCKET_ERROR) {
    sprintf(error, "reachability: connect function failed with error: %ld\n",
            WSAGetLastError());
    perror(error);
    iResult = closesocket(ConnectSocket);
    if (iResult == SOCKET_ERROR)
      sprintf(error,
              "reachability: closesocket function failed with error: %ld\n",
              WSAGetLastError());
    perror(error);
    WSACleanup();
    return false;
  }

  iResult = closesocket(ConnectSocket);
  if (iResult == SOCKET_ERROR) {
    sprintf(error,
            "reachability: closesocket function failed with error: %ld\n",
            WSAGetLastError());
    perror(error);
    WSACleanup();
    return false;
  }

  WSACleanup();

#else
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);

  struct sockaddr_in sin;
  sin.sin_family = AF_INET;
  sin.sin_port = htons(port); // Could be anything
  inet_pton(AF_INET, address, &sin.sin_addr);

  if (connect(sockfd, (struct sockaddr *)&sin, sizeof(sin)) == -1) {
    sprintf(error, "reachability: error connecting %s:%d - %d (%s)\n", address,
            port, errno, strerror(errno));
    perror(error);
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
  int (*callback)(void *user_data, bool reachable, char *error);
  void *user_data;
};

static void *check_address_is_reachable(void *params) {
  struct reachability_params *p = (struct reachability_params *)params;

  while (1) {
    char error[BUFSIZ];
    const char *address = p->address;
    if (p->find_ip) {
      struct hostent *hp = gethostbyname(p->address);
      if (hp) {
        if (hp->h_addrtype == AF_INET) {
          char ip_address[INET_ADDRSTRLEN];
          inet_ntop(AF_INET, hp->h_addr_list[0], ip_address, INET_ADDRSTRLEN);
          address = ip_address;
        } else {
          sprintf(error, "reachability: host %s has no ipv4 address",
                  p->address);
          if (p->callback)
            p->callback(p->user_data, false, error);
        }
      } else {
        sprintf(error, "reachability: cant get host data for hostname: %s",
                p->address);
        if (p->callback)
          p->callback(p->user_data, false, error);
      }
    }
    if (address && *address != 0) {
      if (p->callback) {
        if (p->callback(p->user_data,
                        ip_address_is_reachable(address, p->port, error),
                        error)) {
          // stop function if callback returned not zero
          perror("Reachability function stoped as callback returned non zero");
          break;
        }
      }
    } else if (p->callback)
      p->callback(p->user_data, false,
                  (char *)"reachability: cant't read address");

    sleep(p->timeout);
  }

  free(p);
  p = NULL;

  pthread_exit(0);
}

static pthread_t reachability_dispatch(const char *address, bool find_ip, int port,
                                int timeout, void *user_data,
                                int (*callback)(void *user_data, bool reachable,
                                                char *error)) {

  if (!address || *address == 0) {
    if (callback)
      callback(user_data, false, (char *)"reachability: cant't read address");
    return 0;
  }

  int err;

  pthread_t tid;       //идентификатор потока
  pthread_attr_t attr; //атрибуты потока

  //получаем дефолтные значения атрибутов
  err = pthread_attr_init(&attr);
  if (err) {
    if (callback)
      callback(user_data, false,
               (char *)"reachability: can't set THREAD attributes");
    return 0;
  }

  // allocate Reachability_params
  struct reachability_params *p =
      (struct reachability_params *)malloc(sizeof(struct reachability_params));
  if (p == NULL) {
    if (callback)
      callback(user_data, false, (char *)"reachability: can't allocate memory");
    return 0;
  }
  p->address = address;
  p->port = port;
  p->timeout = timeout;
  p->callback = callback;
  p->user_data = user_data;
  p->find_ip = find_ip;

  //создаем новый поток
  err = pthread_create(&tid, &attr, check_address_is_reachable, p);
  if (err) {
    if (callback)
      callback(user_data, false, (char *)"reachability: can't create THREAD");
    return 0;
  }

  return tid;
}

pthread_t
reachability(const char *address, int port, int timeout, void *user_data,
             int (*callback)(void *user_data, bool reachable, char *error)) {
  if (isalpha(address[0])) {
    return reachability_dispatch(address, true, port, timeout, user_data,
                                 callback);
  } else {
    return reachability_dispatch(address, false, port, timeout, user_data,
                                 callback);
  }
}

#ifdef __cplusplus
}
#endif

#endif // k_lib_reachability_h__
