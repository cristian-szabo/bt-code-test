#include "Router.hpp"

Router::Router()
{
}

Router::Router(Hostname hostname, IPAddress ip_address, bool patched, std::string os_version, std::string notes)
    : hostname(hostname), ip_address(ip_address), patched(patched), os_version(os_version), notes(notes)
{
}

Hostname Router::getHostname() const
{
    return hostname;
}

IPAddress Router::getIPAddress() const
{
    return ip_address;
}

bool Router::getPatched() const
{
    return patched;
}

float Router::getOSVersion() const
{
    return stof(os_version);
}

std::string Router::getOSVersionStr() const
{
    return os_version;
}

std::string Router::getNotes() const
{
    return notes;
}

bool Router::operator==(const Router & other) const
{
    return hostname == other.hostname && ip_address == other.ip_address;
}

bool Router::operator!=(const Router & other) const
{
    return !(*this == other);
}

std::ostream & operator<<(std::ostream & output, const Router & router)
{
    output << router.getHostname() << " (" << router.getIPAddress() << "), ";
    output << "OS version " << router.getOSVersionStr() << " ";

    const std::string& notes = router.getNotes();

    if (!notes.empty())
    {
        output << "[" << notes << "]";
    }

    return output;
}
