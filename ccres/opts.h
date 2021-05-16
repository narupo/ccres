#pragma once
#include <iostream>

struct Options {
    bool help;
    bool json;
    bool pre;
    bool multi_line_name;
    bool render_tokens;
    bool auto_;
    bool need_number;
    bool need_name;
    bool need_date;
    bool need_youbi;
    bool need_time;
    bool need_id;

    void dump() const {
        std::wcout << L"help: " << help << "\n";
        std::wcout << L"json: " << json << "\n";
        std::wcout << L"pre: " << pre << "\n";
        std::wcout << L"multi_line_name: " << multi_line_name << "\n";
        std::wcout << L"render_tokens: " << render_tokens << "\n";
        std::wcout << L"auto_: " << auto_ << "\n";
        std::wcout << L"need_number: " << need_number << "\n";
        std::wcout << L"need_name: " << need_name << "\n";
        std::wcout << L"need_date: " << need_date << "\n";
        std::wcout << L"need_youbi: " << need_youbi << "\n";
        std::wcout << L"need_time: " << need_time << "\n";
        std::wcout << L"need_id: " << need_id << "\n";
        std::wcout << std::flush;
    }
};
