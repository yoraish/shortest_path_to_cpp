# CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(shortest_path_to_cpp)

set(CMAKE_CXX_STANDARD 17)

include_directories(include/shortest_path_to_cpp)

add_executable(planner src/main.cpp src/planner.cpp)