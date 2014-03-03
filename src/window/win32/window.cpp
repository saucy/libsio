#include <iostream>

#include "window.h"

namespace {
	
	void windowInit() {
		MSG msg;
		
		while(GetMessage(&msg, NULL, 0, 0)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	
	std::thread windowInitThread(&windowInit);
	
	WNDCLASS wndclass;
	
	struct init {
		init();
		~init();
	} init;
	
}

static LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM);

init::init() {
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = GetModuleHandle(NULL);
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = TEXT("sio::window");
}

init::~init() {
	
}

LRESULT CALLBACK WndProc(
  _In_  HWND hwnd,
  _In_  UINT msg,
  _In_  WPARAM wparam,
  _In_  LPARAM lparam
) {
	
	switch(msg) {
	case SIO_WM_CREATE_WINDOW:
		std::cout << "Hello, create window!\n";
		((_sioWindowImpl*)wparam)->hwnd = CreateWindow("sio::window", nullptr,
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT,
			CW_USEDEFAULT, CW_USEDEFAULT,
			NULL,
			NULL,
			GetModuleHandle(NULL),
			NULL
		);
		return 0;
	case WM_CREATE:
		return 0;
	case WM_DESTROY:
		return 0;
	}
	
	return DefWindowProc (hwnd, msg, wparam, lparam) ;
}

SIO_Window sio_CreateWindow() {
	_sioWindowImpl* impl = new _sioWindowImpl;
	
	CallWindowProc(&WndProc, NULL, SIO_WM_CREATE_WINDOW, (WPARAM)impl, 0);
	
	
	return impl;
}

void sio_DestroyWindow(SIO_Window w) {
	DestroyWindow(w->hwnd);
}

void sio_ShowWindow(SIO_Window w) {
	ShowWindow(w->hwnd, SW_SHOW);
	UpdateWindow(w->hwnd);
}

void sio_HideWindow(SIO_Window w) {
	ShowWindow(w->hwnd, SW_HIDE);
	UpdateWindow(w->hwnd);
}

void sio_SetWindowTitle(SIO_Window w, std::string str) {
	SetWindowText(w->hwnd, str.c_str());
}

std::string sio_GetWindowTitle(SIO_Window w) {
	int sz = GetWindowTextLength(w->hwnd);
	char titleStr[sz];
	
	GetWindowText(w->hwnd, titleStr, sz);
	
	return std::string(titleStr, sz);
}

void sio_MoveWindow(SIO_Window w, int32_t x, int32_t y) {
	RECT rect;
	GetClientRect(w->hwnd, &rect);
	
	MoveWindow(w->hwnd, x, y, rect.right - rect.left, rect.bottom - rect.top, FALSE);
}

void sio_ResizeWindow(SIO_Window w, int32_t width, int32_t height) {
	RECT rect;
	GetWindowRect(w->hwnd, &rect);
	MapWindowPoints(HWND_DESKTOP, GetParent(w->hwnd), (LPPOINT) &rect, 2);
	
	MoveWindow(w->hwnd, ((PPOINT)&rect)->x, ((PPOINT)&rect)->y, width, height, TRUE);
}

std::array<int32_t, 2> sio_GetWindowPos(SIO_Window w) {
	RECT rect;
	GetWindowRect(w->hwnd, &rect);
	MapWindowPoints(HWND_DESKTOP, GetParent(w->hwnd), (LPPOINT) &rect, 2);
	
	return {((PPOINT)&rect)->x, ((PPOINT)&rect)->y};
}

void sio_SetWindowFocus(SIO_Window w, bool b) {
	SendMessage(w->hwnd, WM_SETFOCUS, 0, 0);
}
