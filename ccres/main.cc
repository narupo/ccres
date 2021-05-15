#include <ccres/main.h>

int main(int argc, char *argv[]) {
    setlocale(LC_ALL, "");
    auto app = new ccres::App(argc, argv);
    int result = app->run();
    delete app;
    return result;
}
