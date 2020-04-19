#include <csignal>
#include "core/app.hpp"
#include "trading/binance_trader.hpp"
#include "trading/trader_manager.hpp"
#include "utility/general/logging.hpp"
#include "utility/general/main_flags.hpp"
namespace {

using namespace Dawn;

}
int main(int argc, char** argv) {
    // -------- House keeping funcs -----------------------
    InitArgs(argc, argv);
    InitLogger();
    signal(SIGINT, Core::dawn_signal_handler);
    signal(SIGABRT, Core::dawn_signal_handler);
    signal(SIGSEGV, Core::dawn_signal_handler);
    // --------------------------------------
    Trading::BinanceTraderConfig config_;
    Trading::BinanceTrader trader(config_);
    trader.Initialize();
    return EXIT_SUCCESS;
}