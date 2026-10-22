#include "threadpool.h"

ThreadPool::ThreadPool(size_t numThreads)
{
	for (int i = 0; i < numThreads; i++)
	{
		workers.emplace_back([this] {workerLoop(); });
	}
}

ThreadPool::~ThreadPool()
{
	{
		std::unique_lock<std::mutex>lock(mtx);
		stop = true;
	}
	cv.notify_all();
}

void ThreadPool::submit(std::function<void()> task)
{
	{
		std::unique_lock<std::mutex> lock(mtx);
		tasks.push(std::move(task));
	}
	cv.notify_one();
}

void ThreadPool::workerLoop()
{
	while (true){
		std::function<void()> task;
		{
			std::unique_lock<std::mutex> lock(mtx);
			cv.wait(lock, [this] { return !tasks.empty() || stop; });//谓词
			if (stop&&tasks.empty()) return;//退出
			task = std::move(tasks.front());//取任务
			tasks.pop();
		}
		task();
	}
}
