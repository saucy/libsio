#ifndef SIO_SRC_WINDOW_HEADER
#define SIO_SRC_WINDOW_HEADER

#include <array>

#include "../../include/sio/window.h"

// For future linkage use
#define SIO_SRC_API

typedef _sioWindowImpl* SIO_Window;

/**
 * @brief Creates window impl in window constructor (see ./window.cpp)
 */
SIO_SRC_API SIO_Window sio_CreateWindow(sio::window*);
SIO_SRC_API void sio_DestroyWindow(SIO_Window);

SIO_SRC_API void sio_ShowWindow(SIO_Window);
SIO_SRC_API void sio_HideWindow(SIO_Window);

SIO_SRC_API void sio_SetWindowTitle(SIO_Window, std::string);
SIO_SRC_API std::string sio_GetWindowTitle(SIO_Window);

SIO_SRC_API void sio_MoveWindow(SIO_Window, int32_t x, int32_t y);
SIO_SRC_API void sio_ResizeWindow(SIO_Window, int32_t x, int32_t y);
SIO_SRC_API void sio_GetWindowSize(SIO_Window, int32_t*&& width, int32_t*&& height);

SIO_SRC_API std::array<int32_t, 2> sio_GetWindowPos(SIO_Window);

SIO_SRC_API void sio_SetWindowFocus(SIO_Window, bool);


#undef SIO_SRC_API
#endif//SIO_SRC_WINDOW_HEADER
