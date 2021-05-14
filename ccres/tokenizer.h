#pragma once
#include <vector>
#include <ccres/token.h>
#include <ccres/string.h>

namespace ccres {

class Tokenizer {
public:
    ~Tokenizer();
    Tokenizer();
    std::vector<ccres::Token> tokenize(const ccres::String &text);
};

}  // ccres
