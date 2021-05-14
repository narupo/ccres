#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <ccres/tokenizer.h>
#include <ccres/parser.h>

namespace ccres {

class App {
private:
    std::vector<std::string> _args;
    Tokenizer _tokenizer;
    Parser _parser;

public:
    ~App();
    App(int argc, char *argv[]);
    int run();

private:
    std::vector<std::string> _argv_to_args(char *argv[]);
};

}  // ccres
