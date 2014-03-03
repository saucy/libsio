#ifndef SIO_SOCKET_HEADER
#define SIO_SOCKET_HEADER

#include "event_thread.h"

namespace sio {
	class socket;
	
	class socket {
	public:
		class connection_event;
		class disconnect_event;
		class message_event;
		
		/**
		 * \brief Create's an invalid socket. Neither listening nor connected to anything.
		 */
		socket();
		socket(socket&&);
		virtual ~socket();
		
		//socket& operator =(sio::socket&&);
		
		//bool operator ==(sio::socket&);
		//bool operator !=(sio::socket&);
		
		operator bool();
		bool operator !();
		
		void send(void* data, uint32_t size);
		
		void close();
		void shutdown();
		
		socket& listen(uint32_t port);
		socket& listen(std::string ip, uint32_t port);
		
		/**
		 * \brief Connect to socket on ip address finding any open port.
		 *  
		 * \param ip - Ip address to connec to. Can be ipv4 or ipv6.
		 */
		socket& connect(std::string ip);
		
		/**
		 * \brief Connect to socket on ip address and port.
		 * 
		 * \param ip - ip address to connect to.
		 * \param port - port to use.
		 */
		socket& connect(std::string ip, uint32_t port);
		
		virtual void on(connection_event&);
		virtual void on(disconnect_event&);
		virtual void on(message_event&);
	private:
		std::thread::id _sioSocket_evtId;
		void* _sioSocket_nativeImpl;
		void _sioSocket_connectionLoop();
		void _sioSocket_messageLoop();
	};
	
	class socket::connection_event {
	public:
		sio::socket socket;
		connection_event(sio::socket&&);
	};
	
	class socket::disconnect_event {
	public:
		
	};
	
	class socket::message_event {
	public:
		void* data;
		uint32_t size;
	};
}

#endif//SIO_SOCKET_HEADER
