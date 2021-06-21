cmake_minimum_required(VERSION 3.17)
project(ImageServer C)

set(CMAKE_C_STANDARD 11)

include_directories(cmake-build-debug/bin)

add_executable(ImageServer main.c)
target_link_libraries(ImageServer ulfius jansson orcania curl m)


add_executable(nodeServer ../nodeServer/mainNode.c)
target_link_libraries(nodeServer ulfius jansson orcania curl m)


add_executable(nodeBase ../nodeServer/nodeBase.c)
target_link_libraries(nodeBase ulfius jansson orcania curl m)