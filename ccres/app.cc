#include <ccres/app.h>

namespace ccres {

App::~App() {
}

App::App(int argc, char *argv[])
    : _args()
    , _tokenizer() {
    _args = _argv_to_args(argv);
}

std::vector<std::string> App::_argv_to_args(char *argv[]) {
    std::vector<std::string> args {};
    for (char **p = argv; *p; p++) {
        args.emplace_back(std::string(*p));
    }
    return args;
}

int App::run() {
    auto text = String();
    text.read_stream(std::cin);
    auto tokens = _tokenizer.tokenize(text);
    for (const auto &tok : tokens) {
        std::cout << tok.type << ": ";
        std::wcout << tok.text << std::endl;
    }
    return 0;
}

}  // ccres