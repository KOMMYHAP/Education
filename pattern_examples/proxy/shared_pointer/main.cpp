#include <cassert>
#include "shared_pointer.h"

int main()
{
	shared_pointer<int> p1{};
	assert(p1.use_count() == 0);

	shared_pointer<int> p2(new int(42));
	assert(p2.use_count() == 1);
	assert(*p2 == 42);

	p1 = p2;
	assert(p1.use_count() == p2.use_count());
	assert(*p1 == *p2);

	return 0;
}