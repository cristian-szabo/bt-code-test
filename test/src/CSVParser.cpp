#include "CSVParser.hpp"

#include <bandit/bandit.h>

using namespace bandit;

go_bandit([]() {
    describe("CSVParser Test", []() {
        it("should trim tab character from header column", [&]() {
            CSVParser<2> csv("column1\t,column2\t\n");
            csv.readHeader("column1", "column2");

            AssertThat(csv.getHeaderColumn<0>(), Equals("column1"));
            AssertThat(csv.getHeaderColumn<1>(), Equals("column2"));
        });

        it("should trim tab character from row cell", [&]() {
            CSVParser<2> csv("column1,column2\ncell1\t,cell2\t\n");
            csv.readHeader("column1", "column2");

            std::string cell1, cell2;
            csv.readRow(cell1, cell2);

            AssertThat(cell1, Equals("cell1"));
            AssertThat(cell2, Equals("cell2"));
        });

        it("should trim white space character from header column", [&]() {
            CSVParser<2, EmptyLineComment, NoQuoteEscape<','>, TrimChars<' '>> csv("column1   ,column2 \n");
            csv.readHeader("column1", "column2");

            AssertThat(csv.getHeaderColumn<0>(), Equals("column1"));
            AssertThat(csv.getHeaderColumn<1>(), Equals("column2"));
        });

        it("should trim white space character from row cell", [&]() {
            CSVParser<2, EmptyLineComment, NoQuoteEscape<','>, TrimChars<' '>> csv("column1,column2\ncell1   ,cell2 \n");
            csv.readHeader("column1", "column2");

            std::string cell1, cell2;
            csv.readRow(cell1, cell2);

            AssertThat(cell1, Equals("cell1"));
            AssertThat(cell2, Equals("cell2"));
        });

        it("should skip empty lines when reading rows", [&]() {
            CSVParser<2> csv(" \n \ncolumn1,column2\n \n    \ncell1,cell2\n");
            csv.readHeader("column1", "column2");

            std::string cell1, cell2;
            csv.readRow(cell1, cell2);

            AssertThat(cell1, Equals("cell1"));
            AssertThat(cell2, Equals("cell2"));
        });

        it("should ignore double quotes when reading row cells", [&]() {
            CSVParser<2, EmptyLineComment, DoubleQuoteEscape<',', '"'>> csv("\"column1\",column2\n\"cell1\",cell2\n");
            csv.readHeader("column1", "column2");

            std::string cell1, cell2;
            csv.readRow(cell1, cell2);

            AssertThat(cell1, Equals("cell1"));
            AssertThat(cell2, Equals("cell2"));
        });
    });
});
