#include "Utility.hpp"

bool ci_char_traits::eq(char c1, char c2)
{
    return std::toupper(c1) == std::toupper(c2);
}

bool ci_char_traits::lt(char c1, char c2)
{
    return std::toupper(c1) <  std::toupper(c2);
}

int ci_char_traits::compare(const char * s1, const char * s2, size_t n)
{
    while (n-- != 0)
    {
        if (std::toupper(*s1) < std::toupper(*s2))
        {
            return -1;
        }

        if (std::toupper(*s1) > std::toupper(*s2))
        {
            return 1;
        }

        ++s1;
        ++s2;
    }

    return 0;
}

const char * ci_char_traits::find(const char * s, int n, char a)
{
    auto const ua(std::toupper(a));

    while (n-- != 0)
    {
        if (std::toupper(*s) == ua)
        {
            return s;
        }

        s++;
    }

    return nullptr;
}

std::basic_istream<char>& operator >> (std::basic_istream<char>& in, ci_string & ci_str)
{
    std::basic_string<char> str;
    in >> str;
    ci_str.assign(str.data(), str.size());
    return in;
}

std::basic_ostream<char>& operator<<(std::basic_ostream<char>& out, const ci_string & ci_str)
{
    std::basic_string<char> str(ci_str.data(), ci_str.size());
    out << str;
    return out;
}
