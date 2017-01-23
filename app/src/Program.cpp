#include "Program.hpp"

Program::Program(std::vector<std::string> args)
{
    if (!args.size())
    {
        throw std::runtime_error("Invalid number of arguments!");
    }

    csv_file = args[0];
}

Program::~Program()
{
}

int Program::run()
{
    char path[FILENAME_MAX];

    if (!GetCurrentDir(path, sizeof(path)))
    {
        throw std::runtime_error("Failed to get current directory path!");
    }

    std::string work_dir(path);
    std::replace(work_dir.begin(), work_dir.end(), '\\', '/');
    work_dir += '/';

    std::cout << "Current working directory: " << work_dir << std::endl;

    std::ifstream file(work_dir + csv_file);

    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open CSV file!");
    }

    RouterCSVParser csv(file);
    csv.readHeader("Hostname", "IP Address", "Patched?", "OS Version", "Notes");

    std::vector<Router> routers;
    Hostname hostname;
    IPAddress ip_address;
    bool patched;
    std::string os_version;
    std::string notes;

    while (csv.readRow(hostname, ip_address, patched, os_version, notes))
    {
        if (!hostname.isValid() || !ip_address.isValid())
        {
            continue;
        }

        routers.emplace_back(hostname, ip_address, patched, os_version, notes);
    }

    auto iter_router = std::remove_if(routers.begin(), routers.end(),
        [](const Router& router)
    {
        return router.getOSVersion() < 12.0 || router.getPatched() == true;
    });

    iter_router = routers.erase(iter_router, routers.end());

    iter_router = std::remove_if(routers.begin(), routers.end(),
        [&routers](const Router& router)
    {
        auto iter_router = std::find_if(routers.begin(), routers.end(), 
            [&](const Router& search_router)
        {
            return search_router.getIPAddress() == router.getIPAddress() && router != search_router;
        });

        if (iter_router != routers.end())
        {
            return true;
        }

        iter_router = std::find_if(routers.begin(), routers.end(),
            [&](const Router& search_router)
        {
            return search_router.getHostname() == router.getHostname() && router != search_router;
        });

        if (iter_router != routers.end())
        {
            return true;
        }

        return false;
    });

    iter_router = routers.erase(iter_router, routers.end());

    std::for_each(routers.begin(), routers.end(),
        [](const Router& router)
    {       
        std::cout << router << std::endl;
    });

    return 0;
}
