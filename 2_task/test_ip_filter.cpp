#define BOOST_TEST_MODULE ip_filter_test_module

#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>

#include "ip_filter.h"

BOOST_AUTO_TEST_SUITE(ip_filter_test_suite)

std::vector<std::vector<int>> test_data = {
    {1,231,69,33},
    {10,87,203,225},
    {225,70,44,170},
    {49,44,168,152},
    {1,1,234,8}
};

struct cout_redirect {
    cout_redirect( std::streambuf * new_buffer )
        : old( std::cout.rdbuf( new_buffer ) )
    { }

    ~cout_redirect( ) {
        std::cout.rdbuf( old );
    }

  private:
    std::streambuf * old;
};

BOOST_AUTO_TEST_CASE(ip_filter_test_out)
{
    boost::test_tools::output_test_stream output;
    {
        cout_redirect guard( output.rdbuf( ) );
        PrintIpPool(test_data);
    }

    BOOST_CHECK( output.is_equal(
        "1.231.69.33\n"
        "10.87.203.225\n"
        "225.70.44.170\n"
        "49.44.168.152\n"
        "1.1.234.8\n"
     ) );
}

BOOST_AUTO_TEST_CASE(ip_filter_test_sort)
{
    std::vector<std::vector<int>> unsorted_data = test_data;
    std::vector<std::vector<int>> sorted_data = {
        {225,70,44,170},
        {49,44,168,152},
        {10,87,203,225},
        {1,231,69,33},
        {1,1,234,8}
    };
    SortIpPool(unsorted_data);
    BOOST_CHECK( unsorted_data == sorted_data );
}

BOOST_AUTO_TEST_CASE(ip_filter_test_filter)
{
    boost::test_tools::output_test_stream output;
    {
        cout_redirect guard( output.rdbuf( ) );
        PrintIpPool(test_data, IpFilter(1));
        PrintIpPool(test_data, IpFilter(225, 70));
        PrintIpPool(test_data, IpFilter(44_any));
        PrintIpPool(test_data, IpFilter(225_noone));
    }
    BOOST_CHECK( output.is_equal(
        "1.231.69.33\n"
        "1.1.234.8\n"
        "225.70.44.170\n"
        "225.70.44.170\n"
        "49.44.168.152\n"
        "1.231.69.33\n"
        "49.44.168.152\n"
        "1.1.234.8\n"
     ) );
}

BOOST_AUTO_TEST_SUITE_END()
