#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include <ccres/token.h>
#include <ccres/string.h>
#include <ccres/ctype.h>

namespace ccres {

class Tokenizer {
private:
    std::vector<std::shared_ptr<Token>> _tokens;
    String::const_iterator _p;
    String::const_iterator _end;

public:
    ~Tokenizer();
    Tokenizer();
    const std::vector<std::shared_ptr<Token>> &tokenize(const String &text);

private:
    void _read_digits();
    void _read_spaces();
};

}  // ccres
