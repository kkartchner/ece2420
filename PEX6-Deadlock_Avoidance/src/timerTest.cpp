#include "Head.hpp"
#include <iostream>
#include <thread>
#include <mutex>
#include <unistd.h>

#define HEADS 7

int main() {
	std::mutex mtx;

	std::vector<std::shared_ptr<std::thread>> threads;
	std::vector<std::shared_ptr<Head>> heads = Head::makePlatter(HEADS);

	std::thread grouper([&mtx]() {
		for (int i = 0; i < 2 * HEADS; ++i)	{
			{
				std::unique_lock<std::mutex> lock(mtx);
				std::cout
					<< "Time: " << i << " seconds" << std::endl;
			}
			sleep(1);
		}
		{
			std::unique_lock<std::mutex> lock(mtx);
			std::cout
				<< "Time: " << 2 * HEADS << " seconds" << std::endl;
		}
	});

	for (int i = 0; i < HEADS; ++i) {
		threads.push_back(std::make_shared<std::thread>([i, &heads, &mtx]() {
			heads[i]->execute([i, &mtx]() {
				{
					std::unique_lock<std::mutex> lock(mtx);
					std::cout << "\tHead " << i << std::endl;
				}
				sleep(2);
			});
		}));
	}

	for (int i = 0; i < HEADS; ++i) {
		threads[i]->join();
	}

	grouper.join();

	return 0;
}
