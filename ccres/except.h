#include <stdexcept>

namespace ccres {

class InvalidOptionError : public std::runtime_error {
public:
    InvalidOptionError(const char *message)
        : std::runtime_error(message) {}
};

};  // ccres
