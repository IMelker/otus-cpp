include_directories(${CMAKE_BINARY_DIR}/1_task)

project(helloworld VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})

configure_file(version.h.in "version.h" @ONLY)

add_executable(helloworld main.cpp)
add_library(libhelloworld lib.cpp)

target_link_libraries( helloworld
    libhelloworld
)

install(TARGETS helloworld RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT l2pickn@yandex.ru)

include(CPack)
