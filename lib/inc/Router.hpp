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

    // Assumption: OS version is formed of two chunks separated by a dot character 
    // which can be stored as a float variable and later compared as a floating point number.
    std::string os_version;

    std::string notes;

};
