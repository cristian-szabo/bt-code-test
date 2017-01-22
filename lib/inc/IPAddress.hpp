#pragma once

#include <array>
#include <sstream>

class IPAddress
{
public:

    IPAddress();

    IPAddress(const std::string& ip_addr);

    bool isValid();

    void create(const std::string& ip_addr);

    void destroy();

    bool operator==(const IPAddress& other) const;

    bool operator!=(const IPAddress& other) const;

    friend std::ostream &operator<<(std::ostream& output, const IPAddress& ip_addr);

    friend std::istream &operator>>(std::istream& input, IPAddress& ip_addr);

private:

    std::array<std::int32_t, 4> data;

};
