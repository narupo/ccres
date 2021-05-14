#pragma once
#include <string>
#include <istream>

namespace ccres {

class String : public std::wstring {
public:
    void read_stream(std::istream &ins);
};

};  // namespace
