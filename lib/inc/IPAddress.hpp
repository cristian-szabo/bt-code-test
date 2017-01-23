#pragma once

#include <stdexcept>
#include <algorithm>
#include <array>
#include <sstream>
#include <regex>

class IPAddress
{
public:

    IPAddress();

    IPAddress(const std::string& ip_addr);

    bool isValid();

    bool create(const std::string& ip_addr);

    void destroy();

    bool operator==(const IPAddress& other) const;

    bool operator!=(const IPAddress& other) const;

    friend std::ostream &operator<<(std::ostream& output, const IPAddress& ip_addr);

    friend std::istream &operator>>(std::istream& input, IPAddress& ip_addr);

private:

    std::array<std::int32_t, 4> data;

};
