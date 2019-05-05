project(arg_parser VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})

find_package(Boost COMPONENTS program_options REQUIRED)
SET(Boost_USE_STATIC_LIBS ON)

add_library(arg_parser arg_parser.cpp)
target_link_libraries(arg_parser ${Boost_LIBRARIES})