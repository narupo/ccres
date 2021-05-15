#include <ccres/ctype.h>

namespace ccres {

bool isalpha(wchar_t c) {
    return std::isalpha(c);
}

bool isdigit(wchar_t c) {
    return std::isdigit(c);
}

bool isdot(wchar_t c) {
    return c == L'.';
}

bool isspace(wchar_t c) {
    return c == L' ' || c == L'　' || c == L'\t';
}

bool iscolon(wchar_t c) {
    return c == L':' || c == L'：';
}

bool isdatesep(wchar_t c) {
    return c == L'-' || c == L'ー' ||
           c == L'/' || c == L'／' ||
           c == L'_' || c == L'＿' ||
           c == L'年' || c == L'月' || c == L'日';
}

bool isyoubileft(wchar_t c) {
    return c == L'[' || c == L'「' ||
           c == L'(' || c == L'（' ||
           c == L'<' || c == L'＜' ||
           c == L'{' || c == L'｛' ||
           c == L'【'; 
}

bool isyoubiright(wchar_t c) {
    return c == L']' || c == L'」' ||
           c == L')' || c == L'）' ||
           c == L'>' || c == L'＞' ||
           c == L'}' || c == L'｝' ||
           c == L'】';
}

};  // ccres
