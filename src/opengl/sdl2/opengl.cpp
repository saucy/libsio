#include <iostream>

#include "opengl.h"

namespace gl = sio::opengl;

gl::context gl::create_context(window& w) {
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	
	#ifdef DEBUG
	
	std::cout << "[LIBSIO WARNING] Created Context with DEBUG flag.\n";
	
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
	
	#endif
	
	gl::context ctx;
	ctx = SDL_GL_CreateContext(w._sioWindow_impl->window);
	
	return ctx;
}

gl::context gl::create_context(window& w, context shareCtx) {
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	
	SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);
	
	#ifdef DEBUG
	
	std::cout << "[LIBSIO WARNING] Created Context with DEBUG flag.\n";
	
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
	
	#endif
	
	gl::context ctx;
	SDL_GL_MakeCurrent(w._sioWindow_impl->window, shareCtx);
	ctx = SDL_GL_CreateContext(w._sioWindow_impl->window);
	
	return ctx;
}

void gl::delete_context(context ctx) {
	SDL_GL_DeleteContext(ctx);
}

void gl::current_context(window& w, context ctx) {
	SDL_GL_MakeCurrent(w._sioWindow_impl->window, ctx);
}

gl::context gl::current_context() {
	return SDL_GL_GetCurrentContext();
}

void gl::swap_buffers(window& w) {
	SDL_GL_SwapWindow(w._sioWindow_impl->window);
}

void gl::swap_buffers() {
	SDL_GL_SwapWindow(SDL_GL_GetCurrentWindow());
}
