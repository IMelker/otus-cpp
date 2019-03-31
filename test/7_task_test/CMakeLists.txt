add_executable(test_nickname "test_nickname.cpp" "../../7_task/radix_tree.h")
set_target_properties(test_nickname PROPERTIES
        COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
        )

target_link_libraries(test_nickname
        ${Boost_LIBRARIES}
        )

add_test(test_nickname_valid test_nickname)
