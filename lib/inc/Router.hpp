#pragma once

#include <string>
#include <vector>

#include "Hostname.hpp"
#include "IPAddress.hpp"

class Router
{
public:

    Router();

    Router(const std::string& hostname,
        const std::string& ip_address,
        const std::string& patched,
        const std::string& os_version,
        const std::string& notes);

    bool create(const std::string& hostname,
        const std::string& ip_address,
        const std::string& patched,
        const std::string& os_version,
        const std::string& notes);

    bool isValid();

    void destroy();

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

    ci_string patched;

    std::string os_version;

    std::string notes;

};
