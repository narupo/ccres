#pragma once
#include <iostream>
#include <ccres/string.h>
#include <ccres/ctype.h>

namespace ccres {

class Cleaner {
public:
    String clean(const String &text);

private:
    String _clean_tags(const String &text);
    String _clean_newlines(const String &text);
    String _clean_spaces(const String &text);
};

};  // ccres
