#include <sio/event_thread.h>
#include <iostream>
#include <string>
#include <utility>

void msg(int num) {
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	std::cout << std::string("sync in ") + std::to_string(num) + "...\n";
}

void countdown() {
	sio::this_thread::push(msg, 5);
	sio::this_thread::push(msg, 4);
	sio::this_thread::push(msg, 3);
	sio::this_thread::push(msg, 2);
	sio::this_thread::push(msg, 1);
	sio::this_thread::push(msg, 0);
	
	sio::this_thread::push([]{
		std::cout << "This message should appear at same time as main's message.\n";
	});
}


int main() {
	sio::event_thread evt;
	
	evt.push(countdown);
	evt.sync();
	
	std::cout << "Message from main.\n";
	
	sio::this_thread::event_loop();
	return 0;
}
