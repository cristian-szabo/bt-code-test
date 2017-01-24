#include "Program.hpp"

Program::Program(std::vector<std::string> args)
{
    if (!args.size())
    {
        throw std::runtime_error("Invalid number of arguments!\nPlease specify the location of the CSV file.");
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

    // Format current directoy to Linux style
    std::string work_dir(path);
    std::replace(work_dir.begin(), work_dir.end(), '\\', '/');
    work_dir += '/';

    std::ifstream file(work_dir + csv_file);

    // Check if the CSV file was opened correctly
    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open CSV file!");
    }

    RouterCSVParser csv(file);

    // Read header row from the CSV file
    csv.readHeader("Hostname", "IP Address", "Patched?", "OS Version", "Notes");

    Router router;
    std::vector<Router> routers;
    std::string hostname, ip_address, patched, os_version, notes;

    // Read each row containing the router data from teh CSV file
    while (csv.readRow(hostname, ip_address, patched, os_version, notes))
    {
        router.create(hostname, ip_address, patched, os_version, notes);

        if (router.isValid())
        {
            routers.push_back(router);

            router.destroy();
        }
    }

    // Remove routers which don't meet the firt two criterias:
    // 1. The router has not already been patched
    // 2. The current version of the router OS is 12 or above
    auto iter_router = std::remove_if(routers.begin(), routers.end(),
        [](const Router& router)
    {
        return router.getOSVersion() < 12.0 || router.getPatched() == true;
    });

    // Perform remove operation
    iter_router = routers.erase(iter_router, routers.end());

    // Remove routers which don't meet the next two criteris:
    // 3. There are no other routers which share the same IP address
    // 4. There are no other routers which share the same hostname
    iter_router = std::remove_if(routers.begin(), routers.end(),
        [&routers](const Router& router)
    {
        // Find other routers with the same ip address
        auto iter_router = std::find_if(routers.begin(), routers.end(), 
            [&](const Router& search_router)
        {
            return search_router.getIPAddress() == router.getIPAddress() && router != search_router;
        });

        if (iter_router != routers.end())
        {
            return true;
        }

        // Find other routers with the same hostname
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

    // Perform remove operation
    iter_router = routers.erase(iter_router, routers.end());

    // Display routers which should be patched int the following format:
    // Print: {hostname} ({ip-address}), OS version {os-version} [{notes}]
    for (const Router& router : routers)
    {
        std::cout << router << std::endl;
    }

    return 0;
}
