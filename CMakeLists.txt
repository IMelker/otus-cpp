cmake_minimum_required(VERSION 3.7)
project(otus-cpp)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()
message("CMake build type = ${CMAKE_BUILD_TYPE}")

set(CMAKE_CXX_FLAGS "-Wpedantic -Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -DDEBUG_LOG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

set(CMAKE_CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

#if (NOT ENV{TRAVIS_BUILD_NUMBER})
#    set(ENV{TRAVIS_BUILD_NUMBER} 1)
#endif()

add_subdirectory(1_task)
add_subdirectory(2_task)
add_subdirectory(3_task)
add_subdirectory(4_task)
add_subdirectory(5_task)
add_subdirectory(6_task)
add_subdirectory(7_task)

enable_testing()
add_subdirectory(test)
