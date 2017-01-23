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

bool Hostname::create(const ci_string & hostname)
{
    if (isValid())
    {
        throw std::runtime_error("Hostname is defined!");
    }

    ci_string buffer = hostname;

    for (char& c : buffer)
    {
        c = std::tolower(c);
    }

    if (std::isdigit(buffer[0]) || buffer[0] == '_')
    {
        return false;
    }

    if (buffer.find('.') == std::string::npos)
    {
        return false;
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
