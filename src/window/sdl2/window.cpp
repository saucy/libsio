#include <map>
#include <iostream>
#include "window.h"

void test() {
	
}

namespace {
	
	template<typename evtT>
	void callSioEv(sio::window* w, std::thread::id thrdId, evtT& e) {
		sio::event_thread::push(thrdId, std::bind([=](evtT ev){
			w->on(ev);
		}, std::move(e)));
		
		sio::event_thread::async(thrdId);
	}
	
	template<typename evtT>
	void callSioEv(sio::window* w, std::thread::id thrdId) {
		evtT e;
		callSioEv(w, thrdId, e);
	}
	
	void windowEventHandler(
		sio::window* w, std::thread::id thrdId, SDL_WindowEvent& e)
	{
		switch(e.event) {
		case SDL_WINDOWEVENT_CLOSE:
			callSioEv<sio::window::close_event>(w, thrdId);
			break;
		case SDL_WINDOWEVENT_RESIZED:
		case SDL_WINDOWEVENT_SIZE_CHANGED:
			callSioEv<sio::window::resize_event>(w, thrdId);
			break;
		case SDL_WINDOWEVENT_FOCUS_GAINED:
			callSioEv<sio::window::kbfocus_event>(w, thrdId);
			break;
		case SDL_WINDOWEVENT_FOCUS_LOST:
			callSioEv<sio::window::kbblur_event>(w, thrdId);
			break;
		case SDL_WINDOWEVENT_ENTER:
			callSioEv<sio::window::mfocus_event>(w, thrdId);
			break;
		case SDL_WINDOWEVENT_LEAVE:
			callSioEv<sio::window::mblur_event>(w, thrdId);
			break;
		}
	}
	
	void windowKeydownHandler(
		sio::window* w, std::thread::id thrdId, SDL_KeyboardEvent& e)
	{
		sio::window::keydown_event sioE;
		sioE.key = e.keysym.scancode;
		sioE.repeat = e.repeat;
		sioE.mod = e.keysym.mod;
		
		callSioEv(w, thrdId, sioE);
	}
	
	void windowKeyupHandler(
		sio::window* w, std::thread::id thrdId, SDL_KeyboardEvent& e)
	{
		sio::window::keyup_event sioE;
		sioE.key = e.keysym.scancode;
		sioE.repeat = e.repeat;
		sioE.repeat = e.keysym.mod;
		
		callSioEv(w, thrdId, sioE);
	}
	
	void windowMouseButtonDownHandler(
		sio::window* w, std::thread::id thrdId, SDL_MouseButtonEvent& e)
	{
		sio::window::mbdown_event sioE;
		sioE.mb = e.button;
		sioE.x = e.x;
		sioE.y = e.y;
		
		callSioEv(w, thrdId, sioE);
	}
	
	void windowMouseButtonUpHandler(
		sio::window* w, std::thread::id thrdId, SDL_MouseButtonEvent& e) {
		sio::window::mbup_event sioE;
		sioE.mb = e.button;
		sioE.x = e.x;
		sioE.y = e.y;
		
		callSioEv(w, thrdId, sioE);
	}
	
	void windowMouseMoveHandler(
		sio::window* w, std::thread::id thrdId, SDL_MouseMotionEvent& e)
	{
		sio::window::mmove_event sioE;
		sioE.x = e.x;
		sioE.y = e.y;
		sioE.xrel = e.xrel;
		sioE.yrel = e.yrel;
		sioE.state = e.state;
		
		callSioEv(w, thrdId, sioE);	
	}
	
	void windowMouseWheelHandler(
		sio::window* w, std::thread::id thrdId, SDL_MouseWheelEvent& e)
	{
		sio::window::mwheel_event sioE;
		sioE.x = e.x;
		sioE.y = e.y;
		
		callSioEv(w, thrdId, sioE);
	}
	
	void windowDropFileHandler(
		sio::window* w, std::thread::id thrdId, SDL_DropEvent& e)
	{
		sio::window::drop_event sioE;
		sioE.path_name = std::string{e.file};
		
		callSioEv(w, thrdId, sioE);
	}
	
	void windowEventLoop(sio::window* w, std::thread::id thrdId) {
		SDL_Event e;
		
		while(SDL_WaitEvent(&e)) {
			switch(e.type) {
				case SDL_WINDOWEVENT:
					windowEventHandler(w, thrdId, e.window);
					break;
				case SDL_KEYDOWN:
					windowKeydownHandler(w, thrdId, e.key);
					break;
				case SDL_KEYUP:
					windowKeyupHandler(w, thrdId, e.key);
					break;
				case SDL_MOUSEBUTTONDOWN:
					windowMouseButtonDownHandler(w, thrdId, e.button);
					break;
				case SDL_MOUSEBUTTONUP:
					windowMouseButtonUpHandler(w, thrdId, e.button);
					break;
				case SDL_MOUSEMOTION:
					windowMouseMoveHandler(w, thrdId, e.motion);
					break;
				case SDL_MOUSEWHEEL:
					windowMouseWheelHandler(w, thrdId, e.wheel);
					break;
				case SDL_DROPFILE:
					windowDropFileHandler(w, thrdId, e.drop);
					break;
			}
		}
		
		std::cout << "Window Event Loop Ended!\n";
	}
	
	void createWindow(SIO_Window w, sio::window* wndw, std::thread::id thrdId, std::condition_variable* cv) {
		SDL_Init(SDL_INIT_EVERYTHING);
		
		w->window = SDL_CreateWindow("",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			640, 480,
			SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
		);
		
		cv->notify_one();
		
		windowEventLoop(wndw, thrdId);
		
	}
	
}

SIO_Window sio_CreateWindow(sio::window* wndw) {
	SIO_Window w = new _sioWindowImpl;
	
	std::mutex m;
	std::unique_lock<std::mutex> lk(m);
	std::condition_variable cv;
	
	std::thread evt(createWindow, w, wndw, std::this_thread::get_id(), &cv);
	evt.detach();
	
	cv.wait(lk);
	
	std::cout << "Done waiting\n";
	
	return w;
}

void sio_DestroyWindow(SIO_Window w) {
	SDL_DestroyWindow(w->window);
}

void sio_ShowWindow(SIO_Window w) {
	SDL_ShowWindow(w->window);
}

void sio_HideWindow(SIO_Window w) {
	SDL_HideWindow(w->window);
}

void sio_SetWindowTitle(SIO_Window w, std::string str) {
	SDL_SetWindowTitle(w->window, str.c_str());
}

std::string sio_GetWindowTitle(SIO_Window w) {
	return SDL_GetWindowTitle(w->window);
}

void sio_SetWindowFocus(SIO_Window w, bool b) {
	
}

void sio_GetWindowSize(SIO_Window wnd, int32_t*&& width, int32_t*&& height) {
	int w, h;
	SDL_GetWindowSize(wnd->window, &w, &h);
	
	(*width) = w;
	(*height) = h;
}

void sio_ResizeWindow(SIO_Window wnd, int32_t w, int32_t h) {
	SDL_SetWindowSize(wnd->window, w, h);
}
