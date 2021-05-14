#include <tests/tests.h>

namespace ccres {

static void test_parser() {
    auto t = Tokenizer();
    auto p = Parser();
    auto s = String();

    s.assign(L"123 name 2021-01-02 01:23:45 ID:abc\ncontent");
    auto tokens = t.tokenize(s);
    auto responses = p.parse(tokens);
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
