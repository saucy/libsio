#include <iostream>

#include "window.h"

sio::window::window()
	: _sioWindow_impl(sio_CreateWindow(this)) {
	
}

sio::window::~window() {
	sio_DestroyWindow(_sioWindow_impl);
}

void sio::window::destroy() {
	sio_DestroyWindow(_sioWindow_impl);
}

void sio::window::show() {
	sio_ShowWindow(_sioWindow_impl);
}

void sio::window::hide() {
	sio_HideWindow(_sioWindow_impl);
}

void sio::window::title(std::string title) {
	sio_SetWindowTitle(_sioWindow_impl, title);
}

std::string sio::window::title() const {
	return sio_GetWindowTitle(_sioWindow_impl);
}

void sio::window::focus() {
	sio_SetWindowFocus(_sioWindow_impl, true);
}

void sio::window::blur() {
	sio_SetWindowFocus(_sioWindow_impl, false);
}

void sio::window::size(const int32_t in_width, const int32_t in_height) {
	sio_ResizeWindow(_sioWindow_impl, in_width, in_height);
}

void sio::window::size(int32_t*&& out_width, int32_t*&& out_height) const {
	sio_GetWindowSize(_sioWindow_impl, std::move(out_width), std::move(out_height));
}

void sio::window::on(keydown_event& e) {}
void sio::window::on(keyup_event& e)   {}
void sio::window::on(kbfocus_event& e) {}
void sio::window::on(kbblur_event& e)  {}

void sio::window::on(mbdown_event& e)  {}
void sio::window::on(mbup_event& e)    {}
void sio::window::on(mfocus_event& e)  {}
void sio::window::on(mblur_event& e)   {}
void sio::window::on(mmove_event& e)   {}
void sio::window::on(mwheel_event& e)  {}

void sio::window::on(close_event& e)   {}
void sio::window::on(resize_event& e)  {}
void sio::window::on(redraw_event& e)  {}

void sio::window::on(drop_event& e)    {}
