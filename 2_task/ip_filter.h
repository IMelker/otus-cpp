#ifndef __IP_FILTER_H__
#define __IP_FILTER_H__

#define IP4_OCTET_COUNT 4
#define as_uint8(X) static_cast<uint8_t>(X)

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

typedef std::vector<uint8_t> vi;
typedef std::vector<std::vector<uint8_t>> vvi;

struct IpFilter
{
    enum class Type: int { Simple, OR, XOR, ALL, NotValid };
    IpFilter() : type(Type::NotValid) {};
    IpFilter(int n1 = -1, int n2 = -1, int n3 = -1, int n4 = -1)
      : type(Type::Simple),
      data({{std::make_pair(as_uint8(n1), n1 != -1)},
            {std::make_pair(as_uint8(n2), n2 != -1)},
            {std::make_pair(as_uint8(n3), n3 != -1)},
            {std::make_pair(as_uint8(n4), n4 != -1)} }) {};

    explicit IpFilter(std::pair<uint8_t, IpFilter::Type>&& typed_filter) :
        type(typed_filter.second), data({ {typed_filter.first, true} }) {};

    bool IpFits(const vi& ip)
    {
        switch(type) {
        case Type::Simple:
        {
            for (int i = 0; i < IP4_OCTET_COUNT; ++i) {
                if (data.at(i).second && data.at(i).first != ip.at(i)) {
                    return false;
                }
            }
            return true;
        }
        case Type::OR:
            return std::any_of(ip.begin(), ip.end(), [&](uint8_t oct){ return oct == data.front().first; });
        case Type::XOR:
            return std::none_of(ip.begin(), ip.end(), [&](uint8_t oct){ return oct == data.front().first; });
        case Type::ALL:
            return std::all_of(ip.begin(), ip.end(), [&](uint8_t oct){ return oct == data.front().first; });
        case Type::NotValid:
            break;
        }
        return false;
    }

    Type type;
    std::vector< std::pair<uint8_t, bool> > data;
};

auto operator"" _any(unsigned long long n) {
    return std::make_pair(as_uint8(n), IpFilter::Type::OR);
}
auto operator"" _noone(unsigned long long n) {
    return std::make_pair(as_uint8(n), IpFilter::Type::XOR);
}
auto operator"" _all(unsigned long long n) {
    return std::make_pair(as_uint8(n), IpFilter::Type::ALL);
}

auto SplitForIntParts(const std::string&& ip){
    vi parts;

    auto start = 0;
    auto stop = ip.find_first_of('.');
    while(stop != std::string::npos)
    {
        parts.push_back( as_uint8( stoi( ip.substr(start, stop - start) ) ) );
        start = stop + 1;
        stop = ip.find_first_of('.', start);
    }
    parts.push_back( as_uint8( stoi( ip.substr(start) ) ) );

    return std::move(parts);
}

void PrintIp(const vi& ip) {
    for (auto it = ip.begin() ; it != ip.end(); ++it) {
        if (it != ip.cbegin()) std::cout << ".";
        std::cout << static_cast<int>(*it);
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
    std::sort(pool.begin(), pool.end(), std::greater<vi>());
    /*std::sort(pool.begin(), pool.end(),
        [] (const vi& a, const vi& b) {
            for (int i = 0; i < 4; ++i) {
                if (a.at(i) != b.at(i)) {
                    return a.at(i) > b.at(i);
                }
            }
            return true;
        }
    );*/
}

#endif
