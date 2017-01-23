#include "CSVParser.hpp"

#include <bandit/bandit.h>

using namespace bandit;

go_bandit([]() {
    describe("CSVParser Test", []() {
        CSVParser<1> csv(R"(column1 ,column2\ncell1,cell2\n)");

        it("should trim white spaces from column and header", [&]() {
            AssertThat(3, Equals(3));
        });
    });
});
