#include <ccres/parser.h>

namespace ccres {

Parser::~Parser() {}

Parser::Parser(Options opts, bool check)
    : _opts(opts)
    , _check(check)
    , _responses()
    , _response()
    {}

const std::vector<std::shared_ptr<Response>> &Parser::parse(const std::vector<std::shared_ptr<Token>> &tokens) {
    _responses.clear();
    TokenIterators its {
        .p = tokens.cbegin(),
        .end = tokens.cend(),
    };

    for (; its.p != its.end; ) {
        auto savep = its.p;
        bool ok = _p_number(its);
        if (ok) {
            _responses.emplace_back(std::move(_response));
        } else {
            its.p = savep + 1;
        }
    }

    return _responses;
}

void Parser::_read_if(TokenIterators &its, token_type_t type) {
    if (its.p == its.end) {
        return;
    }

    if ((*its.p)->type == type) {
        its.p++;
    }
}

void Parser::_read_spaces_and_newlines(TokenIterators &its) {
    for (; its.p != its.end; ) {
        auto t = *its.p++;
        if (!(t->type == TOKEN_TYPE_SPACES ||
              t->type == TOKEN_TYPE_NEWLINE)) {
            its.p--;
            break;
        }
    }
}

bool Parser::is_response(TokenIterators &its) {
    return _p_number(its);
}

bool Parser::_p_number(TokenIterators &its) {
    if (its.p == its.end) {
        return false;
    }

    _response = std::make_shared<Response>();

    if (!_opts.need_number) {
        return _p_name(its);
    }

    auto t = *its.p++;
    if (t->type != TOKEN_TYPE_DIGIT) {
        return false;
    }

    try {
        _response->number = std::stoi(t->text);
    } catch (const std::out_of_range &e) {
        return false;
    }

    _read_spaces_and_newlines(its);
    _read_if(its, TOKEN_TYPE_COLON);

    if (its.p == its.end) {
        if (_opts.need_name || _opts.need_date ||
            _opts.need_youbi || _opts.need_time || 
            _opts.need_id) {
            return false;
        } else {
            return true;
        }
    }

    return _p_name(its);
}

bool Parser::_next_is_date(TokenIterators &its) {
    if (its.p == its.end) {
        return false;
    }

    _read_spaces_and_newlines(its);
    auto t = *its.p++;
    if (t->type != TOKEN_TYPE_DIGIT) {
        return false;
    }

    _read_spaces_and_newlines(its);
    t = *its.p++;
    if (t->type != TOKEN_TYPE_DATE_SEP) {
        return false;
    }

    return true;
}

bool Parser::_next_is_youbi(TokenIterators &its) {
    if (its.p == its.end) {
        return false;
    }

    _read_spaces_and_newlines(its);
    auto t = *its.p++;
    if (t->type != TOKEN_TYPE_YOUBI_LEFT) {
        return false;
    }

    _read_spaces_and_newlines(its);
    t = *its.p++;
    if (!(t->type == TOKEN_TYPE_TEXT ||
         (t->type == TOKEN_TYPE_DATE_SEP && (t->text == L"月" || t->text == L"日")))) {
        return false;
    }

    return true;
}

bool Parser::_next_is_time(TokenIterators &its) {
    if (its.p == its.end) {
        return false;
    }

    _read_spaces_and_newlines(its);
    auto t = *its.p++;
    if (t->type != TOKEN_TYPE_DIGIT) {
        return false;
    }

    _read_spaces_and_newlines(its);
    t = *its.p++;
    if (t->type != TOKEN_TYPE_COLON) {
        return false;
    }

    return true;
}

bool Parser::_next_is_id(TokenIterators &its) {
    if (its.p == its.end) {
        return false;
    }

    _read_spaces_and_newlines(its);
    auto t = *its.p++;
    if (t->type != TOKEN_TYPE_ID) {
        return false;
    }

    return true;
}

bool Parser::_p_name(TokenIterators &its) {
    if (its.p == its.end) {
        return false;
    }

    if (!_opts.need_name) {
        return _p_date(its);
    }

    _read_spaces_and_newlines(its);

    auto buf = String();

    for (; its.p != its.end; its.p++) {
        if (_opts.multi_line_name) {
            auto c = its;
            if ((_opts.need_date && _next_is_date(c))) {
                break;
            }
            c = its;
            if (_opts.need_youbi && _next_is_youbi(c)) {
                break;
            }
            c = its;
            if (_opts.need_time && _next_is_time(c)) {
                break;
            }
            c = its;
            if (_opts.need_id && _next_is_id(c)) {
                break;
            }
            buf += (*its.p)->text;
        } else {
            auto t = *its.p;
            if (t->type == TOKEN_TYPE_SPACES ||
                t->type == TOKEN_TYPE_NEWLINE ||
                t->type == TOKEN_TYPE_DIGIT ||
                t->type == TOKEN_TYPE_YOUBI_LEFT || 
                t->type == TOKEN_TYPE_ID) {
                break;
            } else {
                buf += t->text;
            }            
        }
    }

    _response->name = std::move(buf);

    if (its.p == its.end) {
        if (_opts.need_date || _opts.need_youbi ||
            _opts.need_time || _opts.need_id) {
            return false;
        } else {
            return true;
        }
    }

    return _p_date(its);
}

bool Parser::_p_date(TokenIterators &its) {
    if (its.p == its.end) {
        return false;
    }

    if (!_opts.need_date) {
        return _p_youbi(its);
    }

    _response->datetime = (struct tm){
        .tm_wday = -1,
    };

    // year
    _read_spaces_and_newlines(its);
    auto t = *its.p++;
    if (t->type != TOKEN_TYPE_DIGIT) {
        return false;
    }
    try {
        _response->datetime.tm_year = std::stoi(t->text) - 1900;
    } catch (const std::out_of_range &e) {
        return false;
    }

    if (its.p == its.end) {
        return false;
    }

    _read_spaces_and_newlines(its);
    t = *its.p++;
    if (t->type != TOKEN_TYPE_DATE_SEP) {
        its.p--;
        return _p_youbi(its);
    }

    if (its.p == its.end) {
        return false;
    }

    // month
    _read_spaces_and_newlines(its);
    t = *its.p++;
    if (t->type != TOKEN_TYPE_DIGIT) {
        return false;
    }
    try {
        _response->datetime.tm_mon = std::stoi(t->text) - 1;
    } catch (const std::out_of_range &e) {
        return false;
    }

    if (its.p == its.end) {
        return false;
    }

    _read_spaces_and_newlines(its);
    t = *its.p++;
    if (t->type != TOKEN_TYPE_DATE_SEP) {
        its.p--;
        return _p_youbi(its);
    }

    if (its.p == its.end) {
        return false;
    }

    // day
    _read_spaces_and_newlines(its);
    t = *its.p++;
    if (t->type != TOKEN_TYPE_DIGIT) {
        return false;
    }
    try {
        _response->datetime.tm_mday = std::stoi(t->text);
    } catch (const std::out_of_range &e) {
        return false;
    }

    if (its.p == its.end) {
        if (_opts.need_youbi || _opts.need_time || _opts.need_id) {
            return false;
        } else {
            return true;
        }
    }

    t = *its.p++;
    if (t->type != TOKEN_TYPE_DATE_SEP) {
        its.p--;
    }

    if (its.p == its.end) {
        if (_opts.need_youbi || _opts.need_time || _opts.need_id) {
            return false;
        } else {
            return true;
        }
    }

    return _p_youbi(its);
}

bool Parser::_p_youbi(TokenIterators &its) {
    if (its.p == its.end) {
        return true;
    }

    if (!_opts.need_youbi) {
        return _p_time(its);
    }

    // youbi left
    _read_spaces_and_newlines(its);
    auto t = *its.p++;
    if (t->type != TOKEN_TYPE_YOUBI_LEFT) {
        return false;
    }

    if (its.p == its.end) {
        return false;
    }

    // youbi text
    _read_spaces_and_newlines(its);
    t = *its.p++;
    if (!(t->type == TOKEN_TYPE_TEXT ||
          (t->type == TOKEN_TYPE_DATE_SEP && (t->text == L"月" || t->text == L"日")))) {
        return false;
    }

    static const wchar_t *youbi[] = {L"日", L"月", L"火", L"水", L"木", L"金", L"土", NULL};
    _response->datetime.tm_wday = -1;
    for (int i = 0; youbi[i]; i++) {
        if (youbi[i] == t->text) {
            _response->datetime.tm_wday = i;
            break;
        }
    }

    if (its.p == its.end) {
        return true;
    }

    // youbi right
    _read_spaces_and_newlines(its);
    t = *its.p++;
    if (t->type != TOKEN_TYPE_YOUBI_RIGHT) {
        return false;
    }

    if (its.p == its.end) {
        if (_opts.need_time || _opts.need_id) {
            return false;
        } else {
            return true;
        }
    }

    return _p_time(its);
}

bool Parser::_p_time(TokenIterators &its) {
    if (its.p == its.end) {
        return true;
    }

    if (!_opts.need_time) {
        return _p_id(its);
    }

    // hour
    _read_spaces_and_newlines(its);
    auto t = *its.p++;
    if (t->type != TOKEN_TYPE_DIGIT) {
        return false;
    }
    try {
        _response->datetime.tm_hour = std::stoi(t->text);
    } catch (const std::out_of_range &e) {
        return false;
    }

    if (its.p == its.end) {
        return false;
    }

    t = *its.p++;
    if (t->type != TOKEN_TYPE_COLON) {
        its.p--;
        return _p_id(its);
    }

    if (its.p == its.end) {
        return false;
    }

    // min
    _read_spaces_and_newlines(its);
    t = *its.p++;
    if (t->type != TOKEN_TYPE_DIGIT) {
        return false;
    }
    try {
        _response->datetime.tm_min = std::stoi(t->text);
    } catch (const std::out_of_range &e) {
        return false;
    }

    if (its.p == its.end) {
        return false;
    }

    t = *its.p++;
    if (t->type != TOKEN_TYPE_COLON) {
        its.p--;
        return _p_id(its);
    }

    if (its.p == its.end) {
        return false;
    }

    // sec
    _read_spaces_and_newlines(its);
    t = *its.p++;
    if (t->type != TOKEN_TYPE_DIGIT) {
        return false;
    }
    try {
        _response->datetime.tm_sec = std::stoi(t->text);
    } catch (const std::out_of_range &e) {
        return false;
    }

    if (its.p == its.end) {
        if (_opts.need_id) {
            return false;
        } else {
            return true;
        }
    }

    return _p_id(its);
}

bool Parser::_p_id(TokenIterators &its) {
    if (its.p == its.end) {
        return true;
    }

    if (_check) {
        return true;
    }

    if (!_opts.need_id) {
        return _p_content(its);
    }

    _read_spaces_and_newlines(its);
    auto t = *its.p++;
    if (t->type != TOKEN_TYPE_ID) {
        return false;
    }

    if (its.p == its.end) {
        return true;
    }

    auto buf = String();
    buf += t->text;

    for (; its.p != its.end; ) {
        t = *its.p++;
        if (t->type == TOKEN_TYPE_SPACES ||
            t->type == TOKEN_TYPE_NEWLINE) {
            break;
        } else {
            buf += t->text;
        }
    }

    _response->id = std::move(buf);
    return _p_content(its);
}

bool Parser::_p_content(TokenIterators &its) {
    auto buf = String();
    auto check_parser = Parser(_opts, true);

    _read_spaces_and_newlines(its);
    for (; its.p != its.end; ) {
        TokenIterators cits = its;
        bool is_res = check_parser.is_response(cits);
        if (is_res) {
            goto done;
        }
        auto t = *its.p++;
        buf += t->text;
    }

done:
    if (buf.length()) {
        _response->content = std::move(buf);
    }
    return true;
}

};  // ccres
