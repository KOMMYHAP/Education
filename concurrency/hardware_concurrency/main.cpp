#include <iostream>
#include <thread>

int main()
{
	auto const n = std::thread::hardware_concurrency();
	std::cout << n << " concurrent threads are supported." << std::endl;
	return 0;
}