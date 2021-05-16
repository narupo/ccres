#pragma once
#include <sstream>
#include <ccres/string.h>

namespace ccres {

typedef enum {
    TOKEN_TYPE_TEXT,
    TOKEN_TYPE_SPACES,
    TOKEN_TYPE_DIGIT,
    TOKEN_TYPE_COLON,
    TOKEN_TYPE_DATE_SEP,
    TOKEN_TYPE_NEWLINE,
    TOKEN_TYPE_ID,
    TOKEN_TYPE_YOUBI_LEFT,
    TOKEN_TYPE_YOUBI_RIGHT,
} token_type_t;

struct Token {
    token_type_t type;
    String text;
    ~Token();
    Token();
    Token(token_type_t type, String text);

    std::wstring to_string() const {
        std::wstringstream wss;

        switch (type) {
        case TOKEN_TYPE_TEXT: wss << L"text"; break;
        case TOKEN_TYPE_SPACES: wss << L"spaces"; break;
        case TOKEN_TYPE_DIGIT: wss << L"digit"; break;
        case TOKEN_TYPE_COLON: wss << L"colon"; break;
        case TOKEN_TYPE_DATE_SEP: wss << L"date-sep"; break;
        case TOKEN_TYPE_NEWLINE: wss << L"newline"; break;
        case TOKEN_TYPE_ID: wss << L"id"; break;
        case TOKEN_TYPE_YOUBI_LEFT: wss << L"youbi-left"; break;
        case TOKEN_TYPE_YOUBI_RIGHT: wss << L"youbi-right"; break;
        }

        wss << ": " << text;
        return wss.str();
    }
};

};  // ccres
