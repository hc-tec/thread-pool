cmake_minimum_required(VERSION 3.17)
project(thread_pool C)

set(CMAKE_C_STANDARD 11)

add_executable(thread_pool test.c thread-pool.c thread-pool.h)

target_link_libraries(thread_pool pthread)