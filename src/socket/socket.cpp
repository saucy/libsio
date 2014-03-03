#include "socket.h"

template<typename EventType>
static void CallEvent(sio::socket* sock, EventType e) {
	sock->on(e);
}

static void CallConnectionEvent(sio::socket* sock, std::thread::id& evtId, sio::socket client) {	
	sio::socket::connection_event e(std::move(client));
	sock->on(e);
}

void sio::socket::_sioSocket_connectionLoop() {
	for(;;) {
		sio::socket client;
		do {
			client._sioSocket_nativeImpl = sio_WaitForSocketConnection(_sioSocket_nativeImpl);
		} while(!sio_IsValidSocketNativeImpl(client._sioSocket_nativeImpl));
		
		CallConnectionEvent(this, _sioSocket_evtId, std::move(client));
		sio::event_thread::async(_sioSocket_evtId);
	}
}

void sio::socket::_sioSocket_messageLoop() {
	for(;;) {
		sio::socket::message_event e = sio_WaitForSocketMessage(_sioSocket_nativeImpl);
		if(e.size == 0) break;
		
		sio::event_thread::push(_sioSocket_evtId,
			std::bind(CallEvent<message_event>, this, e));
		sio::event_thread::async(_sioSocket_evtId);
	}
}

sio::socket::socket()
	: _sioSocket_nativeImpl(sio_CreateSocketNativeImpl())
	, _sioSocket_evtId(std::this_thread::get_id())
{ }

sio::socket::socket(socket&& sock)
	: _sioSocket_nativeImpl(sock._sioSocket_nativeImpl)
	, _sioSocket_evtId(std::this_thread::get_id())
{ 
	sock._sioSocket_nativeImpl = nullptr;
}

sio::socket::~socket() {
	close();
	shutdown();
}

sio::socket::operator bool() {
	return sio_IsValidSocketNativeImpl(_sioSocket_nativeImpl);
}

bool sio::socket::operator !() {
	return !sio_IsValidSocketNativeImpl(_sioSocket_nativeImpl);
}

void sio::socket::send(void* data, uint32_t size) {
	sio_SendSocketNativeImpl(_sioSocket_nativeImpl, data, size);
}

void sio::socket::close() {
	sio_CloseSocketNativeImpl(_sioSocket_nativeImpl);
}

void sio::socket::shutdown() {
	sio_ShutdownSocketNativeImpl(_sioSocket_nativeImpl);
}

sio::socket& sio::socket::listen(std::string ip, uint32_t port) {
	sio_ListenSocketNativeImpl(_sioSocket_nativeImpl, ip.c_str(), port);
	
	if(sio_IsValidSocketNativeImpl(_sioSocket_nativeImpl)) {
		std::thread(&socket::_sioSocket_connectionLoop, this).detach();
	}
	
	return *this;
}

sio::socket& sio::socket::listen(uint32_t port) {
	sio_ListenSocketNativeImpl(_sioSocket_nativeImpl, port);
	
	if(sio_IsValidSocketNativeImpl(_sioSocket_nativeImpl)) {
		std::thread(&socket::_sioSocket_connectionLoop, this).detach();
	}
	
	return *this;
}

sio::socket& sio::socket::connect(std::string ip) {
	sio_ConnectSocketNativeImpl(_sioSocket_nativeImpl, ip.c_str());
	
	if(sio_IsValidSocketNativeImpl(_sioSocket_nativeImpl)) {
		
	}
	
	return *this;
}

sio::socket& sio::socket::connect(std::string ip, uint32_t port) {
	sio_ConnectSocketNativeImpl(_sioSocket_nativeImpl, ip.c_str(), port);
	
	if(sio_IsValidSocketNativeImpl(_sioSocket_nativeImpl)) {
		std::thread(&socket::_sioSocket_messageLoop, this).detach();
	}
	
	return *this;
}

// EVENTS START HERE

void sio::socket::on(connection_event&) { }
void sio::socket::on(disconnect_event&) { }
void sio::socket::on(message_event&) { }

sio::socket::connection_event
::connection_event(sio::socket&& sock)
	: socket(std::move(sock))
{
	
}
