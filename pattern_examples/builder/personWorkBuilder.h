#ifndef PERSON_WORK_BUILDER_H
#define PERSON_WORK_BUILDER_H

#include "personBuilder.h"

class PersonWorkBuilder : public PersonBuilder
{
    typedef PersonWorkBuilder Self;
public:
    explicit PersonWorkBuilder(Person &person_ref) noexcept;

    Self& in(std::string const &company_name);

    Self& earning(int annual_income);

    Self& as_a(std::string const &position);
};

#endif