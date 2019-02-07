add_executable(test_bulk "test_bulk.cpp" "../../6_task/bulk_composer.cpp" "../../6_task/executor.cpp")
set_target_properties(test_bulk PROPERTIES
        COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
        )

target_link_libraries(test_bulk
        ${Boost_LIBRARIES}
        )

add_test(test_bulk_valid test_bulk)
