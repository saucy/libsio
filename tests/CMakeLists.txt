project(tests)
cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(../include/)
link_directories(${PROJECT_BINARY_DIR})

add_executable(event_thread event_thread.cpp)
target_link_libraries(event_thread sio)

add_executable(window window.cpp)
target_link_libraries(window sio opengl32)

add_executable(chat_client chat_client.cpp)
target_link_libraries(chat_client sio)

add_executable(chat_server chat_server.cpp)
target_link_libraries(chat_server sio)
