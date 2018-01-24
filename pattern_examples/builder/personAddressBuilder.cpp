#include "personAddressBuilder.h"

PersonAddressBuilder::PersonAddressBuilder(Person &person_ref) noexcept
	: PersonBuilder{ person_ref } 
{}

PersonAddressBuilder& 
PersonAddressBuilder::at(std::string const &street_address)
{
	_person_ref._street_address = street_address;
	return *this;
}

PersonAddressBuilder& 
PersonAddressBuilder::with_post_code(std::string const &post_code)
{
	_person_ref._post_code = post_code;
	return *this;
}

PersonAddressBuilder& 
PersonAddressBuilder::in(std::string const &city)
{
	_person_ref._city = city;
	return *this;
}