#pragma once

#include <string>
#include <cctype>

struct ci_char_traits : public std::char_traits<char>
{
    static bool eq(char c1, char c2);

    static bool lt(char c1, char c2);

    static int compare(const char* s1, const char* s2, size_t n);

    static const char* find(const char* s, int n, char a);
};

using ci_string = std::basic_string<char, ci_char_traits>;

struct boolyesno : std::numpunct<char>
{
protected:

    std::string do_truename() const override;

    std::string do_falsename() const override;
};
