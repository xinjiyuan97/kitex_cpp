cmake_minimum_required(VERSION 3.10)

project(kitex_cpp VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(src)
add_subdirectory(include)

# add_executable(kitex_cpp main.cpp)

# target_link_libraries(kitex_cpp PUBLIC kitex_cpp_lib)