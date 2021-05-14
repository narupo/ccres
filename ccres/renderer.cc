#include <ccres/renderer.h>

namespace ccres {

void Renderer::render_responses(const std::vector<std::shared_ptr<Response>> &responses) {
    char d[100];

    for (const auto &r : responses) {
        std::wcout << r->number;
        if (r->name.length()) {
            std::wcout << " " << r->name;
        }
        if (r->datetime.tm_year && r->datetime.tm_hour) {
            std::strftime(d, sizeof d, "%Y-%m-%d %H:%M:%S", &r->datetime);
            std::wcout << " " << d;
        } else if (r->datetime.tm_hour) {
            std::strftime(d, sizeof d, "%H:%M:%S", &r->datetime);
            std::wcout << " " << d;
        }
        if (r->id.length()) {
            std::wcout << " " << r->id;
        }
        if (r->content.length()) {
            std::wcout << "\n" << r->content << "\n";
        }

        std::wcout << "--------------------------------------------------------------------------------" << std::endl;
    }
}

};  // ccres
