#pragma once

#include <stdexcept>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>

#include "Utility.hpp"

/*
    Helper Functions
*/

namespace detail
{
    inline bool isTrimChar(char c)
    {
        return false;
    }

    template<class... TrimChar>
    inline bool isTrimChar(char c, char trim_char, TrimChar... other_trim_char)
    {
        return c == trim_char || isTrimChar(c, other_trim_char...);
    }

    template<class T>
    inline void parse(const std::string& cell, T& object)
    {
        std::istringstream iss(cell);

        iss >> object;
    }

    template<>
    inline void parse(const std::string& cell, bool& object)
    {
        std::string buffer = cell;

        for (char& c : buffer)
        {
            c = std::tolower(c);
        }

        std::istringstream iss(buffer);

        iss >> std::boolalpha >> object;
    }

    template<>
    inline void parse(const std::string& cell, std::string& object)
    {
        object.assign(cell.begin(), cell.end());
    }
}

/*
    TrimChars Policy
*/

template<char... TrimChar>
struct TrimChars
{
    static void trim(std::string& line) 
    {
        auto found_iter = std::remove_if(line.begin(), line.end(),
            [](char c) 
        {
            return ::detail::isTrimChar(c, TrimChar...);
        });

        found_iter = line.erase(found_iter, line.end());
    }
};

/*
    QuoteEscape Policy
*/

template<char sep>
struct NoQuoteEscape
{
    static std::string split(std::string& line)
    {
        std::string result;

        std::size_t found_pos = line.find_first_of(sep, 0);

        if (found_pos == std::string::npos)
        {
            result = line;

            line.clear();
        }
        else
        {
            result = line.substr(0, found_pos);

            line = line.substr(found_pos + 1);
        }

        return result;
    }

    static void unescape(std::string& line)
    {
    }
};

template<char sep, char quote>
struct DoubleQuoteEscape
{
    static std::string split(std::string& line)
    {
        std::string result;

        std::size_t sep_pos = line.find_first_of(sep, 0);
        std::size_t quote_start_pos = line.find_first_of(quote, 0);
        std::size_t quote_end_pos = line.find_first_of(quote, quote_start_pos + 1);

        if (quote_start_pos != std::string::npos &&
            ((quote_end_pos != std::string::npos && sep_pos < quote_end_pos) ||
                quote_end_pos == std::string::npos))
        {
            throw std::runtime_error("No ending escape string was found!");
        }
        
        if (sep_pos == std::string::npos)
        {
            result = line;

            line.clear();
        }
        else
        {
            result = line.substr(0, sep_pos);

            line = line.substr(sep_pos + 1);
        }

        return result;
    }

    static void unescape(std::string& line)
    {
        for (std::size_t next = line.find(quote);
            next != std::string::npos;
            next = line.find(quote, next)
            )
        {
            line.erase(next, 1);
            next += 1;
        }
    }
};

/*
    Comment Policy
*/

struct NoComment 
{
    static bool isComment(const std::string& line) 
    {
        return false;
    }
};

struct EmptyLineComment 
{
    static bool isComment(const std::string& line)
    {
        if (line.empty())
        {
            return true;
        }

        std::string buffer = line;

        auto found_iter = std::remove(buffer.begin(), buffer.end(), ' ');
        found_iter = buffer.erase(found_iter, buffer.end());

        found_iter = std::remove(buffer.begin(), buffer.end(), '\t');
        found_iter = buffer.erase(found_iter, buffer.end());
            
        if (buffer.empty())
        {
            return true;
        }

        return false;
    }
};

/*
    CSV Parser
*/

template
<
    unsigned int column_num,
    class CommentPolicy = EmptyLineComment,
    class QuotePolicy = NoQuoteEscape<','>,
    class TrimPolicy = TrimChars<'\t'>
>
class CSVParser
{
public:

    explicit CSVParser(const std::string& data) 
    {
        header_columns.resize(column_num);

        reader << data;
    }

    explicit CSVParser(const std::ifstream& file)
    {
        header_columns.resize(column_num);

        reader << file.rdbuf();
    }

    template<class ...ColumnTypes>
    void readHeader(ColumnTypes&... header_columns)
    {
        constexpr std::size_t column_count = sizeof...(ColumnTypes);

        static_assert(column_count >= column_num, "Header columns out of bounds");
        static_assert(column_count <= column_num, "Header columns out of bounds");

        setHeaderColumns(header_columns...);

        std::string line;

        do
        {
            std::getline(reader, line);

            if (reader.eof() && line.empty())
            {
                throw std::runtime_error("Header row is missing!");
            }
        } while (CommentPolicy::isComment(line));

        std::vector<bool> found_columns(column_count);
        std::fill(found_columns.begin(), found_columns.end(), false);

        while (!line.empty())
        {
            std::string column = QuotePolicy::split(line);

            TrimPolicy::trim(column);
            QuotePolicy::unescape(column);

            auto column_iter = std::find(this->header_columns.begin(), this->header_columns.end(), column);

            if (column_iter == this->header_columns.end())
            {
                throw std::runtime_error("Column not found!");
            }

            std::size_t index = std::distance(this->header_columns.begin(), column_iter);

            column_skip.push_back(index);

            found_columns[index] = true;
        }
    }

    template<class ...ColumnTypes>
    bool readRow(ColumnTypes&... column_types)
    {
        constexpr std::size_t column_cout = sizeof...(ColumnTypes);

        static_assert(column_cout >= column_num, "Row columns out of bounds");
        static_assert(column_cout <= column_num, "Row columns out of bounds");

        row_columns.clear();

        std::string line;

        do
        {
            std::getline(reader, line);

            if (reader.eof() && line.empty())
            {
                return false;
            }
        } while (CommentPolicy::isComment(line));

        for(auto column_iter = column_skip.begin(); 
            column_iter != column_skip.end(); 
            column_iter++)
        {
            if (line.empty() && column_iter != (column_skip.begin() + column_skip.size() - 1))
            {
                throw std::runtime_error("Too few columns in the current row!");
            }

            std::string cell = QuotePolicy::split(line);

            if ((*column_iter) < 0)
            {
                row_columns.push_back(std::string());
            }
            else
            {
                TrimPolicy::trim(cell);
                QuotePolicy::unescape(cell);

                row_columns.push_back(cell);
            }
        };

        setRowColumns(0, column_types...);

        if (!line.empty())
        {
            throw std::runtime_error("Too many columns in the current row!");
        }

        return true;
    }

    template<unsigned int index>
    std::string getHeaderColumn()
    {
        static_assert(index < column_num, "Header column index out of bounds 1");
        static_assert(index >= 0, "Header column index out of bounds 2");

        return header_columns.at(index);
    }

private:

    std::stringstream reader;

    std::vector<std::string> header_columns;

    std::vector<std::int32_t> column_skip;

    std::vector<std::string> row_columns;

    template<class ...ColumnTypes>
    void setHeaderColumns(const std::string& column_name, ColumnTypes... column_types) 
    {
        constexpr std::size_t column_left = sizeof...(ColumnTypes);

        header_columns[column_num - column_left - 1] = std::move(column_name);

        setHeaderColumns(std::forward<ColumnTypes>(column_types)...);
    }

    void setHeaderColumns()
    {
    }

    template<class T, class ...ColumnTypes>
    void setRowColumns(std::size_t index, T& object, ColumnTypes&... column_types)
    {
        std::string cell = row_columns.at(index);

        if (column_skip.at(index) != -1)
        {
            ::detail::parse<T>(cell, object);
        }

        setRowColumns(index + 1, column_types...);
    }

    void setRowColumns(std::size_t index)
    {
    }

};
