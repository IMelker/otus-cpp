add_executable(test_print_ip "test_print_ip.cpp")
set_target_properties(test_print_ip PROPERTIES
        COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
        )

target_link_libraries(test_print_ip
        ${Boost_LIBRARIES}
        )

add_test(test_print_ip_valid test_print_ip)
