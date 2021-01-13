cmake_minimum_required(VERSION 3.8..3.17)

set(PROJECT_NAME http_serv LANGUAGES C)

project(${PROJECT_NAME})

set(CMAKE_C_STANDARD 11)



set(HEADER_FILES httpserv.h
                src/net/net.h src/http/http.h
                src/test/test.h
                src/DataTypes/binary_tree/binarytree.h
                src/DataTypes/hashtable/hashtab.h
)
set(SOURCE_FILES httpserv.c src/net/net.c src/http/http.c
                src/test/test.c
                src/DataTypes/binary_tree/binarytree.c
                src/DataTypes/hashtable/hashtab.c
)

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

if(WIN32)
    target_link_libraries(${PROJECT_NAME} wsock32 ws2_32)
endif()