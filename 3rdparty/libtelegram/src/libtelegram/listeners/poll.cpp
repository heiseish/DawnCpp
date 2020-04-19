#include "libtelegram/listeners/poll.h"

namespace telegram {

namespace listener {

bool poll::signal_handler_is_set = false;
std::atomic_flag poll::keep_running_global;

poll::poll(telegram::sender &this_sender, unsigned int this_poll_timeout)
    : sender(this_sender), poll_timeout(this_poll_timeout) {
    /// Default constructor
}

void poll::run() {
    /// Execute the telegram long-polling loop listener service
    try {
        keep_running.test_and_set();
        keep_running_global.test_and_set();
        set_signal_handler();
        boost::asio::io_context context;  // use asio's io_context to provide a
                                          // thread pool work queue
        boost::asio::executor_work_guard work{boost::asio::make_work_guard(
            context)};  // prevent the threads from running out of work
        std::vector<std::thread> threads;
        threads.reserve(num_threads);
        for (unsigned int i = 0; i != num_threads;
             ++i) {  // initialise the thread pool
            threads.emplace_back([&] { context.run(); });
        }
        std::cout << "LibTelegram: Poll listener: Started " << threads.size()
                  << " worker thread" << plural(threads.size()) << "."
                  << std::endl;

        int offset = 0;  // keep track of the last received update offset
        while (keep_running.test_and_set() &&
               keep_running_global
                   .test_and_set()) {  // the poller always runs sequentially,
                                       // single-threaded
            try {
                nlohmann::json tree;
                tree["offset"] = offset;
                // tree["limit"] = 100;
                tree["timeout"] = poll_timeout;
                auto reply_tree(
                    sender.send_json("getUpdates", tree, poll_timeout));
                for (auto const &it :
                     reply_tree["result"]) {  // process each reply entry
                                              // individually
#ifndef NDEBUG
                    std::cerr << it.dump(2) << std::endl;
#endif  // NDEBUG
                    int const update_id = it["update_id"];
                    if (update_id != 0) {
                        offset = std::max(update_id + 1, offset);
                    }
                    boost::asio::post(
                        context,
                        [this, subtree = it] {  // pass a copy of the subtree
                            execute_callbacks(subtree);
                        });
                }
            } catch (std::exception const
                         &e) {  // catch any exceptions from the poll
                std::cerr << "LibTelegram: Exception while trying to "
                             "getUpdates, cannot continue: "
                          << e.what() << std::endl;
                stop_all();
            }
        }
        unset_signal_handler();
        context.stop();
        std::cout << "LibTelegram: Poll listener: Harvesting " << threads.size()
                  << " worker thread" << plural(threads.size()) << "...";
        for (auto &it : threads) {  // close down the thread pool
            it.join();
        }
        std::cout << " Finished." << std::endl;
    } catch (boost::system::system_error const &
                 se) {  // this is the type of error thrown by the fcgi library.
        std::cerr << "LibTelegram: System error: " << se.what() << std::endl;
    } catch (std::exception const &e) {  // catch any other exceptions
        std::cerr << "LibTelegram: Exception: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "LibTelegram: Uncaught exception!" << std::endl;
    }
}

void poll::stop() {
    /// Request that the listener currently running stops gracefully, at the end
    /// of its current long poll. It is only meaningful to call this from
    /// another thread while poll::run() is running.
    keep_running.clear();
}
void poll::stop_all() {
    /// Request that the listener currently running stops gracefully, at the end
    /// of its current long poll. It is only meaningful to call this from
    /// another thread or signal handler while poll::run() is running. Note that
    /// this will stop ALL polling listeners globally.
    keep_running_global.clear();
}

void poll::set_signal_handler() {
/// Set a signal handler to catch ctrl-c in the console and close gracefully
#ifndef TELEGRAM_NO_SIGNAL_HANDLER
    struct sigaction signal;
    signal.sa_handler = signal_handler;
    sigemptyset(&signal.sa_mask);
    signal.sa_flags = 0;
    sigaction(SIGINT, &signal, NULL);
    signal_handler_is_set = true;
#endif  // TELEGRAM_NO_SIGNAL_HANDLER
}
void poll::unset_signal_handler() {
/// Unset any signal handler we set earlier
#ifndef TELEGRAM_NO_SIGNAL_HANDLER
    struct sigaction signal;
    signal.sa_handler = SIG_DFL;
    sigemptyset(&signal.sa_mask);
    signal.sa_flags = 0;
    sigaction(SIGINT, &signal, NULL);
    signal_handler_is_set = false;
#endif  // TELEGRAM_NO_SIGNAL_HANDLER
}
bool poll::is_signal_handler_set() {
    /// Report whether our signal handler is currently set
    return signal_handler_is_set;
}

}  // namespace listener
}  // namespace telegram

void signal_handler(int s) {
    /// Signal handler for ctrl-c events
    std::cout << std::endl;
    std::cout << "LibTelegram: Poll listener: Caught signal " << s
              << ", terminating after this poll..." << std::endl;
    telegram::listener::poll::stop_all();
    telegram::listener::poll::
        unset_signal_handler();  // unset the signal handler, so a second ctrl-c
                                 // will exit immediately
}
