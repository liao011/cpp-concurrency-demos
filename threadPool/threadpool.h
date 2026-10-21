#pragma once
#include<thread>
#include<mutex>
#include<condition_variable>
#include<queue>
#include<vector>
#include<functional>

class ThreadPool
{
public:
	//循环创建jthread
	explicit ThreadPool(size_t numThreads);
	//结束线程,停workers
	~ThreadPool();
	//入队+notify
	void submit(std::function<void()>task);
	//禁止拷贝
	ThreadPool(const ThreadPool&) = delete;
	//禁止赋值
	ThreadPool& operator=(const ThreadPool&) = delete;
private:
	//worker的循环体
	void workerLoop();

	std::queue<std::function<void()>>tasks;
	std::mutex mtx;
	std::condition_variable cv;
	bool stop = false;
	std::vector<std::jthread>workers;
};