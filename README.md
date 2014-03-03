## libsio

#### Window Example
```c++
#include <sio/event_thread.h>
#include <sio/window.h>

class Window
	: public sio::window {
public:
	Window();
	
	void on(keydown_event&) override;
};

TestWindow::TestWIndow() {
	title("Untitled Window");
}

void TestWindow::on(keydown_event& e) {
	switch(e.key) {
	case KEY_ESCAPE:
		exit(0);
		break;
	}
}

int main() {
	Window window;
	
	sio::this_thread::event_loop();
	return 0;
}
```
