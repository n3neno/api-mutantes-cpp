cmake_minimum_required(VERSION 3.10)
project(SimianAPI)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


add_executable(api simian.cpp)


target_include_directories(api PRIVATE "${CMAKE_SOURCE_DIR}")
target_include_directories(api PRIVATE "${CMAKE_SOURCE_DIR}/asio_include")


find_package(Threads REQUIRED)
target_link_libraries(api Threads::Threads sqlite3)

if(WIN32)
    target_link_libraries(api ws2_32 mswsock)
endif()