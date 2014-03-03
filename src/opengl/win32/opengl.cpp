#include "opengl.h"

namespace gl = sio::gl;

gl::context_t gl::create_context(window& w) {
	return (gl::context_t)wglCreateContext(GetDC(w._sioWindow_impl->hwnd));
}

void gl::delete_context(context_t ctx) {
	wglDeleteContext((HGLRC)ctx);
}

void gl::current_context(window& w, context_t ctx) {
	wglMakeCurrent(GetDC(w._sioWindow_impl->hwnd), (HGLRC)ctx);
}

gl::context_t gl::current_context() {
	return (gl::context_t)wglGetCurrentContext();
}

void gl::swap_buffers(window& w) {
	SwapBuffers(GetDC(w._sioWindow_impl->hwnd));
}

void gl::swap_buffers() {
	SwapBuffers(wglGetCurrentDC());
}
