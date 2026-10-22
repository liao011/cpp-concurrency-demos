#include "bench_common.h"
#include <chrono>

int main() {
    runBenchmark("long-task (1ms sleep)", 1000, [](std::atomic<int>& c) {
        return [&c] {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            c++;
        };
    });
}