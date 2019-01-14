//
// Created by imelker on 1/14/19.
//

#include <vector>
#include <list>
#include <tuple>
#include "print_ip.h"

int main () {
  PrintIp(char(-1));
  PrintIp(short(0));
  PrintIp(int(2130706433));
  PrintIp(long(8875824491850138409));
  PrintIp(std::string("8.8.8.8"));
  PrintIp(std::vector<int>{255, 255, 255, 255});
  PrintIp(std::list<std::string>{"255", "255", "255", "255"});
  PrintIp(std::make_tuple(127, 0, 0, 1));
  PrintIp(std::make_tuple("127", "0", "0", "1"));
  //PrintIp(std::make_tuple(127, 0, 0, "1"));
  return 0;
}