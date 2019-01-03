#include "ip_filter.h"

int main()
{
    vvi ip_pool;
    ip_pool.reserve(1024);
    FillIpPool(ip_pool);
    SortIpPool(ip_pool);
    PrintIpPool(ip_pool);
    PrintIpPool(ip_pool, IpFilter(1));
    PrintIpPool(ip_pool, IpFilter(46, 70));
    PrintIpPool(ip_pool, IpFilter(46_any));
    return 0;
}
