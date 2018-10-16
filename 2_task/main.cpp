//#include <cassert>
//#include <cstdlib>
#include <iostream>
#include <string>
#include <set>

typedef std::multiset<unsigned int, std::greater<unsigned int>> uimset_g;

struct IpFilter
{
    enum class Type: int { Simple, OR, XOR, NotValid };

    IpFilter() : n1(-1), n2(-1), n3(-1), n4(-1), type(Type::NotValid) {};
    IpFilter(int n1_=-1, int n2_=-1, int n3_=-1, int n4_=-1) :
        n1(n1_), n2(n2_), n3(n3_), n4(n4_), type(Type::Simple) {};
    explicit IpFilter(std::pair<int, IpFilter::Type>&& typed_filter)
    {
        type = typed_filter.second;
        n1 = typed_filter.first;
        n2 = typed_filter.first;
        n3 = typed_filter.first;
        n4 = typed_filter.first;
    };
    bool IpFits(int n1_, int n2_, int n3_, int n4_)
    {
        switch(type) {
        case Type::Simple:
            return (n1 == -1 || n1 == n1_) &&
                   (n2 == -1 || n2 == n2_) &&
                   (n3 == -1 || n3 == n3_) &&
                   (n4 == -1 || n4 == n4_);
        case Type::OR:
            return (n1 == n1_) || (n2 == n2_) || (n3 == n3_) || (n4 == n4_);
        case Type::XOR:
            return (n1 != n1_) && (n2 != n2_) && (n3 != n3_) && (n4 != n4_);
        case Type::NotValid:
            break;
        }
        return false;
    }
    int n1;
    int n2;
    int n3;
    int n4;
    Type type;
};

static const auto _____ = []()
{
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	return nullptr;
}();

auto DotNotationToUInt (const char * ip) {
    unsigned v = 0;
    const char * start = ip;
    for (int i = 0; i < 4; i++) {
        char c;
        int n = 0;
        while ( true ) {
            c = * start;
            start++;
            if (c >= '0' && c <= '9') {
                n *= 10;
                n += c - '0';
            }
            else if ((i < 3 && c == '.') || i == 3) {
                break;
            }
            else {
                return 0u;
            }
        }
        if (n >= 256) {
            return 0u;
        }
        v *= 256;
        v += n;
    }
    return v;
}

void FillIpPool(uimset_g& pool)
{
    std::string ip;
    while (getline(std::cin, ip)) {
        const auto& v = DotNotationToUInt(ip.data());
        if (v > 0) pool.insert(v);
    }
}

void PrintIpPool(const uimset_g& pool)
{
    for(const auto& ip : pool)
    {
        std::cout << ((ip >> 24) & 0xFF) << '.'
                  << ((ip >> 16) & 0xFF) << '.'
                  << ((ip >> 8) & 0xFF) << '.'
                  << (ip & 0xFF) << std::endl;
    }
}
void PrintIpPool(const uimset_g& pool, IpFilter&& filter)
{
    for(const auto& ip : pool)
    {
        int n1 = (ip >> 24) & 0xFF;
        int n2 = (ip >> 16) & 0xFF;
        int n3 = (ip >> 8) & 0xFF;
        int n4 = ip & 0xFF;
        if (filter.IpFits(n1,n2,n3,n4))
        {
            std::cout << n1 << '.' << n2 << '.' << n3 << '.' << n4 <<  std::endl;
        }
    }
}

auto operator"" _any(unsigned long long n) {
    return std::make_pair(n, IpFilter::Type::OR);
};
auto operator"" _noone(unsigned long long n) {
    return std::make_pair(n, IpFilter::Type::XOR);
};

int main(int argc, char const *argv[])
{
    uimset_g ip_pool;
    FillIpPool(ip_pool);
    PrintIpPool(ip_pool);
    PrintIpPool(ip_pool, IpFilter(1));
    PrintIpPool(ip_pool, IpFilter(46, 70));
    PrintIpPool(ip_pool, IpFilter(46_any));
    return 0;
}
