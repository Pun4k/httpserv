cmake_minimum_required(VERSION 3.8..3.17)

set(PROJECT_NAME HTTPSERV LANGUAGES C)

project(${PROJECT_NAME})

set(CMAKE_C_STANDARD 11)

add_executable(httpserv main.c
        src/net/net.c src/net/net.h
        src/test/test.c
        )

if(WIN32)
    target_link_libraries(${PROJECT_NAME} wsock32 ws2_32)
endif()
