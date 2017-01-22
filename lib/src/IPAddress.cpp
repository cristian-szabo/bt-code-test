#include "IPAddress.hpp"

IPAddress::IPAddress()
{
    data.fill(-1);
}

IPAddress::IPAddress(const std::string & addr)
{
    create(addr);
}

bool IPAddress::isValid()
{
    bool valid = true;

    std::for_each(data.begin(), data.end(), 
        [&](std::int32_t addr) 
    {
        if (addr < 0 || addr > 255)
        {
            valid = false;
        }
    });

    return valid;
}

void IPAddress::create(const std::string & ip_addr)
{
    if (isValid())
    {
        throw std::runtime_error("IPAddress is defined!");
    }

    std::istringstream ss(ip_addr);

    std::for_each(data.begin(), data.end(),
        [&](std::int32_t& addr)
    {
        std::string val;
        std::getline(ss, val, '.');

        addr = stoi(val);
    });
}

void IPAddress::destroy()
{
    if (!isValid())
    {
        throw std::runtime_error("IPAddress is not defined!");
    }

    data.fill(-1);
}

bool IPAddress::operator==(const IPAddress& other) const
{
    return (data[0] == other.data[0] &&
        data[1] == other.data[1] && 
        data[2] == other.data[2] && 
        data[3] == other.data[3]);
}

bool IPAddress::operator!=(const IPAddress& other) const
{
    return !(*this == other);
}

std::ostream& operator<<(std::ostream& output, const IPAddress & ip_addr)
{
    output << ip_addr.data[0] << "." << ip_addr.data[1] << "." << ip_addr.data[2] << "." << ip_addr.data[3];

    return output;
}

std::istream& operator>>(std::istream& input, IPAddress & ip_addr)
{
    std::string str;
    
    std::copy(
        std::istreambuf_iterator<char>(input), 
        std::istreambuf_iterator<char>(), 
        std::back_inserter(str));

    if (ip_addr.isValid())
    {
        ip_addr.destroy();
    }

    ip_addr.create(str);

    return input;
}
