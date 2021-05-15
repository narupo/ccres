#include <ccres/app.h>
#include <ccres/renderer.h>
#include <ccres/cleaner.h>
#include <ccres/except.h>

namespace ccres {

App::~App() {}

App::App(int argc, char *argv[])
    : _argc(argc)
    , _argv(argv)
    , _opts({
        .json = false,
        .need_number = true,
        .need_name = true,
        .need_date = true,
        .need_youbi = false,
        .need_time = true,
        .need_id = true,
    })
    {}

void App::_parse_opts(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        std::string_view arg = argv[i];
        if (arg == "-h" || arg == "--help") {
            _opts.help = true;
        } else if (arg == "--json") {
            _opts.json = true;
        } else if (arg == "--off-number") {
            _opts.need_number = false;
        } else if (arg == "--off-name") {
            _opts.need_name = false;
        } else if (arg == "--off-date") {
            _opts.need_date = false;
        } else if (arg == "--off-youbi") {
            _opts.need_youbi = false;
        } else if (arg == "--off-time") {
            _opts.need_time = false;
        } else if (arg == "--off-id") {
            _opts.need_id = false;
        } else if (arg == "--off-all") {
            _opts.need_number = false;
            _opts.need_name = false;
            _opts.need_date = false;
            _opts.need_youbi = false;
            _opts.need_time = false;
            _opts.need_id = false;
        }
    }
}

void App::_show_usage() const {
    std::cout << "ccres\n";
    std::cout << "\n";
    std::cout << "    The blog comments filter.\n";
    std::cout << "\n";
    std::cout << "Usage:\n";
    std::cout << "\n";
    std::cout << "    $ ccres [options]\n";
    std::cout << "\n";
    std::cout << "The options are:\n";
    std::cout << "\n";
    std::cout << "    --help    Show usage.\n";
    std::cout << "    --json    Dump json format.\n";
    std::cout << std::endl;
}

int App::run() {
    setlocale(LC_ALL, "");

    try {
        _parse_opts(_argc, _argv);
    } catch (const InvalidOptionError &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    if (_opts.help) {
        _show_usage();
        return 0;
    }

    auto cleaner = Cleaner();
    auto tokenizer = Tokenizer();
    auto parser = Parser(_opts);
    auto renderer = Renderer();

    auto text = String();
    text.read_stream(std::wcin);
    text = cleaner.clean(text);
    auto tokens = tokenizer.tokenize(text);
    auto responses = parser.parse(tokens);

    if (_opts.json) {
        renderer.render_responses_as_json(responses);
    } else {
        renderer.render_responses(responses);
    }
    return 0;
}

}  // ccres
