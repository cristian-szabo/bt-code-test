#include "CSVParser.hpp"

#include <bandit/bandit.h>

using namespace bandit;

go_bandit([]() {
    describe("CSVParser Test", []() {
        it("should trim tab character from header column", [&]() {
            CSVParser<2> csv("column1\t,column2\t\n");
            csv.readHeader("column1", "column2");
        });

        it("should trim tab character from row cell", [&]() {
            CSVParser<2> csv("column1,column2\ncell1\t,cell2\t\n");
            csv.readHeader("column1", "column2");

            std::string cell1, cell2;
            csv.readRow(cell1, cell2);

            AssertThat(cell1, Equals("cell1"));
            AssertThat(cell2, Equals("cell2"));
        });
    });
});
