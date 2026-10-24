#include<iostream>
#include"threadpool.h"
#include<chrono>
#include<atomic>

int main()
{
	ThreadPool pool(4);
	std::vector<std::future<int>> futs;
	for (int i = 1; i <= 100; i++) {
		futs.push_back(pool.submit([i] { return i * i; }));
	}
	int sum = 0;
	for (auto& f : futs) sum += f.get();
	std::cout << sum << std::endl;
}