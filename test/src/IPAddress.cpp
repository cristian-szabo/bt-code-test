#include "IPAddress.hpp"

#include <bandit/bandit.h>

using namespace bandit;
using namespace snowhouse;

go_bandit([]() {
    describe("IPAddress Test", []() {
        IPAddress ip_addr;

        after_each([&]() {
            if (ip_addr.isValid())
            {
                ip_addr.destroy();
            }
        });

        it("should create from minimum ip address value", [&]() {
            bool result = ip_addr.create("1.0.0.0");
            
            AssertThat(result, IsTrue());
        });

        it("should create from maximum ip address value", [&]() {
            bool result = ip_addr.create("255.255.255.254");
            
            AssertThat(result, IsTrue());
        });

        it("should not create if one value is bigger than 255", [&]() {
            bool result = ip_addr.create("256.0.0.0");

            AssertThat(result, IsFalse());
        });

        it("should not create if one value is a letter", [&]() {
            bool result = ip_addr.create("a.b.c.d");

            AssertThat(result, IsFalse());
        });

        it("should not create if one chunk is missing", [&]() {
            bool result = ip_addr.create("1.0.0");

            AssertThat(result, IsFalse());
        });

        it("should not create if one chunk is empty", [&]() {
            bool result = ip_addr.create("5.0..7");

            AssertThat(result, IsFalse());
        });

        it("should not create if a chunk is a special character", [&]() {
            bool result = ip_addr.create("*.'.;./");

            AssertThat(result, IsFalse());
        });

        it("should not create if a chunk is a negative number", [&]() {
            bool result = ip_addr.create("-3.-257.-0.-1");

            AssertThat(result, IsFalse());
        });

        it("should not create if a chunk hash numbers and special characters", [&]() {
            bool result = ip_addr.create("3/2.2^3.7.2");

            AssertThat(result, IsFalse());
        });

        it("should not create if a ip address is localhost", [&]() {
            bool result = ip_addr.create("127.0.0.1");

            AssertThat(result, IsFalse());
        });
    });
});
