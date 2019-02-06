add_executable(test_matrix "test_matrix.cpp")
set_target_properties(test_matrix PROPERTIES
        COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
        )

target_link_libraries(test_matrix
        ${Boost_LIBRARIES}
        )

add_test(test_matrix_valid test_matrix)
