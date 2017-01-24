#include "Router.hpp"

Router::Router()
{
}

Router::Router(const std::string& hostname,
    const std::string& ip_address,
    const std::string& patched,
    const std::string& os_version,
    const std::string& notes)
{
    create(hostname, ip_address, patched, os_version, notes);
}

bool Router::create(const std::string& hostname,
    const std::string& ip_address,
    const std::string& patched,
    const std::string& os_version,
    const std::string& notes)
{
    if (isValid())
    {
        throw std::runtime_error("Router is defined!");
    }

    if (this->hostname.isValid())
    {
        this->hostname.destroy();
    }

    if (!this->hostname.create(ci_string(hostname.begin(), hostname.end())))
    {
        return false;
    }

    if (this->ip_address.isValid())
    {
        this->ip_address.destroy();
    }

    if (!this->ip_address.create(ip_address))
    {
        return false;
    }

    ci_string buffer(patched.begin(), patched.end());

    this->patched.assign(patched.begin(), patched.end());

    if (buffer != "yes" && buffer != "no")
    {
        return false;
    }

    this->patched = buffer;

    std::size_t pos;
    float tmp;

    try
    {
        tmp = stof(os_version, &pos);
    }
    catch (std::exception&)
    {
        tmp = -1;
    }

    if (pos != os_version.size())
    {
        return false;
    }
    
    if (tmp >= std::numeric_limits<float>::epsilon())
    {
        this->os_version = os_version;
    }

    this->notes = notes;

    return true;
}

bool Router::isValid()
{
    return hostname.isValid() && ip_address.isValid() && !patched.empty() && !os_version.empty();
}

void Router::destroy()
{
    if (!isValid())
    {
        throw std::runtime_error("Router is not defined!");
    }

    hostname.destroy();

    ip_address.destroy();

    patched.clear();

    os_version.clear();

    notes.clear();
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
    return patched == "yes" ? true : false;
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
