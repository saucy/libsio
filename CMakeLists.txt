project(libsio)
cmake_minimum_required(VERSION 2.8)

include(ExternalProject)

# Default to SDL2 because Native API is incomplete/broken
option(USE_SDL2 "Use SDL2 instead of native libraries." ON)
option(USE_NATIVE "Use native libraries." OFF)

if(USE_SDL2)
	set(SDL2_SOURCE_DIR "${CMAKE_BINARY_DIR}/SDL2-src")
	set(SDL2_BINARY_DIR "${CMAKE_BINARY_DIR}/SDL2-bin")
	set(SDL2_INSTALL_DIR "${CMAKE_BINARY_DIR}/SDL2/")
	set(SDL2_CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/SDL2 "-DDIRECTX=OFF")
	
	if(WIN32)
		set(SDL2_TEST_COMMAND
			${CMAKE_COMMAND} -E copy ${SDL2_BINARY_DIR}/libSDL2.dll ${SDL2_BINARY_DIR}/libSDL2.so
		)
	else()
		set(SDL2_TEST_COMMAND "")
	endif()
	
	ExternalProject_Add(ExternProject_SDL2
		URL "http://www.libsdl.org/release/SDL2-2.0.1.tar.gz"
		URL_MD5 "0eb97039488bf463e775295f7b18b227"
		
		SOURCE_DIR   ${SDL2_SOURCE_DIR}
		BINARY_DIR   ${SDL2_BINARY_DIR}
		INSTALL_DIR  ${SDL2_INSTALL_DIR}
		CMAKE_ARGS   ${SDL2_CMAKE_ARGS}
		
		TEST_BEFORE_INSTALL 1
		TEST_COMMAND ${SDL2_TEST_COMMAND}
	)
	
	add_custom_command(TARGET ExternProject_SDL2
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${SDL2_SOURCE_DIR}/include ${CMAKE_BINARY_DIR}/include
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${SDL2_BINARY_DIR}/include ${CMAKE_BINARY_DIR}/include
	)
	
	link_directories(${SDL2_BINARY_DIR})
	include_directories(${SDL2_INSTALL_DIR}/include)
elseif(USE_NATIVE)
	message(FATAL_ERROR "USE_NATIVE is currently imcomplete. Enable USE_SDL2 to continue.")
else()
	message(FATAL_ERROR "USE_NATIVE and USE_SDL2 options are both disabled. Cannot continue.")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(${CMAKE_BINARY_DIR}/include)

if(CMAKE_BUILD_TYPE)
	string(TOUPPER ${CMAKE_BUILD_TYPE} CAPITALIZED_CMAKE_BUILD_TYPE)
	if(${CAPITALIZED_CMAKE_BUILD_TYPE} STREQUAL "DEBUG")
		add_definitions(-DDEBUG)
	endif()
endif()

## Platform stuff
set(SHARED_LIB_EXT "dll")
set(STATIC_LIB_EXT "a")

###########################################################
## libsio Event Thread

## Header Files
set(SIO_EVENT_THREAD_HEADER_FILES
	"include/sio/event_thread.h"
	"src/event_thread/event_thread.h"
)

## Source Files
set(SIO_EVENT_THREAD_SOURCE_FILES
	"src/event_thread/event_thread.cpp"
	"src/event_thread/event_threadf.cpp"
)

add_library(sio_EventThread STATIC
	${SIO_EVENT_THREAD_SOURCE_FILES} ${SIO_EVENT_THREAD_HEADER_FILES}
)

###########################################################
## libsio OpenGL

if(USE_SDL2)
	set(SIO_OPENGL_HEADER_FILES "")
	set(SIO_OPENGL_SOURCE_FILES
		"src/opengl/sdl2/opengl.cpp" "src/opengl/sdl2/opengl.h"
	)
	
	set(SIO_OPENGL_LIBS "")
elseif(USE_NATIVE)
	set(SIO_OPENGL_HEADER_FILES "")
	set(SIO_OPENGL_SOURCE_FILES
		"src/opengl/win32/opengl.cpp" "src/opengl/win32/opengl.h"
	)
	set(SIO_OPENGL_LIBS
		opengl32
	)
endif()

set(SIO_OPENGL_HEADER_FILES
	"include/sio/opengl.h"
	${SIO_OPENGL_HEADER_FILES}
)

set(SIO_OPENGL_SOURCE_FILES
	"src/opengl/opengl.h"
	${SIO_OPENGL_SOURCE_FILES}
)

set(SIO_OPENGL_LIBS
	
	${SIO_OPENGL_LIBS}
)

###########################################################
## libsio Window

if(USE_SDL2)
	set(SIO_WINDOW_HEADER_FILES)
	set(SIO_WINDOW_SOURCE_FILES
		"src/window/sdl2/window.cpp" "src/window/sdl2/window.h"
	)
	
	set(SIO_WINDOW_LIBS
		"SDL2"
	)
elseif(USE_NATIVE)
	set(SIO_WINDOW_HEADER_FILES)
	set(SIO_WINDOW_SOURCE_FILES
		"src/window/win32/window.cpp" "src/window/win32/window.h"
	)
	
	set(SIO_WINDOW_LIBS
		"gdi32"
	)
endif()

set(SIO_WINDOW_HEADER_FILES
	"include/sio/window.h"
	"include/sio/window/scancode.h"
	"include/sio/window/scancode.inl"
	${SIO_WINDOW_HEADER_FILES}
)

set(SIO_WINDOW_SOURCE_FILES
	"src/window/window.cpp" "src/window/window.h"
	${SIO_WINDOW_SOURCE_FILES}
)

add_library(sio_Window STATIC
	${SIO_WINDOW_SOURCE_FILES} ${SIO_WINDOW_HEADER_FILES}
)

target_link_libraries(sio_Window ${SIO_WINDOW_LIBS})

###########################################################
## libsio Socket

if(WIN32)
	set(SIO_SOCKET_HEADER_FILES
		"include/sio/socket.h"
	)
	
	set(SIO_SOCKET_SOURCE_FILES
		"src/socket/socket.h"
		"src/socket/socket.cpp"
		"src/socket/win32/socket.h"
		"src/socket/win32/socket.cpp"
	)
	
	set(SIO_SOCKET_LIBS
		"ws2_32"
	)
	
	add_library(sio_Socket STATIC
		${SIO_SOCKET_SOURCE_FILES} ${SIO_SOCKET_HEADER_FILES}
	)
	
else()
	message(WARNING "libsio_Socket not supported on non-windows platforms at this time.")
	set(SIO_SOCKET_HEADER_FILES "")
	set(SIO_SOCKET_SOURCE_FILES "")
	set(SIO_SOCKET_LIBS "")
endif()

###########################################################
## libsio All

set(SIO_HEADER_FILES
	${SIO_EVENT_THREAD_HEADER_FILES}
	${SIO_OPENGL_HEADER_FILES}
	${SIO_WINDOW_HEADER_FILES}
	${SIO_SOCKET_HEADER_FILES}
)

set(SIO_SOURCE_FILES
	${SIO_EVENT_THREAD_SOURCE_FILES}
	${SIO_OPENGL_SOURCE_FILES}
	${SIO_WINDOW_SOURCE_FILES}
	${SIO_SOCKET_SOURCE_FILES}
)

set(SIO_LIBS
	${SIO_OPENGL_LIBS}
	${SIO_WINDOW_LIBS}
	${SIO_SOCKET_LIBS}
)

add_library(sio SHARED ${SIO_SOURCE_FILES} ${SIO_HEADER_FILES})

target_link_libraries(sio ${SIO_LIBS})

add_library(sioSTATIC STATIC ${SIO_SOURCE_FILES} ${SIO_HEADER_FILES})

target_link_libraries(sioSTATIC ${SIO_LIBS})

set_target_properties(sioSTATIC
	PROPERTIES OUTPUT_NAME "sio"
)

add_subdirectory(tests)

install(TARGETS sio DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
install(TARGETS sioSTATIC DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include DESTINATION ${CMAKE_INSTALL_PREFIX})
