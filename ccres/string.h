#pragma once
#include <string>
#include <istream>
#include <iostream>

namespace ccres {

class String : public std::wstring {
public:
    void read_stream(std::wistream &ins);
};

};  // namespace
