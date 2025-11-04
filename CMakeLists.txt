cmake_minimum_required(VERSION 4.1)
project(File_Indexer)

set(CMAKE_CXX_STANDARD 20)

find_package(Boost REQUIRED)

add_executable(File_Indexer main.cpp)
target_link_libraries(File_Indexer Boost::headers)
