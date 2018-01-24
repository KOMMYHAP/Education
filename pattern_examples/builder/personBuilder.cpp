#include "personBuilder.h"
	
PersonBuilder::PersonBuilder(Person &person_ref) noexcept
	: _person_ref{ person_ref } 
{}

PersonBuilder::PersonBuilder()  noexcept
	: _person_ref{ _person } 
{}

PersonBuilder::operator Person() noexcept
{ 
	return std::move(_person_ref);
}

PersonWorkBuilder PersonBuilder::works()
{
	return PersonWorkBuilder{ _person_ref };
}

PersonAddressBuilder PersonBuilder::lives()
{
	return PersonAddressBuilder{ _person_ref };
}