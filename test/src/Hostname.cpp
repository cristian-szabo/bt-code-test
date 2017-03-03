#include "Hostname.hpp"

#include <bandit/bandit.h>

using namespace bandit;
using namespace snowhouse;

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
            bool result = hostname.create("example.com");

            AssertThat(result, IsTrue());
        });

        it("should create from a valid hostname with subdomain", [&]() {
            bool result = hostname.create("foo.test.com");

            AssertThat(result, IsTrue());
        });

        it("should not create from a hostname with size less than 10", [&]() {
            bool result = hostname.create("test.com");

            AssertThat(result, IsFalse());
        });

        it("should not create from a hostname with size bigger than 63", [&]() {
            std::string name(64, 'a');

            bool result = hostname.create(name.c_str());

            AssertThat(result, IsFalse());
        });

        it("should not create from a hostname starting with digit", [&]() {
            bool result = hostname.create("123.example.com");

            AssertThat(result, IsFalse());
        });

        it("should not create from a hostname ending with a special character", [&]() {
            bool result = hostname.create("abc.example.co*");

            AssertThat(result, IsFalse());
        });
    });
});
