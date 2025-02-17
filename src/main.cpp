#include <csignal>
#include "core/app.hpp"
#include "utility/general/logging.hpp"
#include "utility/general/main_flags.hpp"

#include <iostream>

namespace {

using namespace Dawn;

}
int main(int argc, char** argv)
{
    // -------- House keeping funcs -----------------------
    InitArgs(argc, argv);
    InitLogger();
    signal(SIGINT, Core::dawn_signal_handler);
    signal(SIGABRT, Core::dawn_signal_handler);
    signal(SIGSEGV, Core::dawn_signal_handler);
    // --------------------------------------
    Core::Application app;
    if (!app.Initialize()) {
        return EXIT_FAILURE;
    }
    app.Start();
    return EXIT_SUCCESS;
}
