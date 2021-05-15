#include <ccres/string.h>

namespace ccres {

void String::read_stream(std::wistream &fin) {
    std::wstring s = L"";
    wchar_t c;
    while (fin.get(c)) {
        s += c;
    }

    assign(s.begin(), s.end());
}

};  // ccres
