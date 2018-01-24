#include <thread>
#include <iostream>
#include <chrono>

void test()
{
	std::cout << "[started]" << std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(2));
	std::cout << "[completed]" << std::endl;
}

int main()
{
	std::thread t(test);

	std::cout << "Before join." << std::endl;

	t.join();
	std::cout << "After join." << std::endl;

	if (t.joinable()) {
		t.detach();
		std::cout << "After detach." << std::endl;
	}
}