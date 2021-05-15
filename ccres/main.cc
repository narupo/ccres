#include <ccres/main.h>

int main(int argc, char *argv[]) {
    auto app = new ccres::App(argc, argv);
    int result = app->run();
    delete app;
    return result;
}
