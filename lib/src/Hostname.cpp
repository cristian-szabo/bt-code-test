#include "Hostname.hpp"

Hostname::Hostname()
{
}

Hostname::Hostname(const ci_string& hostname)
{
    create(hostname);
}

bool Hostname::isValid()
{
    return !data.empty();
}

/*
    GCC 4.6 does not fully implement ECMAScript regex fully thus the use
    of the std::regex is not possible in the current version. With version
    GCC 4.9 which fully implements C++ 11 features has support for regex 
    functionality. The following regex can be used to check the hostname:

    "^(([a-zA-Z0-9]|[a-zA-Z0-9][a-zA-Z0-9\-]*[a-zA-Z0-9])\.)*([A-Za-z0-9]|[A-Za-z0-9][A-Za-z0-9\-]*[A-Za-z0-9])$";
*/
bool Hostname::create(const ci_string & hostname)
{
    if (isValid())
    {
        throw std::runtime_error("Hostname is defined!");
    }

    if (hostname.size() > 63 || hostname.size() < 10)
    {
        return false;
    }

    ci_string buffer = hostname;

    for (char& c : buffer)
    {
        c = std::tolower(c);
    }

    if (!std::isalpha(buffer[0]))
    {
        return false;
    }

    std::size_t last_pos = buffer.size() - 1;

    if (!std::isalnum(buffer[last_pos]))
    {
        return false;
    }

    for (char c : buffer)
    {
        if (std::isalnum(c))
        {
            continue;
        }

        if (c != '.' && c != '-')
        {
            return false;
        }
    }

    data = buffer;

    return true;
}

void Hostname::destroy()
{
    data.clear();
}

bool Hostname::operator==(const Hostname & other) const
{
    return data == other.data;
}

bool Hostname::operator!=(const Hostname & other) const
{
    return !(*this == other);
}

std::ostream& operator<<(std::ostream& output, const Hostname & hostname)
{
    output << hostname.data.c_str();

    return output;
}

std::istream& operator>>(std::istream& input, Hostname & hostname)
{
    ci_string str;
    
    std::copy(
        std::istreambuf_iterator<char>(input), 
        std::istreambuf_iterator<char>(), 
        std::back_inserter(str));

    if (hostname.isValid())
    {
        hostname.destroy();
    }

    hostname.create(str);

    return input;
}
