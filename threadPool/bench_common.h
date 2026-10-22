#pragma once
#include "threadpool.h"
#include <chrono>
#include <atomic>
#include <iostream>
#include <string>

// 通用压测：给定任务工厂，跑1/2/4/8 worker
template <typename JobFactory>
void runBenchmark(const std::string& name, int taskCount, JobFactory makeJob) {
    std::cout << "=== " << name << " (tasks=" << taskCount << ") ===\n";
    for (size_t n : {1, 2, 4, 8}) {
        std::atomic<int> counter{ 0 };
        auto job = makeJob(counter);          // 每个bench自己定义任务长什么样

        auto start = std::chrono::steady_clock::now();
        {
            ThreadPool pool(n);
            for (int i = 0; i < taskCount; i++) pool.submit(job);
        }
        auto end = std::chrono::steady_clock::now();
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

        std::cout << "workers=" << n << " 耗时=" << ms
            << "ms counter=" << counter.load() << "\n";
    }
}