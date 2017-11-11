cmake_minimum_required(VERSION 3.8)
project(c-collector)

set(CMAKE_C_STANDARD 11)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

set(SOURCE_FILES
        main.c
        src/net/frame.h
        src/net/frame.c
        src/net/buffer.h
        src/net/buffer.c
        src/net/sock.h
        src/net/sock.c
        src/net/sock_frame.h
        src/net/sock_frame.c
        src/utils/counter.h
        src/utils/bytes.h
        src/utils/list.h
        src/utils/tools.h
        src/utils/lfqueue.h
        src/utils/lfqueue.c
        src/utils/cross-platform.h
        src/zlog/zlog.c
        src/zlog/zlog.h
        src/zlog/zlog-config.h
        test/utils/lfqueue_test.h
        test/net/sock_test.h
        test/net/buffer_test.h
        test/net/frame_test.h
        )
add_executable(c-collector ${SOURCE_FILES})

include_directories(C:/PluginPackage/libuv/include)
target_link_libraries(c-collector Threads::Threads C:/PluginPackage/libuv/libuv.dll)