#pragma once

#include <cctype>
#include <stdexcept>
#include <algorithm>
#include <iterator>

#include "Utility.hpp"

/*
    Hostname assumptions:
    - must be 63 characters or less (e.g. some Cisco IOS versions)
    - should have at least 10 characters (RFC1035 - Domain Names - Implementation and Specification)
    - follows the ARPANET rules: must start with a letter, end with a letter or digit, can only contain 
    letters, digits and hyphens. To follow the BTCodeTest dots will also be permited.
    - is case insensitive
*/
class Hostname
{
public:

    Hostname();

    Hostname(const ci_string& hostname);

    bool isValid();

    bool create(const ci_string& hostname);

    void destroy();

    bool operator==(const Hostname& other) const;

    bool operator!=(const Hostname& other) const;

    friend std::ostream &operator<<(std::ostream& output, const Hostname& hostname);

    friend std::istream &operator>>(std::istream& input, Hostname& hostname);

private:

    ci_string data;

};
