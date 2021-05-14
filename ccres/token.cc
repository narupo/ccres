#include <ccres/token.h>

namespace ccres {

Token::~Token() {}
Token::Token() : type(), text() {}
Token::Token(token_type_t type, String text) : type(type), text(text) {}

};  // ccres
