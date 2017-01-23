#include "Hostname.hpp"

#include <bandit/bandit.h>

using namespace bandit;

go_bandit([]() {
    describe("Hostname Test", []() {
        Hostname hostname;

        after_each([&]() {
            if (hostname.isValid())
            {
                hostname.destroy();
            }
        });

        it("should create from a valid hostname witout subdomain", [&]() {
            bool result = hostname.create("test.com");

            AssertThat(result, Equals(true));
        });

        it("should create from a valid hostname with subdomain", [&]() {
            bool result = hostname.create("foo.test.com");

            AssertThat(result, Equals(true));
        });

        it("should not create from a hostname starting with digit", [&]() {
            bool result = hostname.create("123.com");

            AssertThat(result, Equals(false));
        });

        it("should not create from hostname starting with underscore", [&]() {
            bool result = hostname.create("_test.com");

            AssertThat(result, Equals(false));
        });
    });
});
