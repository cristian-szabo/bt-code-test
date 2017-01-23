#include "IPAddress.hpp"

#include <bandit/bandit.h>

using namespace bandit;

go_bandit([]() {
    describe("IPAddress Test", []() {
        IPAddress ip_addr;

        after_each([&]() {
            if (ip_addr.isValid())
            {
                ip_addr.destroy();
            }
        });

        it("should create from minimum ip address values", [&]() {
            bool result = ip_addr.create("0.0.0.0");
            
            AssertThat(result, Equals(true));
        });

        it("should create from maximum ip address values", [&]() {
            bool result = ip_addr.create("255.255.255.255");
            
            AssertThat(result, Equals(true));
        });

        it("should not create if one value is bigger than 255", [&]() {
            bool result = ip_addr.create("256.0.0.0");

            AssertThat(result, Equals(false));
        });

        it("should not create if one value is a letter", [&]() {
            bool result = ip_addr.create("a.0.0.0");

            AssertThat(result, Equals(false));
        });

        it("should not create if one value is missing", [&]() {
            bool result = ip_addr.create("0.0.0");

            AssertThat(result, Equals(false));
        });
    });
});
