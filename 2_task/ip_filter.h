#ifndef __IP_FILTER_H__
#define __IP_FILTER_H__

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

typedef std::vector<int> vi;
typedef std::vector<std::vector<int>> vvi;

struct IpFilter
{
    enum class Type: int { Simple, OR, XOR, NotValid };
    IpFilter() : n1(-1), n2(-1), n3(-1), n4(-1), type(Type::NotValid) {};
    IpFilter(int n1_=-1, int n2_=-1, int n3_=-1, int n4_=-1) :
        n1(n1_), n2(n2_), n3(n3_), n4(n4_), type(Type::Simple) {};
    explicit IpFilter(std::pair<int, IpFilter::Type>&& typed_filter) :
        n1(typed_filter.first), n2(typed_filter.first), n3(typed_filter.first),
        n4(typed_filter.first), type(typed_filter.second) {};
    bool IpFits(const vi& ip)
    {
        switch(type) {
        case Type::Simple:
            return (n1 == -1 || n1 == ip.at(0)) &&
                   (n2 == -1 || n2 == ip.at(1)) &&
                   (n3 == -1 || n3 == ip.at(2)) &&
                   (n4 == -1 || n4 == ip.at(3));
        case Type::OR:
            return (n1 == ip.at(0)) || (n2 == ip.at(1)) ||
                   (n3 == ip.at(2)) || (n4 == ip.at(3));
        case Type::XOR:
            return (n1 != ip.at(0)) && (n2 != ip.at(1)) &&
                   (n3 != ip.at(2)) && (n4 != ip.at(3));
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

auto operator"" _any(unsigned long long n) {
    return std::make_pair(n, IpFilter::Type::OR);
}
auto operator"" _noone(unsigned long long n) {
    return std::make_pair(n, IpFilter::Type::XOR);
}

auto SplitForIntParts(const std::string&& ip){
    vi parts;

    auto start = 0;
    auto stop = ip.find_first_of('.');
    while(stop != std::string::npos)
    {
        parts.push_back(stoi(ip.substr(start, stop - start)));
        start = stop + 1;
        stop = ip.find_first_of('.', start);
    }
    parts.push_back(stoi(ip.substr(start)));

    return std::move(parts);
}


void PrintIp(const vi& ip) {
    for (auto it = ip.begin() ; it != ip.end(); ++it) {
        if (it != ip.cbegin()) std::cout << ".";
        std::cout << *it;
    }
    std::cout << std::endl;
}
void PrintIpPool(const vvi& pool)
{
    for(const auto& ip_v: pool) PrintIp(ip_v);
}
void PrintIpPool(const vvi& pool, IpFilter&& filter)
{
    for(const auto& ip_v: pool) {
        if (filter.IpFits(ip_v)) PrintIp(ip_v);
    }
}

void FillIpPool(vvi& pool)
{
    try
    {
        for(std::string line; std::getline(std::cin, line);)
        {
            line = line.substr(0, line.find_first_of('\t'));
            pool.emplace_back(SplitForIntParts(std::move(line)));
        }
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        exit(1);
    }
}

void SortIpPool(vvi& pool) {
    std::sort(pool.begin(), pool.end(),
        [] (const vi& a, const vi& b) {
            for (int i = 0; i < 4; ++i) {
                if (a.at(i) != b.at(i)) {
                    return a.at(i) > b.at(i);
                }
            }
            return true;
        }
    );
}

#endif
