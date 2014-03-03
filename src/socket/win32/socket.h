#ifndef SIO_SRC_SOCKET_WIN32_SOCKET_HEADER
#define SIO_SRC_SOCKET_WIN32_SOCKET_HEADER

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#include "../socket.h"

typedef struct {
	SOCKET sock;
	
} SIOwin32_socket;

#endif//SIO_SRC_SOCKET_WIN32_SOCKET_HEADER
