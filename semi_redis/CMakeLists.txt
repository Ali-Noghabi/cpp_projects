cmake_minimum_required(VERSION 3.16)
project(semi_redis)

set(CMAKE_CXX_STANDARD 14)

# Add pthread library
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(semi_redis server.cpp)
target_link_libraries(semi_redis PRIVATE Threads::Threads)

add_executable(client client.cpp)
