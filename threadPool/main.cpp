#include<iostream>
#include"threadpool.h"

int main()
{
	{
		ThreadPool pool(4);
		for (int i = 0; i < 8; i++)
		{
			pool.submit([i] {std::cout << "task" << i << std::endl; });
		}
		std::cout << "submit all" << std::endl;
	}
	return 0;
}