#include "IPAddress.hpp"

IPAddress::IPAddress()
{
    data.fill(-1);
}

IPAddress::IPAddress(const std::string & addr)
{
    data.fill(-1);

    create(addr);
}

bool IPAddress::isValid()
{
    return (std::count(data.begin(), data.end(), -1) == 0);
}

/*
    GCC 4.6 does not fully implement ECMAScript regex fully thus the use
    of the std::regex is not possible in the current version. With version
    GCC 4.9 which fully implement C++ 11 features has regex functionality
    implemented. The following regex can be used to check the ip address:

    "^(([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])\.){3}([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])$";
*/
bool IPAddress::create(const std::string & ip_addr)
{
    if (isValid())
    {
        throw std::runtime_error("IPAddress is defined!");
    }

    std::size_t dot_num = std::count(ip_addr.begin(), ip_addr.end(), '.');

    if (dot_num != 3)
    {
        return false;
    }

    std::array<std::int32_t, 4> buffer;
    std::istringstream ss(ip_addr);

    buffer.fill(-1);

    for (std::int32_t& addr : buffer)
    {
        std::string chunk;
        std::getline(ss, chunk, '.');

        std::size_t pos;
        std::int32_t tmp;

        // Check if the chunk can be converted to a digit character
        try
        {
            tmp = stoi(chunk, &pos);
        }
        catch (std::exception&)
        {
            tmp = -1;
        }

        // Check if the number of processed characters is the same as the chunk size 
        if (pos != chunk.size())
        {
            continue;
        }

        // Check if the chunk is between the 0-255 range
        if (tmp >= 0 && tmp <= 255)
        {
            addr = tmp;
        }
    }

    // A valid IP address can't have the first chunk equal to zero
    if (buffer[0] == 0)
    {
        return false;
    }

    // Localhost IP address 
    if (buffer[0] == 127 && buffer[1] == 0 && buffer[2] == 0 && buffer[3] == 1)
    {
        return false;
    }

    // Wildcard to bind to all addresses is invalid ip address
    if (std::count(buffer.begin(), buffer.end(), 255) == 4)
    {
        return false;
    }

    // Local broadcast is invalid ip address
    if (std::count(buffer.begin(), buffer.end(), 0) == 4)
    {
        return false;
    }

    // If a chunk is invalid then the whole IP address is invalid
    if (std::count(buffer.begin(), buffer.end(), -1))
    {
        return false;
    }

    std::copy(buffer.begin(), buffer.end(), data.begin());

    return true;
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
