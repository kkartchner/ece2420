#include <iostream>
#include "Head.hpp"
#include <mutex>
#include <memory>
#include <thread>
#include <unistd.h>

#define NUM_HEADS 8

uint32_t array1[NUM_HEADS] = {0};
uint32_t array2[NUM_HEADS] = {0};

// Expectation: All values in array1 should equal 2*numIterations
// Verifies thread safety
void modArray1(int i)
{
	int temp = array1[i];
	usleep(1000);
	array1[i] = temp + 1;

	temp = array1[(i + 1) % NUM_HEADS];
	usleep(1000);
	array1[(i + 1) % NUM_HEADS] = temp + 1;
}

// Expectation: Stolchastic behavior in array2
// Verifies optimal concurrency
void modArray2(int i)
{
	for(int j = 0; j < NUM_HEADS; j++)
	{
		int temp = array2[j];
		usleep(1000);
		array2[j] = temp + 1;
	}
}

void test(std::function<void(int)> mod, uint32_t* array) {
	std::mutex mtx;
	std::vector<std::shared_ptr<Head> > headVec = Head::makePlatter(NUM_HEADS);

	//function to run as thread
	std::function<void(int i)> func = [&headVec, &mtx, mod](int i){
		for(int j = 0; j < 1000; j++)
		{
			mtx.lock();
			std::cout << "Iteration: " << j << std::endl;
			mtx.unlock();
			headVec[i]->execute([i, mod](){ mod(i); });
		}
	};

	std::vector<std::thread> tVec;
	for(int i = 0; i < NUM_HEADS; ++i)
	{
		tVec.emplace_back(func, i);
	}

	for(auto it = tVec.begin(); it != tVec.end(); ++it) {
		it->join();
	}

	for(int i = 0; i < NUM_HEADS; i++) {
		std::cout << "Idx: " << i << " Val: " << array[i] << std::endl;
	}
}

int main() {
	test(modArray1, array1);
	test(modArray2, array2);
}
