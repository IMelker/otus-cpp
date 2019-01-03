add_executable(test_version "test_version.cpp")
set_target_properties(test_version PROPERTIES
        COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
        )

target_link_libraries(test_version
        ${Boost_LIBRARIES}
        libhelloworld
        )

add_test(test_version_valid test_version)
