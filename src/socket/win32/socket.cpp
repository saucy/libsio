#include "socket.h"

static WSADATA  wsaData;

namespace {

	struct init {
		init();
		~init();
	} init;

}

init::init() {
	WSAStartup(MAKEWORD(2,2), &wsaData);
}

init::~init() {
	WSACleanup();
}

void* sio_CreateSocketNativeImpl() {
	SOCKET sock = INVALID_SOCKET;
	
	return (void*)sock;
}

bool sio_IsValidSocketNativeImpl(void*& impl) {
	return (SOCKET&)impl != INVALID_SOCKET;
}

void sio_CloseSocketNativeImpl(void*& impl) {
	SOCKET& sock = (SOCKET&)impl;
	closesocket(sock);
}

void sio_ShutdownSocketNativeImpl(void*& impl) {
	SOCKET& sock = (SOCKET&)impl;
	shutdown(sock, SD_BOTH);
}

void sio_SendSocketNativeImpl(void*& impl, void* data, uint32_t size) {
	SOCKET& sock = (SOCKET&)impl;
	
	int res = send(sock, (const char*)data, (int)size, 0);
	if(res == SOCKET_ERROR) {
		printf("Could not send data!\n");
		closesocket(sock);
		WSACleanup();
		exit(1);
		return;
	}
}

sio::socket::message_event sio_WaitForSocketMessage(void*& impl) {
	SOCKET& sock = (SOCKET&)impl;
	sio::socket::message_event e;
	int res;
	char sockBuf[512];
	
	res = recv(sock, sockBuf, 512, 0);
		
	if(res > 0) {
		e.data = malloc(res);
		memcpy(e.data, sockBuf, res);
		e.size = res;
	} else
	if(res == 0) {
		e.data = nullptr;
		e.size = 0;
	} else {
		printf("recv failed!\n");
		e.data = nullptr;
		e.size = 0;
	}
	
	
	
	return e;
}

void sio_ListenSocketNativeImpl(void*& impl, const char* ip, uint32_t port) {

}

void sio_ListenSocketNativeImpl(void*& impl, uint32_t port) {
	SOCKET& sock = (SOCKET&)impl;
	
	struct addrinfo* result = NULL;
	struct addrinfo* ptr = NULL;
	struct addrinfo hints;
	
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;
	
	int iResult = getaddrinfo(NULL, std::to_string(port).c_str(), &hints, &result);
	if(iResult != 0) {
		WSACleanup();
		exit(1);
		return;
	}
	
	sock = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if(sock == INVALID_SOCKET) {
		freeaddrinfo(result);
		WSACleanup();
		exit(1);
		return;
	}
	
	iResult = bind(sock, result->ai_addr, (int)result->ai_addrlen);
	if(iResult == SOCKET_ERROR) {
		freeaddrinfo(result);
		closesocket(sock);
		WSACleanup();
		exit(1);
		return;
	}
	
	freeaddrinfo(result);
	
	listen(sock, SOMAXCONN);
}

void sio_ConnectSocketNativeImpl(void*& impl, const char* ip) {
	printf("need port to connect\n");
	
}

void sio_ConnectSocketNativeImpl(void*& impl, const char* ip, uint32_t port) {
	SOCKET& sock = (SOCKET&)impl;
	
	struct addrinfo* result = NULL;
	struct addrinfo* ptr = NULL;
	struct addrinfo hints;
	
	int iResult;
	
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;
	
	iResult = getaddrinfo(ip, std::to_string(port).c_str(), &hints, &result);
	if(iResult != 0) {
		printf("getaddrinfo failed\n");
		WSACleanup();
		exit(1);
		return;
	}
	
	for(ptr=result; ptr != NULL; ptr=ptr->ai_next) {
		sock = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if(sock == INVALID_SOCKET) {
			printf("socket failed to create!\n");
			WSACleanup();
			exit(1);
			return;
		}
		
		printf("Attempting to connect to the server...\n");
		iResult = connect(sock, ptr->ai_addr, (int)ptr->ai_addrlen);
		if(iResult != 0) {
			closesocket(sock);
			sock = INVALID_SOCKET;
			continue;
		}
		break;
	}
	
	if(sock == INVALID_SOCKET) {
		printf("Unable to connect to server.\n");
	}
	
}

void* sio_WaitForSocketConnection(void*& impl) {
	SOCKET& sock = (SOCKET&)impl;
	
	printf("Waiting for connection...\n");
	SOCKET client = accept(sock, NULL, NULL);
	printf("Someone connected!\n");
	
	return (void*)client;
}
