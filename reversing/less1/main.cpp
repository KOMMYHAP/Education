#include <iostream>

int main()
{
	int num = 0;
	std::cin >> num;
	if (!std::cin) {
		std::cin.clear();
	}
	if (num != 42) {
		std::cout << "Bad." << std::endl;
	} else {
		std::cout << "Good." << std::endl;
	}
}