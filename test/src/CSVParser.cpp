#include "CSVParser.hpp"

#include <bandit/bandit.h>

using namespace bandit;

go_bandit([]() {
    describe("CSVParser", []() {
        before_each([&]() {
            
        });

        it("Test 1", [&]() {
            AssertThat(3, Equals(3));
        });
    });
});
