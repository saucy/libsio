#ifndef SIO_SRC_SOCKET_SOCKET_HEADER
#define SIO_SRC_SOCKET_SOCKET_HEADER

#include <algorithm>
#include <utility>

#include "../../include/sio/socket.h"

void* sio_CreateSocketNativeImpl();

void sio_DeleteSocketNativeImpl(void*&);

bool sio_IsValidSocketNativeImpl(void*&);

void sio_ListenSocketNativeImpl(void*&, const char* ip, uint32_t port);

void sio_ListenSocketNativeImpl(void*&, uint32_t port);

void sio_ConnectSocketNativeImpl(void*&, const char* ip);

void sio_ConnectSocketNativeImpl(void*&, const char* ip, uint32_t port);

void sio_CloseSocketNativeImpl(void*&);

void sio_ShutdownSocketNativeImpl(void*&);

void* sio_WaitForSocketConnection(void*&);

void sio_SendSocketNativeImpl(void*&, void* data, uint32_t size);

sio::socket::message_event sio_WaitForSocketMessage(void*&);

#endif//SIO_SRC_SOCKET_SOCKET_HEADER
