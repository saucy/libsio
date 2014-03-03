#ifndef SIO_SRC_WINDOW_WINDOW_HEADER
#define SIO_SRC_WINDOW_WINDOW_HEADER

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#undef KMOD_ALT
#undef KMOD_CTRL
#undef KMOD_SHIFT
#undef KMOD_GUI

#include "../window.h"

class _sioWindowImpl {
public:
	SDL_Window* window;
};

#endif//SIO_SRC_WINDOW_WINDOW_HEADER
