#include <vector>
#include <string>
#include <iostream>

#include <sio/socket.h>

class ChatClientSocket : public sio::socket {
public:
	ChatClientSocket(sio::socket&&);
	
	void on(message_event&) override;
};

ChatClientSocket::ChatClientSocket(sio::socket&& sock)
	: socket::socket(std::move(sock))
{}

void ChatClientSocket::on(message_event& e) {
	std::string str((const char*)e.data, e.size);
	std::cout << str << "\n";
}

class ChatServerSocket : public sio::socket {
	std::vector<ChatClientSocket> clients;
public:
	ChatServerSocket();
	
	void on(connection_event&) override;
};

ChatServerSocket::ChatServerSocket()
	: sio::socket::socket()
{
	listen(27015);
}

void ChatServerSocket::on(connection_event& e) {
	std::cout << "Someone joined chat!\n";
	clients.push_back(std::move(e.socket));
}

int main() {
	ChatServerSocket server;
	
	sio::this_thread::event_loop();
	return 0;
}
