#include "bench_common.h"

int main() {
    runBenchmark("empty-task", 100000, [](std::atomic<int>& c) {
        return [&c] { c++; };
    });
}