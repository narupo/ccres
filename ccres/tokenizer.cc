#include <ccres/tokenizer.h>

namespace ccres {

Tokenizer::~Tokenizer() {
}

Tokenizer::Tokenizer() : _tokens(), _p(), _end() {
}

const std::vector<Token> &Tokenizer::tokenize(const String &text) {
    _tokens.clear();
    _p = text.cbegin();
    _end = text.cend();
    auto buf = String();

    auto store = [&](token_type_t type, String &&text) {
        auto token = Token(type, std::move(text));
        _tokens.emplace_back(std::move(token));
    };
    auto store_text = [&]() {
        if (buf.length()) {
            auto token = Token(TOKEN_TYPE_TEXT, std::move(buf));
            _tokens.emplace_back(std::move(token));
            buf.clear();
        }
    };

    for (; _p != _end; ) {
        auto c1 = *_p++;
        auto c2 = *_p;

        if (isdigit(c1)) {
            store_text();
            _p--;
            _read_digits();
        } else if (isspace(c1)) {
            store_text();
            _p--;
            _read_spaces();
        } else if (iscolon(c1)) {
            store_text();
            auto s = String();
            s.push_back(c1);
            store(TOKEN_TYPE_COLON, std::move(s));
        } else if (isdatesep(c1)) {
            store_text();
            auto s = String();
            s.push_back(c1);
            store(TOKEN_TYPE_DATE_SEP, std::move(s));
        } else if (c1 == '\r' && c2 == '\n') {
            store_text();
            auto s = String();
            s.push_back(c1);
            s.push_back(c2);
            store(TOKEN_TYPE_NEWLINE, std::move(s));
        } else if (c1 == '\r' || c1 == '\n') {
            store_text();
            auto s = String();
            s.push_back(c1);
            store(TOKEN_TYPE_NEWLINE, std::move(s));
        } else {
            buf += c1;
        }
    }

    return _tokens;
}

void Tokenizer::_read_spaces() {
    auto buf = String();

    for (; _p != _end; ) {
        auto c = *_p++;
        if (isspace(c)) {
            buf += c;
        } else {
            _p--;
            break;
        }
    }

    if (buf.length()) {
        auto token = Token(TOKEN_TYPE_SPACES, std::move(buf));
        _tokens.emplace_back(std::move(token));
    }
}

void Tokenizer::_read_digits() {
    auto buf = String();
    int m = 0;

    for (; _p != _end; ) {
        auto c = *_p++;
        switch (m) {
        case 0:
            if (isdigit(c)) {
                buf += c;
            } else if (isdot(c)) {
                buf += c;
                m = 10;
            } else {
                _p--;
                goto done;
            }
            break;
        case 10:  // found dot
            if (isdigit(c)) {
                buf += c;
            } else {
                _p--;
                goto done;
            }
            break;
        }
    }

done:
    if (buf.length()) {
        auto &&tok = Token(TOKEN_TYPE_DIGIT, std::move(buf));
        _tokens.emplace_back(std::move(tok));
    }
}

};  // ccres
