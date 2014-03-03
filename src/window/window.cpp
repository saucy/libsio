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

int32_t sio::window::width() const {
	int32_t windowWidth, windowHeight;
	sio_GetWindowSize(_sioWindow_impl, &windowWidth, &windowHeight);
	
	return windowWidth;
}

int32_t sio::window::height() const {
	int32_t windowWidth, windowHeight;
	sio_GetWindowSize(_sioWindow_impl, &windowWidth, &windowHeight);
	
	return windowHeight;
}

void sio::window::width(const int32_t w) {
	sio_ResizeWindow(_sioWindow_impl, w, height());
}

void sio::window::height(const int32_t h) {
	sio_ResizeWindow(_sioWindow_impl, width(), h);
}

int32_t sio::window::pos_x() const {
	return sio_GetWindowPos(_sioWindow_impl)[0];
}

int32_t sio::window::pos_y() const {
	return sio_GetWindowPos(_sioWindow_impl)[1];
}

void sio::window::pos_x(const int32_t x) {
	sio_MoveWindow(_sioWindow_impl, x, pos_y());
}

void sio::window::pos_y(const int32_t y) {
	sio_MoveWindow(_sioWindow_impl, pos_x(), y);
}

void sio::window::size(const int32_t w, const int32_t h) {
	sio_ResizeWindow(_sioWindow_impl, w, h);
}

void sio::window::pos(const int32_t x, const int32_t y) {
	sio_MoveWindow(_sioWindow_impl, x, y);
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
