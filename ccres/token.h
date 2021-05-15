#pragma once
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
};

};  // ccres
