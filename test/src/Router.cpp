#include "Router.hpp"

#include <bandit/bandit.h>

using namespace bandit;

go_bandit([]() {
    describe("Router Test", []() {
        Hostname hostname("test.com");
        IPAddress ip_addr("1.1.1.1");

        it("should create a router object with a valid hostname", [&]() {
            Router router(hostname, ip_addr, false, "1.0", "");

            AssertThat(router.getHostname(), Equals(hostname));
        });

        it("should create a router object with a valid ip address", [&]() {
            Router router(hostname, ip_addr, false, "1.0", "");

            AssertThat(router.getIPAddress(), Equals(ip_addr));
        });
    });
});
