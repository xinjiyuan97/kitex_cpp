cmake_minimum_required(VERSION 3.10)

project(kitex_cpp VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} /usr/local/lib/cmake /usr/include)

add_executable(kitex_cpp main.cpp src/transport.cpp)
find_path(GLOG_INCLUDE_DIRS NAMES glog/logging.h)
find_library(GLOG_LIBRARIES NAMES glog)


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${GLOG_INCLUDE_DIRS})

target_link_libraries(kitex_cpp ${GLOG_LIBRARIES})
# link local include
target_include_directories(kitex_cpp PUBLIC
                          ${PROJECT_BINARY_DIR}
                          ${PROJECT_SOURCE_DIR}/include)

add_subdirectory(src)
add_subdirectory(include)

# add_executable(kitex_cpp main.cpp)

# target_link_libraries(kitex_cpp PUBLIC kitex_cpp_lib)