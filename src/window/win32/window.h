#ifndef SIO_SRC_WINDOW_WIN32_WINDOW_HEADER
#define SIO_SRC_WINDOW_WIN32_WINDOW_HEADER

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windef.h>

#include "../window.h"

#define SIO_WM_CREATE_WINDOW (WM_USER + 1)

class _sioWindowImpl {
public:
	HWND hwnd;
};

#endif//SIO_SRC_WINDOW_WIN32_WINDOW_HEADER
