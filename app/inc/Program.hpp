#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <iostream>

#include <stdio.h>

#ifdef _WIN32
#   include <direct.h>
#   define GetCurrentDir _getcwd
#else
#   include <unistd.h>
#   define GetCurrentDir getcwd
#endif

#include "CSVParser.hpp"
#include "Router.hpp"
#include "Hostname.hpp"
#include "IPAddress.hpp"

class Program
{
public:

    Program(std::vector<std::string> args);

    ~Program();

    int run();

private:

    using RouterCSVParser = CSVParser<5>;

    std::string csv_file;

};
