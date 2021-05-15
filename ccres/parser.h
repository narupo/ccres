#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include <cstdint>
#include <ctime>
#include <ccres/token.h>
#include <ccres/response.h>

namespace ccres {

class Parser {
private:
    std::vector<std::shared_ptr<Token>>::const_iterator _p;
    std::vector<std::shared_ptr<Token>>::const_iterator _end;
    std::vector<std::shared_ptr<Response>> _responses;
    std::shared_ptr<Response> _response;
    bool _check;

public:
    ~Parser();
    Parser(bool check=false);
    const std::vector<std::shared_ptr<Response>> &parse(const std::vector<std::shared_ptr<Token>> &tokens);
    bool is_response(
        std::vector<std::shared_ptr<Token>>::const_iterator p,
        std::vector<std::shared_ptr<Token>>::const_iterator end
    );

private:
    void _read_if(token_type_t type);
    void _read_newlines();
    void _read_spaces();
    void _read_spaces_and_newlines();
    bool _p_number();
    bool _p_name();
    bool _p_date();
    bool _p_youbi();
    bool _p_time();
    bool _p_id();
    bool _p_content();
};

};  // ccres
