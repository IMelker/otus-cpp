project(logger VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost COMPONENTS log REQUIRED)

add_library(logger logger.cpp)
target_link_libraries(logger ${Boost_LLIBRARIES})