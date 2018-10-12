#include <iostream>
#include <string>
#include <set>

// hack to turn off syncro with stdio
static const auto _____ = []()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    return nullptr;
}();

auto ip_to_int (const char * ip) {
    unsigned v = 0;
    const char * start = ip;
    for (int i = 0; i < 4; i++) {
        char c;
        int n = 0;
        while (true) {
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

void print_ip(unsigned int ip) {
    std::cout << ((ip >> 24) & 0xFF) << '.'
              << ((ip >> 16) & 0xFF) << '.'
              << ((ip >> 8) & 0xFF) << '.'
              << (ip & 0xFF) << '\n';
}

int main () {
    std::multiset<unsigned int> ip_list;
    std::string ip;
    while (getline(std::cin, ip)) {
        auto v = ip_to_int(ip.data());
        if (v > 0) ip_list.insert(v);
    }
    for (auto i = ip_list.end(); i != ip_list.begin(); --i) {
        print_ip(*i);
    }
    return 0;
}
