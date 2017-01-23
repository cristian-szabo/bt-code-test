#include "Hostname.hpp"

#include <bandit/bandit.h>

using namespace bandit;

go_bandit([]() {
    describe("Hostname Test", []() {
        before_each([&]() {

        });

        it("should ", [&]() {
            AssertThat(3, Equals(3));
        });
    });
});
