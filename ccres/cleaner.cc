#include <ccres/cleaner.h>

namespace ccres {

String Cleaner::clean(const String &text) {
    String s = _clean_tags(text);
    s = _clean_spaces(s);
    s = _clean_newlines(s);
    return s;
}

String Cleaner::_clean_tags(const String &text) {
    auto s = String();
    int m = 0;

    for (auto c : text) {
        switch (m) {
        case 0:
            if (c == '<') {
                m = 10;
            } else {
                s += c;
            }
            break;
        case 10:
            if (c == '>') {
                m = 0;
            }
            break;
        }
    }

    return s;
}

String Cleaner::_clean_spaces(const String &text) {
    auto s = String();
    int m = 0;

    for (auto c : text) {
        switch (m) {
        case 0:
            if (isspace(c)) {
                s += c;
                m = 10;
            } else {
                s += c;
            }
            break;
        case 10:  // found space
            if (isspace(c)) {
                // pass
            } else {
                s += c;
                m = 0;
            }
            break;
        }
    }

    return s;
}

String Cleaner::_clean_newlines(const String &text) {
    auto s = String();
    auto p = text.cbegin();
    auto end = text.cend();
    int m = 0;

    for (; p != end; ) {
        auto c1 = *p++;
        auto c2 = *p;
        switch (m) {
        case 0:
            if (c1 == '\r' && c2 == '\n') {
                m = 10;
                p++;
            } else if (c1 == '\r' || c1 == '\n') {
                m = 10;
            } else {
                s += c1;
            }
            break;
        case 10:
            if (c1 == '\r' && c2 == '\n') {
                p++;
            } else if (c1 == '\r' || c1 == '\n') {
                // pass
            } else {
                s += '\n';
                s += c1;
                m = 0;
            }
            break;
        }
    }

    if (m == 0) {
        s += '\n';
    }

    return s;
}

}  // ccres
