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
    _read_newlines();
    _read_spaces();
    _read_newlines();
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
    _response->number = std::stoi(t->text);

    return _p_name();
}

bool Parser::_p_name() {
    if (_p == _end) {
        return false;
    }

    _read_spaces_and_newlines();
    auto t = *_p++;
    if (t->type != TOKEN_TYPE_TEXT) {
        return false;
    }

    _response->name = t->text;

    return _p_date();
}

bool Parser::_p_date() {
    if (_p == _end) {
        return false;
    }

    auto savep = _p;
    _response->datetime = (struct tm){0};

    // year
    _read_spaces_and_newlines();
    auto t = *_p++;
    if (t->type != TOKEN_TYPE_DIGIT) {
        _p = savep;
        return _p_id();
    }
    _response->datetime.tm_year = std::stoi(t->text) - 1900;

    _read_spaces_and_newlines();
    t = *_p++;
    if (t->type != TOKEN_TYPE_DATE_SEP) {
        _p = savep;
        return _p_time();
    }

    // month
    _read_spaces_and_newlines();
    t = *_p++;
    if (t->type != TOKEN_TYPE_DIGIT) {
        _p--;
        return _p_time();
    }
    _response->datetime.tm_mon = std::stoi(t->text) - 1;

    _read_spaces_and_newlines();
    t = *_p++;
    if (t->type != TOKEN_TYPE_DATE_SEP) {
        _p--;
        return _p_time();
    }

    // day
    _read_spaces_and_newlines();
    t = *_p++;
    if (t->type != TOKEN_TYPE_DIGIT) {
        _p--;
        return _p_time();
    }
    _response->datetime.tm_mday = std::stoi(t->text);

    return _p_time();
}

bool Parser::_p_time() {
    if (_p == _end) {
        return false;
    }

    auto savep = _p;

    // hour
    _read_spaces_and_newlines();
    auto t = *_p++;
    if (t->type != TOKEN_TYPE_DIGIT) {
        _p--;
        return _p_id();
    }
    _response->datetime.tm_hour = std::stoi(t->text);

    t = *_p++;
    if (t->type != TOKEN_TYPE_COLON) {
        _p = savep;
        return _p_id();
    }

    // min
    _read_spaces_and_newlines();
    t = *_p++;
    if (t->type != TOKEN_TYPE_DIGIT) {
        _p = savep;
        return _p_id();
    }
    _response->datetime.tm_min = std::stoi(t->text);

    t = *_p++;
    if (t->type != TOKEN_TYPE_COLON) {
        _p = savep;
        return _p_id();
    }

    // sec
    _read_spaces_and_newlines();
    t = *_p++;
    if (t->type != TOKEN_TYPE_DIGIT) {
        return _p_id();
    }
    _response->datetime.tm_sec = std::stoi(t->text);

    return _p_id();
}

bool Parser::_p_id() {
    if (_p == _end) {
        return false;
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
