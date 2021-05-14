#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include <ctime>
#include <ccres/response.h>

namespace ccres {

class Renderer {
public:
    void render_responses(const std::vector<std::shared_ptr<Response>> &responses);
};

};  // ccres
