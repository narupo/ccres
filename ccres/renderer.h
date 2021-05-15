#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include <ctime>
#include <sstream>
#include <ccres/string.h>
#include <ccres/response.h>

namespace ccres {

class Renderer {
public:
    void render_responses(const std::vector<std::shared_ptr<Response>> &responses);
    void render_responses_as_json(const std::vector<std::shared_ptr<Response>> &responses);

private:
    String _escape(const String &s);
    wchar_t *_conv_datetime(wchar_t *dst, size_t dstsize, struct tm *tm);
};

};  // ccres
