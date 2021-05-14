#pragma once
#include <iostream>
#include <vector>
#include <ccres/token.h>
#include <ccres/string.h>
#include <ccres/ctype.h>

namespace ccres {

class Tokenizer {
private:
    std::vector<ccres::Token> _tokens;
    String::const_iterator _p;
    String::const_iterator _end;

public:
    ~Tokenizer();
    Tokenizer();
    const std::vector<ccres::Token> &tokenize(const ccres::String &text);

private:
    void _read_digits();
    void _read_spaces();
};

}  // ccres
