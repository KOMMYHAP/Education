#include "test.h"

int main()
{
	if (test() != 0) {
		printf("Test was not completed successfully.");
		return 1;
	}
	return 0;
}