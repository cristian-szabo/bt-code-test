#pragma once

#include <cctype>
#include <algorithm>
#include <iterator>

#include "Utility.hpp"

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
