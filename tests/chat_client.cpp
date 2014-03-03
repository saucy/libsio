#include <iostream>

#include <sio/socket.h>

class ChatClientSocket : public sio::socket {
public:
	ChatClientSocket();
};

ChatClientSocket::ChatClientSocket()
	: sio::socket::socket()
{
	this->connect("70.69.189.38", 27015);
	this->send((void*)"hello", 5);
}
	
void WaitMessageFunc(std::string msg, std::function<bool()> f) {
	std::chrono::milliseconds tickDuration(167);
	using clock = std::chrono::system_clock;
	
	if(!f()) {
		for(unsigned i=0; 3 > i; i++) {
			std::cout << ".";
			std::this_thread::sleep_for(tickDuration);
		}
		
		if(f()) {
			std::cout << "\b\b\b   \n";
			std::cout << std::string(msg.length(), '\b') << std::string(msg.length(), ' ') << std::string(msg.length(), '\b');
			return;
		}
		
		std::this_thread::sleep_for(tickDuration*2);
		
		for(unsigned i=0; 3 > i; i++) {
			std::cout << "\b \b";
			std::this_thread::sleep_for(tickDuration);
		}
		
		sio::this_thread::push(WaitMessageFunc, msg, f);
	} else {
		std::cout << std::string(msg.length(), '\b') << std::string(msg.length(), ' ') << std::string(msg.length(), '\b');
	}
}
	
void WaitMessage(std::string msg, std::function<bool()> f) {
	std::cout << msg;
	
	sio::this_thread::push(WaitMessageFunc, msg, f);
}

int main() {
	ChatClientSocket client;
	
	WaitMessage("Connecting to server", [&]{
		return client ? true : false;
	});
	
	sio::this_thread::event_loop();
	return 0;
}
