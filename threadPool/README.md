# ThreadPool

A C++20 thread pool: fixed workers + task queue + graceful shutdown.
学习项目：手写线程池（互斥锁 + 条件变量 + future）

## Features
- submit() accepts any callable, returns std::future for async result
- Graceful shutdown: workers drain the queue before exit
- Header + implementation split, CMake build

## Build & Run
```bash
g++ -std=c++20 -pthread threadpool.cpp main.cpp -o pool
./pool        # expect output: 338350
