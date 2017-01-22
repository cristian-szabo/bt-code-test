#pragma once

#include <string>
#include <vector>

#include "Hostname.hpp"
#include "IPAddress.hpp"

class Router
{
public:

    Router();

    Router(Hostname hostname,
        IPAddress ip_address,
        bool patched,
        std::string os_version,
        std::string notes);

    Hostname getHostname() const;

    IPAddress getIPAddress() const;

    bool getPatched() const;

    float getOSVersion() const;

    std::string getOSVersionStr() const;

    std::string getNotes() const;

    bool operator==(const Router& other) const;

    bool operator!=(const Router& other) const;

    friend std::ostream &operator<<(std::ostream& output, const Router& router);

private:

    Hostname hostname;

    IPAddress ip_address;

    bool patched;

    std::string os_version;

    std::string notes;

};
