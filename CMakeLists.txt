cmake_minimum_required(VERSION 3.8)
project(c-collector)

set(CMAKE_C_STANDARD 11)

set(SOURCE_FILES main.c net/frame.h net/frame.c utils/list.h net/buffer.h net/buffer.c net/sock.h net/sock.c utils/counter.h utils/bytes.h)
add_executable(c-collector ${SOURCE_FILES})

include_directories(C:/PluginPackage/libuv/include)
target_link_libraries(c-collector C:/PluginPackage/libuv/libuv.dll)