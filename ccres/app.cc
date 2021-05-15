#include <ccres/app.h>
#include <ccres/renderer.h>

namespace ccres {

App::~App() {
}

App::App(int argc, char *argv[])
    : _args()
    , _tokenizer()
    , _parser()
    , _renderer() {
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
    text.read_stream(std::wcin);
    auto tokens = _tokenizer.tokenize(text);
    auto responses = _parser.parse(tokens);
    // _renderer.render_responses(responses);
    _renderer.render_responses_as_json(responses);
    return 0;
}

}  // ccres
