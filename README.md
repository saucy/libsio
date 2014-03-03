## libsio

#### Window Example
```c++
#include <sio/event_thread.h>
#include <sio/window.h>

class TestWindow
	: public sio::window {
public:
	TestWindow();
	
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
	TestWindow testWindow;
	
	sio::this_thread::event_loop();
	return 0;
}
```
