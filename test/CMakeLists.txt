project(test)

include_directories(${Boost_INCLUDE_DIR} ${CMAKE_SOURCE_DIR})
link_directories("${CMAKE_BINARY_DIR}/lib")

find_package(Boost COMPONENTS unit_test_framework REQUIRED)

add_subdirectory(1_task_test)
add_subdirectory(2_task_test)
add_subdirectory(3_task_test)
add_subdirectory(4_task_test)
add_subdirectory(5_task_test)
add_subdirectory(6_task_test)
add_subdirectory(7_task_test)