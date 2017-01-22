#include "Main.hpp"

int main(int argc, char* argv[])
{
    int exit_code;

    try
    {
        Program app({ argv + 1, argv + argc });

        exit_code = app.run();
    }
    catch (const std::exception& ex)
    {
        std::cout << ex.what() << std::endl;

        exit_code = -1;
    }

    return exit_code;
}
