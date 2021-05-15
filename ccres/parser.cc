#include <ccres/parser.h>

namespace ccres {

Parser::~Parser() {}

Parser::Parser(bool check)
    : _p()
    , _end()
    , _responses()
    , _response()
    , _check(check) {}

const std::vector<std::shared_ptr<Response>> &Parser::parse(const std::vector<std::shared_ptr<Token>> &tokens) {
    _responses.clear();
    _p = tokens.cbegin();
    _end = tokens.cend();

    for (; _p != _end; ) {
        auto savep = _p;
        bool ok = _p_number();
        if (ok) {
            _responses.emplace_back(std::move(_response));
        } else {
            _p = savep + 1;
        }
    }

    return _responses;
}

void Parser::_read_if(token_type_t type) {
    if ((*_p)->type == type) {
        _p++;
    }
}

void Parser::_read_newlines() {
    for (; _p != _end; ) {
        auto t = *_p++;
        if (t->type != TOKEN_TYPE_NEWLINE) {
            _p--;
            break;
        }
    }
}

void Parser::_read_spaces() {
    for (; _p != _end; ) {
        auto t = *_p++;
        if (t->type != TOKEN_TYPE_SPACES) {
            _p--;
            break;
        }
    }
}

void Parser::_read_spaces_and_newlines() {
    for (; _p != _end; ) {
        auto t = *_p++;
        if (!(t->type == TOKEN_TYPE_SPACES ||
              t->type == TOKEN_TYPE_NEWLINE)) {
            _p--;
            break;
        }
    }
}

bool Parser::is_response(
    std::vector<std::shared_ptr<Token>>::const_iterator p,
    std::vector<std::shared_ptr<Token>>::const_iterator end
) {
    _p = p;
    _end = end;
    return _p_number();
}

bool Parser::_p_number() {
    if (_p == _end) {
        return false;
    }

    auto t = *_p++;
    if (t->type != TOKEN_TYPE_DIGIT) {
        return false;
    }

    _response = std::make_shared<Response>();
    try {
        _response->number = std::stoi(t->text);
    } catch (const std::out_of_range &e) {
        return false;
    }

    _read_spaces_and_newlines();
    _read_if(TOKEN_TYPE_COLON);

    if (_p == _end) {
        return false;
    }

    return _p_name();
}

bool Parser::_p_name() {
    if (_p == _end) {
        return false;
    }

    _read_spaces_and_newlines();
    auto t = *_p++;
    if (t->type != TOKEN_TYPE_TEXT) {
        _p--;
        return _p_date();
    }

    _response->name = t->text;

    if (_p == _end) {
        return false;
    }

    return _p_date();
}

bool Parser::_p_date() {
    if (_p == _end) {
        return false;
    }

    auto savep = _p;
    _response->datetime = (struct tm){
        .tm_wday = -1,
    };

    // year
    _read_spaces_and_newlines();
    auto t = *_p++;
    if (t->type != TOKEN_TYPE_DIGIT) {
        _p = savep;
        return _p_id();
    }
    try {
        _response->datetime.tm_year = std::stoi(t->text) - 1900;
    } catch (const std::out_of_range &e) {
        return false;
    }

    if (_p == _end) {
        return false;
    }

    _read_spaces_and_newlines();
    t = *_p++;
    if (t->type == TOKEN_TYPE_COLON) {
        _p = savep;
        _response->datetime.tm_year = 0;
        return _p_time();
    } else if (t->type != TOKEN_TYPE_DATE_SEP) {
        _p--;
        return _p_youbi();
    }

    if (_p == _end) {
        return false;
    }

    // month
    _read_spaces_and_newlines();
    t = *_p++;
    if (t->type != TOKEN_TYPE_DIGIT) {
        _p--;
        return _p_youbi();
    }
    try {
        _response->datetime.tm_mon = std::stoi(t->text) - 1;
    } catch (const std::out_of_range &e) {
        return false;
    }

    if (_p == _end) {
        return false;
    }

    _read_spaces_and_newlines();
    t = *_p++;
    if (t->type != TOKEN_TYPE_DATE_SEP) {
        _p--;
        return _p_youbi();
    }

    if (_p == _end) {
        return false;
    }

    // day
    _read_spaces_and_newlines();
    t = *_p++;
    if (t->type != TOKEN_TYPE_DIGIT) {
        _p--;
        return _p_youbi();
    }
    try {
        _response->datetime.tm_mday = std::stoi(t->text);
    } catch (const std::out_of_range &e) {
        return false;
    }

    if (_p == _end) {
        return true;
    }

    return _p_youbi();
}

bool Parser::_p_youbi() {
    if (_p == _end) {
        return true;
    }

    auto savep = _p;

    // youbi left
    _read_spaces_and_newlines();
    auto t = *_p++;
    if (t->type != TOKEN_TYPE_YOUBI_LEFT) {
        _p--;
        return _p_time();
    }

    if (_p == _end) {
        return true;
    }

    // youbi text
    _read_spaces_and_newlines();
    t = *_p++;
    if (t->type != TOKEN_TYPE_TEXT) {
        _p = savep;
        return _p_time();
    }

    static const wchar_t *youbi[] = {L"日", L"月", L"火", L"水", L"木", L"金", L"土", NULL};
    _response->datetime.tm_wday = -1;
    for (int i = 0; youbi[i]; i++) {
        if (youbi[i] == t->text) {
            _response->datetime.tm_wday = i;
            break;
        }
    }

    if (_p == _end) {
        return true;
    }

    // youbi right
    _read_spaces_and_newlines();
    t = *_p++;
    if (t->type != TOKEN_TYPE_YOUBI_RIGHT) {
        _p--;
        return _p_time();
    }

    if (_p == _end) {
        return true;
    }

    return _p_time();
}

bool Parser::_p_time() {
    if (_p == _end) {
        return true;
    }

    // hour
    _read_spaces_and_newlines();
    auto t = *_p++;
    if (t->type != TOKEN_TYPE_DIGIT) {
        _p--;
        return _p_id();
    }
    try {
        _response->datetime.tm_hour = std::stoi(t->text);
    } catch (const std::out_of_range &e) {
        return false;
    }

    if (_p == _end) {
        return false;
    }

    t = *_p++;
    if (t->type != TOKEN_TYPE_COLON) {
        _p--;
        return _p_id();
    }

    if (_p == _end) {
        return false;
    }

    // min
    _read_spaces_and_newlines();
    t = *_p++;
    if (t->type != TOKEN_TYPE_DIGIT) {
        _p--;
        return _p_id();
    }
    try {
        _response->datetime.tm_min = std::stoi(t->text);
    } catch (const std::out_of_range &e) {
        return false;
    }

    if (_p == _end) {
        return false;
    }

    t = *_p++;
    if (t->type != TOKEN_TYPE_COLON) {
        _p--;
        return _p_id();
    }

    if (_p == _end) {
        return false;
    }

    // sec
    _read_spaces_and_newlines();
    t = *_p++;
    if (t->type != TOKEN_TYPE_DIGIT) {
        _p--;
        return _p_id();
    }
    try {
        _response->datetime.tm_sec = std::stoi(t->text);
    } catch (const std::out_of_range &e) {
        return false;
    }

    if (_p == _end) {
        return true;
    }

    return _p_id();
}

bool Parser::_p_id() {
    if (_p == _end) {
        return true;
    }

    if (_check) {
        return true;
    }

    _read_spaces_and_newlines();
    auto t = *_p++;
    if (t->type != TOKEN_TYPE_ID) {
        _p--;
        return _p_content();
    }

    if (_p == _end) {
        return true;
    }

    auto buf = String();
    buf += t->text;

    for (; _p != _end; ) {
        t = *_p++;
        if (t->type == TOKEN_TYPE_SPACES ||
            t->type == TOKEN_TYPE_NEWLINE) {
            break;
        } else {
            buf += t->text;
        }
    }

    _response->id = std::move(buf);
    return _p_content();
}

bool Parser::_p_content() {
    auto buf = String();
    auto check_parser = Parser(true);

    for (; _p != _end; ) {
        bool is_res = check_parser.is_response(_p, _end);
        if (is_res) {
            goto done;
        }
        auto t = *_p++;
        buf += t->text;
    }

done:
    if (buf.length()) {
        _response->content = std::move(buf);
    }
    return true;
}

};  // ccres
