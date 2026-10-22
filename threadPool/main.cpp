#include<iostream>
#include"threadpool.h"
#include<chrono>
#include<atomic>

int main()
{
	std::atomic<int> counter{ 0 };
	auto job = [&counter] { std::this_thread::sleep_for
	(std::chrono::milliseconds(1));counter++; };
	for (size_t n : {1, 2, 4, 8})
	{
		counter = 0;
		auto start = std::chrono::steady_clock::now();

		{
			ThreadPool pool(n);
			for (int i = 0; i < 1000; i++)
			{
				pool.submit(job);
			}
		}
		auto end = std::chrono::steady_clock::now();

		auto ms = std::chrono::duration_cast<std::chrono::milliseconds>
			(end-start).count();

		std::cout << "workers = " << n << " 耗时 = " << ms 
			<< " counter: "<<counter.load()<<std::endl;
	}
	return 0;
}