#include<iostream>
#include<mutex>
#include<thread>
#include<queue>
#include<condition_variable>
#include<string>
#include<atomic>

std::queue<int> q;
std::mutex gLock;
std::mutex pLock;
std::queue<std::string> s;
std::condition_variable cv;
std::condition_variable cv_p;
static std::atomic<int> hasWorked = 0;
static int messageSendCount = 0;


void producer(int min, int max)
{
	for (int i = min; i <= max; i++)
	{
		std::unique_lock<std::mutex> lock(gLock);
		q.push(i);
		lock.unlock();
		cv.notify_one();
	}
	hasWorked++;
	{
		std::unique_lock<std::mutex> lock(pLock);
		s.push("该生产者任务已完成");
	}
	cv_p.notify_one();
	cv.notify_all();
}

void comsumer()
{
	int value = -1;
	while (true)
	{
		{
			std::unique_lock<std::mutex> lock(gLock);
			cv.wait(lock, [] { return !q.empty() || hasWorked.load() == 2; });
			if (hasWorked.load() == 2 && q.empty()) return;
			value = q.front();
			q.pop();
		}
		{
			std::unique_lock<std::mutex> lock(pLock);
			s.push("消费：" + std::to_string(value));
		}
		cv_p.notify_all();
	}

}

void print()
{
	while (true)
	{
		std::string str;
		{
			std::unique_lock<std::mutex> lock(pLock);
			cv_p.wait(lock, [] {return !s.empty() || messageSendCount == 20002; });
			if (messageSendCount == 20002) return;
			str = std::move(s.front());
			s.pop();
		}
		std::cout << str << std::endl;
		messageSendCount++;
	}
}

int main()
{
	std::thread p1(producer, 1, 10000);
	std::thread p2(producer, 10001, 20000);
	std::thread c1(comsumer);
	std::thread c2(comsumer);
	std::thread d(print);
	p1.join();
	p2.join();
	c1.join();
	c2.join();
	d.join();
}