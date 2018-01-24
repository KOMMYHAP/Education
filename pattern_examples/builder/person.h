#ifndef PERSON_H
#define PERSON_H

#include <string>
#include <utility> // for std::move

class PersonBuilder;

class Person
{
private: // some fields:
	// work info:
	std::string _company_name, _position;
	int _annual_income = 0;

	// personal info:
	std::string _street_address, _post_code, _city;

private: // private c-tor
	// for representation of builder pattern power
	Person() = default;
public:
	static PersonBuilder create();

	friend class PersonBuilder;
	friend class PersonAddressBuilder;
	friend class PersonWorkBuilder;
};

#include "personBuilder.h"

#endif // PERSON_H