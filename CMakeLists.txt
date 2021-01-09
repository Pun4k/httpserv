cmake_minimum_required(VERSION 3.8..3.17)

set(PROJECT_NAME HTTPSERV LANGUAGES C)

project(httpserv)

set(CMAKE_C_STANDARD 11)



set(HEADER_FILES src/net/net.h src/http/http.h
                src/test/test.h
                src/DataTypes/binary_tree/binarytree.h
                src/DataTypes/hashtable/hashtab.h
)
set(SOURCE_FILES httpserv.c src/net/net.c
                src/test/test.c
                src/DataTypes/binary_tree/binarytree.c
                src/DataTypes/hashtable/hashtab.c
)

add_executable(httpserv ${HEADER_FILES} ${SOURCE_FILES})

if(WIN32)
    target_link_libraries(${PROJECT_NAME} wsock32 ws2_32)
endif()