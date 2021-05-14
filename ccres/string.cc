#include <ccres/string.h>

namespace ccres {

void String::read_stream(std::istream &ins) {
    std::string s = "";
    char c;
    while (ins.get(c)) {
        s += c;
    }

    assign(s.begin(), s.end());
}

};  // ccres
