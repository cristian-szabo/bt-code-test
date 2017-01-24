#include "Router.hpp"

#include <bandit/bandit.h>

using namespace bandit;

go_bandit([]() {
    describe("Router Test", []() {
        Router router;
        std::string hostname = "example.com";
        std::string ip_address = "1.1.1.1";
        std::string patched = "no";
        std::string os_version = "1.0";
        std::string notes = "test";

        after_each([&]() {
            if (router.isValid())
            {
                router.destroy();
            }
        });

        it("should create a router object with a valid data", [&]() {
            bool result = router.create(hostname, ip_address, patched, os_version, notes);

            AssertThat(result, Equals(true));
        });

        it("should not create a router object with an invalid hostname", [&]() {
            bool result = router.create("", ip_address, patched, os_version, notes);

            AssertThat(result, Equals(false));
        });

        it("should not create a router object with an invalid ip address", [&]() {
            bool result = router.create(hostname, "", patched, os_version, notes);

            AssertThat(result, Equals(false));
        });

        it("should not create a router object with an invalid patched(yes/no)", [&]() {
            bool result = router.create(hostname, ip_address, "", os_version, notes);

            AssertThat(result, Equals(false));

            result = router.create(hostname, ip_address, "123", os_version, notes);

            AssertThat(result, Equals(false));

            result = router.create(hostname, ip_address, "*;]", os_version, notes);

            AssertThat(result, Equals(false));
        });

        it("should not create a router object with an invalid OS version", [&]() {
            bool result = router.create(hostname, ip_address, patched, "", notes);

            AssertThat(result, Equals(false));

            result = router.create(hostname, ip_address, patched, "3xc", notes);

            AssertThat(result, Equals(false));

            result = router.create(hostname, ip_address, patched, "abc", notes);

            AssertThat(result, Equals(false));
        });

        it("should two router objects be equal", [&]() {
            bool result = router.create(hostname, ip_address, patched, os_version, notes);
            Router router_other(hostname, ip_address, patched, os_version, notes);

            AssertThat(router, Equals(router_other));
        });
    });
});
