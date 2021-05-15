#include <tests/tests.h>

namespace ccres {

static void test_parser() {
    setlocale(LC_ALL, "");
    auto t = Tokenizer();
    auto p = Parser((Options) {
        .json = false,
        .need_number = true,
        .need_name = true,
        .need_date = true,
        .need_youbi = true,
        .need_time = true,
        .need_id = true,
    });
    auto s = String();
    std::vector<std::shared_ptr<Token>> tokens;
    std::vector<std::shared_ptr<Response>> responses;

    p = Parser((Options) {
        .json = false,
        .need_number = true,
        .need_name = true,
        .need_date = true,
        .need_youbi = false,
        .need_time = true,
        .need_id = true,
    });
    s.assign(L"10\n名無しのプログラマー\n2021年05月15日01:05\nID:abc123\nabc\ndef");
    tokens = t.tokenize(s);
    responses = p.parse(tokens);
    assert(responses.size() == 1);
    assert(responses[0]->number == 10);
    assert(responses[0]->name == L"名無しのプログラマー");
    assert(responses[0]->datetime.tm_year == 121);
    assert(responses[0]->datetime.tm_mon == 4);
    assert(responses[0]->datetime.tm_mday == 15);
    assert(responses[0]->datetime.tm_wday == -1);
    assert(responses[0]->datetime.tm_hour == 1);
    assert(responses[0]->datetime.tm_min == 5);
    assert(responses[0]->datetime.tm_sec == 0);
    assert(responses[0]->id == L"ID:abc123");
    assert(responses[0]->content == L"abc\ndef");

    p = Parser((Options) {
        .json = false,
        .need_number = true,
        .need_name = true,
        .need_date = true,
        .need_youbi = true,
        .need_time = true,
        .need_id = true,
    });
    s.assign(L"123 name 2021年02月03日 (月) 01:23:45 ID:abc\ncontent");
    tokens = t.tokenize(s);
    responses = p.parse(tokens);
    assert(responses.size() == 1);
    assert(responses[0]->number == 123);
    assert(responses[0]->name == L"name");
    assert(responses[0]->datetime.tm_year == 121);
    assert(responses[0]->datetime.tm_mon == 1);
    assert(responses[0]->datetime.tm_mday == 3);
    assert(responses[0]->datetime.tm_wday == 1);
    assert(responses[0]->datetime.tm_hour == 1);
    assert(responses[0]->datetime.tm_min == 23);
    assert(responses[0]->datetime.tm_sec == 45);
    assert(responses[0]->id == L"ID:abc");
    assert(responses[0]->content == L"content");

    p = Parser((Options) {
        .json = false,
        .need_number = true,
        .need_name = true,
        .need_date = true,
        .need_youbi = true,
        .need_time = true,
        .need_id = false,
    });
    s.assign(L"123 name 2021-02-03 (月) 01:23:45\ncontent");
    tokens = t.tokenize(s);
    responses = p.parse(tokens);
    assert(responses.size() == 1);
    assert(responses[0]->number == 123);
    assert(responses[0]->name == L"name");
    assert(responses[0]->datetime.tm_year == 121);
    assert(responses[0]->datetime.tm_mon == 1);
    assert(responses[0]->datetime.tm_mday == 3);
    assert(responses[0]->datetime.tm_wday == 1);
    assert(responses[0]->datetime.tm_hour == 1);
    assert(responses[0]->datetime.tm_min == 23);
    assert(responses[0]->datetime.tm_sec == 45);
    assert(responses[0]->id == L"");
    assert(responses[0]->content == L"content");

    p = Parser((Options) {
        .json = false,
        .need_number = true,
        .need_name = true,
        .need_date = true,
        .need_youbi = true,
        .need_time = false,
        .need_id = true,
    });
    s.assign(L"123 name 2021-02-03 (月) ID:abc\ncontent");
    tokens = t.tokenize(s);
    responses = p.parse(tokens);
    assert(responses.size() == 1);
    assert(responses[0]->number == 123);
    assert(responses[0]->name == L"name");
    assert(responses[0]->datetime.tm_year == 121);
    assert(responses[0]->datetime.tm_mon == 1);
    assert(responses[0]->datetime.tm_mday == 3);
    assert(responses[0]->datetime.tm_wday == 1);
    assert(responses[0]->datetime.tm_hour == 0);
    assert(responses[0]->datetime.tm_min == 0);
    assert(responses[0]->datetime.tm_sec == 0);
    assert(responses[0]->id == L"ID:abc");
    assert(responses[0]->content == L"content");

    p = Parser((Options) {
        .json = false,
        .need_number = true,
        .need_name = true,
        .need_date = true,
        .need_youbi = false,
        .need_time = true,
        .need_id = true,
    });
    s.assign(L"123 name 2021-02-03 01:23:45 ID:abc\ncontent");
    tokens = t.tokenize(s);
    responses = p.parse(tokens);
    assert(responses.size() == 1);
    assert(responses[0]->number == 123);
    assert(responses[0]->name == L"name");
    assert(responses[0]->datetime.tm_year == 121);
    assert(responses[0]->datetime.tm_mon == 1);
    assert(responses[0]->datetime.tm_mday == 3);
    assert(responses[0]->datetime.tm_wday == -1);
    assert(responses[0]->datetime.tm_hour == 1);
    assert(responses[0]->datetime.tm_min == 23);
    assert(responses[0]->datetime.tm_sec == 45);
    assert(responses[0]->id == L"ID:abc");
    assert(responses[0]->content == L"content");

    p = Parser((Options) {
        .json = false,
        .need_number = true,
        .need_name = true,
        .need_date = false,
        .need_youbi = true,
        .need_time = true,
        .need_id = true,
    });
    s.assign(L"123 name (月) 01:23:45 ID:abc\ncontent");
    tokens = t.tokenize(s);
    responses = p.parse(tokens);
    assert(responses.size() == 1);
    assert(responses[0]->number == 123);
    assert(responses[0]->name == L"name");
    assert(responses[0]->datetime.tm_year == 0);
    assert(responses[0]->datetime.tm_mon == 0);
    assert(responses[0]->datetime.tm_mday == 0);
    assert(responses[0]->datetime.tm_wday == 1);
    assert(responses[0]->datetime.tm_hour == 1);
    assert(responses[0]->datetime.tm_min == 23);
    assert(responses[0]->datetime.tm_sec == 45);
    assert(responses[0]->id == L"ID:abc");
    assert(responses[0]->content == L"content");

    p = Parser((Options) {
        .json = false,
        .need_number = true,
        .need_name = false,
        .need_date = true,
        .need_youbi = true,
        .need_time = true,
        .need_id = true,
    });
    s.assign(L"123 2021-01-02 (月) 01:23:45 ID:abc\ncontent");
    tokens = t.tokenize(s);
    responses = p.parse(tokens);
    assert(responses.size() == 1);
    assert(responses[0]->number == 123);
    assert(responses[0]->name == L"");
    assert(responses[0]->datetime.tm_year == 121);
    assert(responses[0]->datetime.tm_mon == 0);
    assert(responses[0]->datetime.tm_mday == 2);
    assert(responses[0]->datetime.tm_wday == 1);
    assert(responses[0]->datetime.tm_hour == 1);
    assert(responses[0]->datetime.tm_min == 23);
    assert(responses[0]->datetime.tm_sec == 45);
    assert(responses[0]->id == L"ID:abc");
    assert(responses[0]->content == L"content");

    p = Parser((Options) {
        .json = false,
        .need_number = false,
        .need_name = true,
        .need_date = true,
        .need_youbi = true,
        .need_time = true,
        .need_id = true,
    });
    s.assign(L"name 2021-01-02 (月) 01:23:45 ID:abc\ncontent");
    tokens = t.tokenize(s);
    responses = p.parse(tokens);
    assert(responses.size() == 1);
    assert(responses[0]->number == 0);
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

    p = Parser((Options) {
        .json = false,
        .need_number = true,
        .need_name = true,
        .need_date = true,
        .need_youbi = true,
        .need_time = true,
        .need_id = true,
    });
    s.assign(L"123 name 2021-01-02 (月) 01:23:45 ID:abc\ncontent");
    tokens = t.tokenize(s);
    responses = p.parse(tokens);
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

    p = Parser((Options) {
        .json = false,
        .need_number = true,
        .need_name = true,
        .need_date = true,
        .need_youbi = true,
        .need_time = true,
        .need_id = true,
    });
    s.assign(L"123: name 2021-01-02 (月) 01:23:45 ID:abc\ncontent");
    tokens = t.tokenize(s);
    responses = p.parse(tokens);
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

    p = Parser((Options) {
        .json = false,
        .need_number = true,
        .need_name = true,
        .need_date = true,
        .need_youbi = true,
        .need_time = true,
        .need_id = true,
    });
    s.assign(L"123 : name 2021-01-02 (月) 01:23:45 ID:abc\ncontent");
    tokens = t.tokenize(s);
    responses = p.parse(tokens);
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

    p = Parser((Options) {
        .json = false,
        .need_number = true,
        .need_name = true,
        .need_date = true,
        .need_youbi = true,
        .need_time = true,
        .need_id = true,
    });
    s.assign(L"123 ： name 2021-01-02 (月) 01:23:45 ID:abc\ncontent");
    tokens = t.tokenize(s);
    responses = p.parse(tokens);
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

    p = Parser((Options) {
        .json = false,
        .need_number = true,
        .need_name = true,
        .need_date = true,
        .need_youbi = true,
        .need_time = true,
        .need_id = true,
    });
    s.assign(L"123 name 2021-01 (月) 01:23:45 ID:abc\ncontent");
    tokens = t.tokenize(s);
    responses = p.parse(tokens);
    assert(responses.size() == 1);
    assert(responses[0]->number == 123);
    assert(responses[0]->name == L"name");
    assert(responses[0]->datetime.tm_year == 121);
    assert(responses[0]->datetime.tm_mon == 0);
    assert(responses[0]->datetime.tm_mday == 0);
    assert(responses[0]->datetime.tm_wday == 1);
    assert(responses[0]->datetime.tm_hour == 1);
    assert(responses[0]->datetime.tm_min == 23);
    assert(responses[0]->datetime.tm_sec == 45);
    assert(responses[0]->id == L"ID:abc");
    assert(responses[0]->content == L"content");

    p = Parser((Options) {
        .json = false,
        .need_number = true,
        .need_name = true,
        .need_date = true,
        .need_youbi = true,
        .need_time = true,
        .need_id = true,
    });
    s.assign(L"123 name 2021 (月) 01:23:45 ID:abc\ncontent");
    tokens = t.tokenize(s);
    responses = p.parse(tokens);
    assert(responses.size() == 1);
    assert(responses[0]->number == 123);
    assert(responses[0]->name == L"name");
    assert(responses[0]->datetime.tm_year == 121);
    assert(responses[0]->datetime.tm_mon == 0);
    assert(responses[0]->datetime.tm_mday == 0);
    assert(responses[0]->datetime.tm_wday == 1);
    assert(responses[0]->datetime.tm_hour == 1);
    assert(responses[0]->datetime.tm_min == 23);
    assert(responses[0]->datetime.tm_sec == 45);
    assert(responses[0]->id == L"ID:abc");
    assert(responses[0]->content == L"content");

    p = Parser((Options) {
        .json = false,
        .need_number = true,
        .need_name = false,
        .need_date = true,
        .need_youbi = true,
        .need_time = true,
        .need_id = true,
    });
    s.assign(L"123 2021-01-02 (月) 01:23:45 ID:abc\ncontent");
    tokens = t.tokenize(s);
    responses = p.parse(tokens);
    assert(responses.size() == 1);
    assert(responses[0]->number == 123);
    assert(responses[0]->name == L"");
    assert(responses[0]->datetime.tm_year == 121);
    assert(responses[0]->datetime.tm_mon == 0);
    assert(responses[0]->datetime.tm_mday == 2);
    assert(responses[0]->datetime.tm_wday == 1);
    assert(responses[0]->datetime.tm_hour == 1);
    assert(responses[0]->datetime.tm_min == 23);
    assert(responses[0]->datetime.tm_sec == 45);
    assert(responses[0]->id == L"ID:abc");
    assert(responses[0]->content == L"content");

    p = Parser((Options) {
        .json = false,
        .need_number = true,
        .need_name = true,
        .need_date = false,
        .need_youbi = true,
        .need_time = true,
        .need_id = true,
    });
    s.assign(L"123 name (月) 01:23:45 ID:abc\ncontent");
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

    p = Parser((Options) {
        .json = false,
        .need_number = true,
        .need_name = false,
        .need_date = true,
        .need_youbi = true,
        .need_time = true,
        .need_id = true,
    });
    s.assign(L"123 2021-01-02 (月) 01:23:45 ID:abc\ncontent");
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

    p = Parser((Options) {
        .json = false,
        .need_number = true,
        .need_name = false,
        .need_date = true,
        .need_youbi = true,
        .need_time = true,
        .need_id = true,
    });
    s.assign(L"123 2021-01-02 (月) 01:23 ID:abc\ncontent");
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

    p = Parser((Options) {
        .json = false,
        .need_number = true,
        .need_name = false,
        .need_date = true,
        .need_youbi = true,
        .need_time = true,
        .need_id = true,
    });
    s.assign(L"123 2021-01-02 (月) 01 ID:abc\ncontent");
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

    p = Parser((Options) {
        .json = false,
        .need_number = true,
        .need_name = true,
        .need_date = true,
        .need_youbi = true,
        .need_time = true,
        .need_id = false,
    });
    s.assign(L"123 name 2021-01-02 (月) 01:23:45\ncontent");
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

    p = Parser((Options) {
        .json = false,
        .need_number = true,
        .need_name = true,
        .need_date = true,
        .need_youbi = true,
        .need_time = true,
        .need_id = false,
    });
    s.assign(L"123 name 2021-01-02 (月) 01:23:45");
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

    p = Parser((Options) {
        .json = false,
        .need_number = true,
        .need_name = true,
        .need_date = true,
        .need_youbi = false,
        .need_time = false,
        .need_id = false,
    });
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

    p = Parser((Options) {
        .json = false,
        .need_number = true,
        .need_name = true,
        .need_date = false,
        .need_youbi = false,
        .need_time = false,
        .need_id = false,
    });
    s.assign(L"123 name");
    tokens = t.tokenize(s);
    responses = p.parse(tokens);
    assert(responses.size() == 1);
    assert(responses[0]->number == 123);
    assert(responses[0]->name == L"name");

    p = Parser((Options) {
        .json = false,
        .need_number = true,
        .need_name = false,
        .need_date = false,
        .need_youbi = false,
        .need_time = false,
        .need_id = false,
    });
    s.assign(L"123");
    tokens = t.tokenize(s);
    responses = p.parse(tokens);
    assert(responses.size() == 1);
    assert(responses[0]->number == 123);

    p = Parser((Options) {
        .json = false,
        .need_number = true,
        .need_name = true,
        .need_date = true,
        .need_youbi = true,
        .need_time = true,
        .need_id = true,
    });
    s.assign(L"123\n name \n2021-01-02 \n (月) \r 01:23:45\r\nID:abc\ncontent");
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

    /* mugen loop
    p = Parser((Options) {
        .json = false,
        .need_number = false,
        .need_name = false,
        .need_date = false,
        .need_youbi = false,
        .need_time = false,
        .need_id = false,
    });
    s.assign(L"123\n name \n2021-01-02 \n (月) \r 01:23:45\r\nID:abc\ncontent");
    tokens = t.tokenize(s);
    responses = p.parse(tokens);
    assert(responses.size() == 19);  // omg
    */
}
}  // ccres

using namespace ccres;
using namespace std;

int main() {
    test_parser();
    cout << "Done" << endl;
    return 0;
}
