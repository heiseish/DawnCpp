#include <benchmark/benchmark.h>
#include <string>
#include "utility/crypto/encrypt.hpp"

static void SHA256(benchmark::State& state) {
    for (auto _ : state) {
        auto res = Dawn::Utility::sha256(
            "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoij"
            "klmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu");
    }
}
// Register the function as a benchmark
BENCHMARK(SHA256);

// Define another benchmark
static void HMAC_SHA256(benchmark::State& state) {
    for (auto _ : state) {
        auto res = Dawn::Utility::hmac_sha256(
            "key", "The quick brown fox jumps over the lazy dog");
    }
}
BENCHMARK(HMAC_SHA256);

BENCHMARK_MAIN();