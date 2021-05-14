#pragma once
#include <cstdint>
#include <ctime>
#include <ccres/string.h>

namespace ccres {

/*
12: name: 2021-01-01 00:00:00.00: ID xxx
*/
struct Response {
    int32_t number;
    String name;
    struct tm datetime;
    String id;
    String content;
};

};  // ccres
