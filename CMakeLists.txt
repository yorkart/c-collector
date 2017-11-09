cmake_minimum_required(VERSION 3.8)
project(c-collector)

set(CMAKE_C_STANDARD 11)

set(SOURCE_FILES main.c src/net/frame.h src/net/frame.c src/utils/list.h src/net/buffer.h src/net/buffer.c src/net/sock.h src/net/sock.c src/utils/counter.h src/utils/bytes.h test/net/sock_test.h test/net/buffer_test.h test/net/frame_test.h src/utils/tools.h src/net/sock_frame.h src/net/sock_frame.c)
add_executable(c-collector ${SOURCE_FILES})

include_directories(C:/PluginPackage/libuv/include)
target_link_libraries(c-collector C:/PluginPackage/libuv/libuv.dll)