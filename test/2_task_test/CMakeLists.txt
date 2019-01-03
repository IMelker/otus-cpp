add_executable(test_ip_filter "test_ip_filter.cpp")
set_target_properties(test_ip_filter PROPERTIES
        COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
        )

target_link_libraries(test_ip_filter
        ${Boost_LIBRARIES}
        )

add_test(test_ip_filter_valid test_ip_filter)
