#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <locale>
#include <ccres/cleaner.h>
#include <ccres/tokenizer.h>
#include <ccres/parser.h>
#include <ccres/renderer.h>
#include <ccres/opts.h>

namespace ccres {

class App {
private:
    int _argc;
    char **_argv;
    Options _opts;
    
public:
    ~App();
    App(int argc, char *argv[]);
    int run();

private:
    void _parse_opts(int argc, char *argv[]);
    void _show_usage() const;
};

}  // ccres
