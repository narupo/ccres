#include <ccres/renderer.h>

namespace ccres {

wchar_t *Renderer::_conv_datetime(wchar_t *dst, size_t dstsize, struct tm *tm) {
    if (tm->tm_year && tm->tm_wday >= 0 && tm->tm_hour) {
        std::wcsftime(dst, dstsize, L"%Y-%m-%d (%a) %H:%M:%S", tm);
        return dst;
    } else if (tm->tm_year && tm->tm_hour) {
        std::wcsftime(dst, dstsize, L"%Y-%m-%d %H:%M:%S", tm);
        return dst;
    } else if (tm->tm_hour) {
        std::wcsftime(dst, dstsize, L"%H:%M:%S", tm);
        return dst;
    } else {
        return nullptr;
    }
}

void Renderer::render_responses(const std::vector<std::shared_ptr<Response>> &responses) {
    wchar_t d[100] {0};

    for (const auto &r : responses) {
        std::wcout << r->number;
        if (r->name.length()) {
            std::wcout << " " << r->name;
        }
        if (_conv_datetime(d, sizeof d, &r->datetime)) {
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

void Renderer::render_responses_as_json(const std::vector<std::shared_ptr<Response>> &responses) {
    std::wstringstream json;
    wchar_t d[100];
    decltype(responses.size()) i = 0;
    json << L"[\n";

    for (const auto &r : responses) {
        json << L"\t{\n";
        json << L"\t\t\"number\": " << r->number << ",\n";
        json << L"\t\t\"name\": \"" << _escape(r->name) << "\",\n";
        if (_conv_datetime(d, sizeof d, &r->datetime)) {
            json << L"\t\t\"datetime\": \"" << d << "\",\n";    
        }
        json << L"\t\t\"id\": \"" << _escape(r->id) << "\",\n";
        json << L"\t\t\"content\": \"" << _escape(r->content) << "\"\n";
        json << L"\t}";
        if (i < responses.size() - 1) {
            json << L",\n";
        } else {
            json << L"\n";
        }
        i++;
    }

    json << L"]\n";
    std::wcout << json.str() << std::endl;
}

String Renderer::_escape(const String &s) {
    auto dst = String();

    for (const auto &c : s) {
        switch (c) {
        default: dst += c; break;
        case L'"':
            dst += L"\\";
            dst += c;
            break;
        case L'\r': dst += L'\\'; dst += L'r'; break;
        case L'\n': dst += L'\\'; dst += L'n'; break;
        case L'\t': dst += L'\\'; dst += L't'; break;
        case L'\a': dst += L'\\'; dst += L'a'; break;
        case L'\b': dst += L'\\'; dst += L'b'; break;
        case L'\f': dst += L'\\'; dst += L'f'; break;
        case L'\v': dst += L'\\'; dst += L'v'; break;
        }
    }

    return dst;
}

};  // ccres
