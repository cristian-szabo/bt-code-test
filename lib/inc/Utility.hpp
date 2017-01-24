#pragma once

#include <locale>
#include <string>
#include <cctype>

struct ci_char_traits : public std::char_traits<char>
{
    static bool eq(char c1, char c2);

    static bool lt(char c1, char c2);

    static int compare(const char* s1, const char* s2, size_t n);

    static const char* find(const char* s, int n, char a);
};

typedef std::basic_string<char, ci_char_traits> ci_string;
