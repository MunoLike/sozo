/*
 * strutil.cpp
 *
 *  Created on: 2020/12/10
 *      Author: MunoLike
 */
#include <vector>
#include <string>
#include <cstdio>
#include <iostream>

namespace utils{
std::vector<double> splitDouble(std::string str, char sep){
    auto offset = std::string::size_type(0);
    auto separator_length = std::string::size_type(1);
    auto list = std::vector<double>();
    while (1) {
      auto pos = str.find(sep, offset);
      if (pos == std::string::npos) {
        list.push_back(std::stod(str.substr(offset)));
        break;
      }
      list.push_back(std::stod(str.substr(offset, pos - offset)));
      offset = pos + separator_length;
    }

    return list;
}
}




