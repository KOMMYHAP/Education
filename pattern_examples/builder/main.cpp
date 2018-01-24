#include "person.h"

int main()
{
	Person p = Person::create()
		.works()
			.in("The Coca-Cola Company")
			.earning(42)
			.as_a("cleaner")
		.lives()
			.in("Russia")
			.at("Bluhera 32/1")
			.with_post_code("M630073");
}