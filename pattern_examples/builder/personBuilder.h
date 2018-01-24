#ifndef PERSON_BUILDER_H
#define PERSON_BUILDER_H

#include "person.h"

class PersonWorkBuilder;
class PersonAddressBuilder;

class PersonBuilder
{
private:
	Person _person;
protected:
	Person &_person_ref;

	explicit PersonBuilder(Person &person_ref) noexcept;
public:
	PersonBuilder() noexcept;
	operator Person() noexcept;

	PersonWorkBuilder works();

	PersonAddressBuilder lives();
};

#include "personWorkBuilder.h"
#include "personAddressBuilder.h"

#endif // PERSON_BUILDER_H