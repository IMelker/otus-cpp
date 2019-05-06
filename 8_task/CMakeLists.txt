project(bayan VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})

find_package(Boost COMPONENTS thread log program_options REQUIRED)
add_definitions(-DBOOST_LOG_DYN_LINK)
add_definitions(-DVERSION=\"${PROJECT_VERSION}\")

include_directories("${CMAKE_SOURCE_DIR}/common")
link_directories("${CMAKE_BINARY_DIR}/lib")

add_executable(bayan main.cpp scanner.cpp)
target_link_libraries(bayan
                      arg_parser
                      logger
                      ${Boost_LIBRARIES}
                      )

install(TARGETS bayan RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT l2pickn@yandex.ru)

include(CPack)
