#include <iostream>
#include "ThreadBarrier.h"
#include <vector>
#include <random>
int main()
{
	size_t threadCount;
	std::cout << "Enter thread count:";
	std::cin >> threadCount;
	std::vector<std::thread> threads;
	threads.reserve(threadCount);
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> UID(0, 500);
	ThreadBarrier barrier(threadCount);
	for (size_t i = 0; i < threadCount; i++)
		threads.emplace_back([&]() 
			{
			std::this_thread::sleep_for(std::chrono::milliseconds(UID(gen)));
			std::cout << std::this_thread::get_id() << " start\n";
			barrier.wait();
			std::cout << std::this_thread::get_id() << " ended\n";
			}
	);
	for (auto& it : threads)
	{
		if (it.joinable())
		{
			const std::thread::id id = it.get_id();
			it.join();
			std::cout << id << " joined\n";
		}
	}
}

