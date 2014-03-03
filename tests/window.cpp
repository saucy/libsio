#include <iostream>
#include <string>
#include <sio/window.h>
#include <sio/opengl.h>

#include <GL/gl.h>

namespace gl = sio::opengl;

class Window
	: public sio::window {
private:
	gl::context glCtx;
	void initOpenGL();
public:
	sio::event_thread openGLThread;
	Window();
	
	void on(keydown_event&) override;
};

void Window::initOpenGL() {
	glCtx = gl::create_context(*this);
}

Window::Window()
	: openGLThread(std::bind(&Window::initOpenGL, this))
{
	openGLThread.sync();
	
	show();
}

void Window::on(keydown_event& e) {
	switch(e.key) {
		case KEY_ESCAPE:
			exit(0);
			break;
		case KEY_R:
			openGLThread.push([]{
				glClearColor(1.0, 0.0, 0.0, 1.0);
				glClear(GL_COLOR_BUFFER_BIT);
				gl::swap_buffers();
			});
			break;
		case KEY_G:
			openGLThread.push([]{
				glClearColor(0.0, 1.0, 0.0, 1.0);
				glClear(GL_COLOR_BUFFER_BIT);
				gl::swap_buffers();
			});
			break;
		case KEY_B:
			openGLThread.push([]{
				glClearColor(0.0, 0.0, 1.0, 1.0);
				glClear(GL_COLOR_BUFFER_BIT);
				gl::swap_buffers();
			});
			break;
	}
	
	if(e.key == KEY_R || e.key == KEY_G || e.key == KEY_B) {
		openGLThread.async();
	}
}

int main() {
	Window w;
	std::cout << " <ESCAPE> - Exit application at anytime."     "\n"
	             "      <R> - Change window's color to red."    "\n"
	             "      <G> - Change window's color to green."  "\n"
	             "      <B> - Change window's color to blue."   "\n"
	             "\n";
	
	sio::this_thread::event_loop();
	return 0;
}
