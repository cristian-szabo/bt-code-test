#include "Router.hpp"

#include <bandit/bandit.h>

using namespace bandit;

go_bandit([]() {
    describe("Router", []() {
        before_each([&]() {

        });

        it("Test 1", [&]() {
            AssertThat(3, Equals(3));
        });
    });
});
