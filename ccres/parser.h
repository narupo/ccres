#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include <cstdint>
#include <ctime>
#include <ccres/token.h>
#include <ccres/response.h>
#include <ccres/opts.h>

namespace ccres {

typedef std::vector<std::shared_ptr<Token>> tokens_t;
typedef std::vector<std::shared_ptr<Token>>::const_iterator tokens_it_t;

struct TokenIterators {
    tokens_it_t p;
    tokens_it_t end;
};

class Parser {
public:

private:
    Options _opts;
    bool _check;
    std::vector<std::shared_ptr<Response>> _responses;
    std::shared_ptr<Response> _response;

public:
    ~Parser();
    Parser(Options opts, bool check=false);
    const std::vector<std::shared_ptr<Response>> &parse(const tokens_t &tokens);
    bool is_response(TokenIterators &its);

private:
    void _read_if(TokenIterators &its, token_type_t type);
    void _read_newlines(TokenIterators &its);
    void _read_spaces(TokenIterators &its);
    void _read_spaces_and_newlines(TokenIterators &its);
    bool _next_is_date(TokenIterators &its);
    bool _next_is_youbi(TokenIterators &its);
    bool _next_is_time(TokenIterators &its);
    bool _next_is_id(TokenIterators &its);
    bool _p_number(TokenIterators &its);
    bool _p_name(TokenIterators &its);
    bool _p_date(TokenIterators &its);
    bool _p_youbi(TokenIterators &its);
    bool _p_time(TokenIterators &its);
    bool _p_id(TokenIterators &its);
    bool _p_content(TokenIterators &its);
};

};  // ccres
