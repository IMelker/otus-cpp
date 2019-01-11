add_executable(test_allocator "test_allocator.cpp")
set_target_properties(test_allocator PROPERTIES
        COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
        )

target_link_libraries(test_allocator
        ${Boost_LIBRARIES}
        )

add_test(test_allocator_valid test_allocator)
