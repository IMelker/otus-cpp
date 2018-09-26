cmake_minimum_required(VERSION 3.2)

project(first_task VERSION 0.0.0)

set(PROJECT_VERSION_PATCH 1)
configure_file(version.h.in "../version.h" @ONLY)

add_executable(helloworld main.cpp)

add_library(libhelloworld lib.cpp)


set_target_properties( helloworld libhelloworld PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    COMPILE_OPTIONS "-Wpedantic;-Wall;-Wextra"
)

target_link_libraries( helloworld
    libhelloworld
)
