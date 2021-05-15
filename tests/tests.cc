#include <tests/tests.h>

namespace ccres {

static void test_parser() {
    auto t = Tokenizer();
    auto p = Parser();
    auto s = String();

    s.assign(L"123 name 2021-01-02 (月) 01:23:45 ID:abc\ncontent");
    auto tokens = t.tokenize(s);
    auto responses = p.parse(tokens);
    assert(responses.size() == 1);
    assert(responses[0]->number == 123);
    assert(responses[0]->name == L"name");
    assert(responses[0]->datetime.tm_year == 121);
    assert(responses[0]->datetime.tm_mon == 0);
    assert(responses[0]->datetime.tm_mday == 2);
    assert(responses[0]->datetime.tm_wday == 1);
    assert(responses[0]->datetime.tm_hour == 1);
    assert(responses[0]->datetime.tm_min == 23);
    assert(responses[0]->datetime.tm_sec == 45);
    assert(responses[0]->id == L"ID:abc");
    assert(responses[0]->content == L"content");

    s.assign(L"123: name 2021-01-02 01:23:45 ID:abc\ncontent");
    tokens = t.tokenize(s);
    responses = p.parse(tokens);
    assert(responses.size() == 1);
    assert(responses[0]->number == 123);
    assert(responses[0]->name == L"name");
    assert(responses[0]->datetime.tm_year == 121);
    assert(responses[0]->datetime.tm_mon == 0);
    assert(responses[0]->datetime.tm_mday == 2);
    assert(responses[0]->datetime.tm_hour == 1);
    assert(responses[0]->datetime.tm_min == 23);
    assert(responses[0]->datetime.tm_sec == 45);
    assert(responses[0]->id == L"ID:abc");
    assert(responses[0]->content == L"content");

    s.assign(L"123 : name 2021-01-02 01:23:45 ID:abc\ncontent");
    tokens = t.tokenize(s);
    responses = p.parse(tokens);
    assert(responses.size() == 1);
    assert(responses[0]->number == 123);
    assert(responses[0]->name == L"name");
    assert(responses[0]->datetime.tm_year == 121);
    assert(responses[0]->datetime.tm_mon == 0);
    assert(responses[0]->datetime.tm_mday == 2);
    assert(responses[0]->datetime.tm_hour == 1);
    assert(responses[0]->datetime.tm_min == 23);
    assert(responses[0]->datetime.tm_sec == 45);
    assert(responses[0]->id == L"ID:abc");
    assert(responses[0]->content == L"content");

    s.assign(L"123 ： name 2021-01-02 01:23:45 ID:abc\ncontent");
    tokens = t.tokenize(s);
    responses = p.parse(tokens);
    assert(responses.size() == 1);
    assert(responses[0]->number == 123);
    assert(responses[0]->name == L"name");
    assert(responses[0]->datetime.tm_year == 121);
    assert(responses[0]->datetime.tm_mon == 0);
    assert(responses[0]->datetime.tm_mday == 2);
    assert(responses[0]->datetime.tm_hour == 1);
    assert(responses[0]->datetime.tm_min == 23);
    assert(responses[0]->datetime.tm_sec == 45);
    assert(responses[0]->id == L"ID:abc");
    assert(responses[0]->content == L"content");

    s.assign(L"123 2021-01-02 01:23:45 ID:abc\ncontent");
    tokens = t.tokenize(s);
    responses = p.parse(tokens);
    assert(responses.size() == 1);
    assert(responses[0]->number == 123);
    assert(responses[0]->name == L"");
    assert(responses[0]->datetime.tm_year == 121);
    assert(responses[0]->datetime.tm_mon == 0);
    assert(responses[0]->datetime.tm_mday == 2);
    assert(responses[0]->datetime.tm_hour == 1);
    assert(responses[0]->datetime.tm_min == 23);
    assert(responses[0]->datetime.tm_sec == 45);
    assert(responses[0]->id == L"ID:abc");
    assert(responses[0]->content == L"content");

    s.assign(L"123 name 2021-01 01:23:45 ID:abc\ncontent");
    tokens = t.tokenize(s);
    responses = p.parse(tokens);
    assert(responses.size() == 1);
    assert(responses[0]->number == 123);
    assert(responses[0]->name == L"name");
    assert(responses[0]->datetime.tm_year == 121);
    assert(responses[0]->datetime.tm_mon == 0);
    assert(responses[0]->datetime.tm_mday == 0);
    assert(responses[0]->datetime.tm_hour == 1);
    assert(responses[0]->datetime.tm_min == 23);
    assert(responses[0]->datetime.tm_sec == 45);
    assert(responses[0]->id == L"ID:abc");
    assert(responses[0]->content == L"content");

    s.assign(L"123 name 2021 01:23:45 ID:abc\ncontent");
    tokens = t.tokenize(s);
    responses = p.parse(tokens);
    assert(responses.size() == 1);
    assert(responses[0]->number == 123);
    assert(responses[0]->name == L"name");
    assert(responses[0]->datetime.tm_year == 121);
    assert(responses[0]->datetime.tm_mon == 0);
    assert(responses[0]->datetime.tm_mday == 0);
    assert(responses[0]->datetime.tm_hour == 1);
    assert(responses[0]->datetime.tm_min == 23);
    assert(responses[0]->datetime.tm_sec == 45);
    assert(responses[0]->id == L"ID:abc");
    assert(responses[0]->content == L"content");

    s.assign(L"123 name 01:23:45 ID:abc\ncontent");
    tokens = t.tokenize(s);
    responses = p.parse(tokens);
    assert(responses.size() == 1);
    assert(responses[0]->number == 123);
    assert(responses[0]->name == L"name");
    assert(responses[0]->datetime.tm_year == 0);
    assert(responses[0]->datetime.tm_mon == 0);
    assert(responses[0]->datetime.tm_mday == 0);
    assert(responses[0]->datetime.tm_hour == 1);
    assert(responses[0]->datetime.tm_min == 23);
    assert(responses[0]->datetime.tm_sec == 45);
    assert(responses[0]->id == L"ID:abc");
    assert(responses[0]->content == L"content");

    s.assign(L"123 2021-01-02 01:23:45 ID:abc\ncontent");
    tokens = t.tokenize(s);
    responses = p.parse(tokens);
    assert(responses.size() == 1);
    assert(responses[0]->number == 123);
    assert(responses[0]->name == L"");
    assert(responses[0]->datetime.tm_year == 121);
    assert(responses[0]->datetime.tm_mon == 0);
    assert(responses[0]->datetime.tm_mday == 2);
    assert(responses[0]->datetime.tm_hour == 1);
    assert(responses[0]->datetime.tm_min == 23);
    assert(responses[0]->datetime.tm_sec == 45);
    assert(responses[0]->id == L"ID:abc");
    assert(responses[0]->content == L"content");

    s.assign(L"123 2021-01-02 01:23 ID:abc\ncontent");
    tokens = t.tokenize(s);
    responses = p.parse(tokens);
    assert(responses.size() == 1);
    assert(responses[0]->number == 123);
    assert(responses[0]->name == L"");
    assert(responses[0]->datetime.tm_year == 121);
    assert(responses[0]->datetime.tm_mon == 0);
    assert(responses[0]->datetime.tm_mday == 2);
    assert(responses[0]->datetime.tm_hour == 1);
    assert(responses[0]->datetime.tm_min == 23);
    assert(responses[0]->datetime.tm_sec == 0);
    assert(responses[0]->id == L"ID:abc");
    assert(responses[0]->content == L"content");

    s.assign(L"123 2021-01-02 01 ID:abc\ncontent");
    tokens = t.tokenize(s);
    responses = p.parse(tokens);
    assert(responses.size() == 1);
    assert(responses[0]->number == 123);
    assert(responses[0]->name == L"");
    assert(responses[0]->datetime.tm_year == 121);
    assert(responses[0]->datetime.tm_mon == 0);
    assert(responses[0]->datetime.tm_mday == 2);
    assert(responses[0]->datetime.tm_hour == 1);
    assert(responses[0]->datetime.tm_min == 0);
    assert(responses[0]->datetime.tm_sec == 0);
    assert(responses[0]->id == L"ID:abc");
    assert(responses[0]->content == L"content");

    s.assign(L"123 name 2021-01-02 01:23:45\ncontent");
    tokens = t.tokenize(s);
    responses = p.parse(tokens);
    assert(responses.size() == 1);
    assert(responses[0]->number == 123);
    assert(responses[0]->name == L"name");
    assert(responses[0]->datetime.tm_year == 121);
    assert(responses[0]->datetime.tm_mon == 0);
    assert(responses[0]->datetime.tm_mday == 2);
    assert(responses[0]->datetime.tm_hour == 1);
    assert(responses[0]->datetime.tm_min == 23);
    assert(responses[0]->datetime.tm_sec == 45);
    assert(responses[0]->id == L"");
    assert(responses[0]->content == L"content");

    s.assign(L"123 name 2021-01-02 01:23:45");
    tokens = t.tokenize(s);
    responses = p.parse(tokens);
    assert(responses.size() == 1);
    assert(responses[0]->number == 123);
    assert(responses[0]->name == L"name");
    assert(responses[0]->datetime.tm_year == 121);
    assert(responses[0]->datetime.tm_mon == 0);
    assert(responses[0]->datetime.tm_mday == 2);
    assert(responses[0]->datetime.tm_hour == 1);
    assert(responses[0]->datetime.tm_min == 23);
    assert(responses[0]->datetime.tm_sec == 45);
    assert(responses[0]->id == L"");
    assert(responses[0]->content == L"");

    s.assign(L"123 name 2021-01-02");
    tokens = t.tokenize(s);
    responses = p.parse(tokens);
    assert(responses.size() == 1);
    assert(responses[0]->number == 123);
    assert(responses[0]->name == L"name");
    assert(responses[0]->datetime.tm_year == 121);
    assert(responses[0]->datetime.tm_mon == 0);
    assert(responses[0]->datetime.tm_mday == 2);
    assert(responses[0]->datetime.tm_hour == 0);
    assert(responses[0]->datetime.tm_min == 0);
    assert(responses[0]->datetime.tm_sec == 0);
    assert(responses[0]->id == L"");
    assert(responses[0]->content == L"");

    s.assign(L"123 name");
    tokens = t.tokenize(s);
    responses = p.parse(tokens);
    assert(responses.size() == 0);

    s.assign(L"123");
    tokens = t.tokenize(s);
    responses = p.parse(tokens);
    assert(responses.size() == 0);

    s.assign(L"123\n name \n2021-01-02 \r 01:23:45\r\nID:abc\ncontent");
    tokens = t.tokenize(s);
    responses = p.parse(tokens);
    assert(responses.size() == 1);
    assert(responses[0]->number == 123);
    assert(responses[0]->name == L"name");
    assert(responses[0]->datetime.tm_year == 121);
    assert(responses[0]->datetime.tm_mon == 0);
    assert(responses[0]->datetime.tm_mday == 2);
    assert(responses[0]->datetime.tm_hour == 1);
    assert(responses[0]->datetime.tm_min == 23);
    assert(responses[0]->datetime.tm_sec == 45);
    assert(responses[0]->id == L"ID:abc");
    assert(responses[0]->content == L"content");

}
}  // ccres

using namespace ccres;
using namespace std;

int main() {
    test_parser();
    cout << "Done" << endl;
    return 0;
}
