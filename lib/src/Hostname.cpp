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

void Hostname::create(const ci_string & hostname)
{
    if (isValid())
    {
        throw std::runtime_error("Hostname is defined!");
    }

    data = hostname;

    std::for_each(data.begin(), data.end(), 
        [](char& c) 
    {
        c = std::tolower(c);
    });
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
