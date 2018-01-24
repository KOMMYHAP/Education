#ifndef PERSON_ADDRESS_BUILDER_H
#define PERSON_ADDRESS_BUILDER_H

#include "personBuilder.h"

class PersonAddressBuilder : public PersonBuilder
{
    typedef PersonAddressBuilder Self;
public:
    explicit PersonAddressBuilder(Person &person_ref) noexcept;

    Self& at(std::string const &street_address);

    Self& with_post_code(std::string const &post_code);

    Self& in(std::string const &city);
};

#endif