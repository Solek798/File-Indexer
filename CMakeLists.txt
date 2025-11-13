cmake_minimum_required(VERSION 4.1)
project(File_Indexer)

set(CMAKE_CXX_STANDARD 20)

# TODO(FK): need to check if this works on Windows
find_package(Boost REQUIRED COMPONENTS program_options filesystem)

add_executable(File_Indexer main.cpp
        Parameters.cpp
        Parameters.h)

target_link_libraries(File_Indexer Boost::program_options Boost::filesystem)
