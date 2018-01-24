#include "personWorkBuilder.h"

PersonWorkBuilder::PersonWorkBuilder(Person &person_ref) noexcept
	: PersonBuilder{ person_ref } 
{}

PersonWorkBuilder& 
PersonWorkBuilder::in(std::string const &company_name)
{
	_person_ref._company_name = company_name;
	return *this;
}

PersonWorkBuilder& 
PersonWorkBuilder::earning(int annual_income)
{
	_person_ref._annual_income = annual_income;
	return *this;
}

PersonWorkBuilder& 
PersonWorkBuilder::as_a(std::string const &position)
{
	_person_ref._position = position;
	return *this;
}