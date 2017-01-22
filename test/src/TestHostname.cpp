#include "Hostname.hpp"

#include <bandit/bandit.h>

using namespace bandit;

go_bandit([]() {
    describe("Test Hostname", []() {
        before_each([&]() {

        });

        it("should ", [&]() {
            AssertThat(3, Equals(3));
        });
    });
});
