#pragma once

namespace ccres {

typedef enum {
    TOKEN_TYPE_TEXT,
    TOKEN_TYPE_SPACES,
} token_type_t;

struct Token {
    token_type_t type;
    ~Token();
    Token();
};

};  // ccres
