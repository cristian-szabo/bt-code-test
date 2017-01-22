#pragma once

#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>

#include "Utility.hpp"

/*
    TrimChars Policy
*/

template<char... TrimChar>
struct TrimChars
{
public:

    static void trim(std::string& line) 
    {
        auto found_iter = std::remove_if(line.begin(), line.end(),
            [](char c) 
        {
            return isTrimChar(c, TrimChar...);
        });

        found_iter = line.erase(found_iter, line.end());
    }

private:

    static bool isTrimChar(char c)
    {
        return false;
    }

    template<class... OtherTrimChar>
    static bool isTrimChar(char c, char trim_char, OtherTrimChar... other_trim_char) 
    {
        return c == trim_char || isTrimChar(c, other_trim_char...);
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
            
        if (found_iter != buffer.end())
        {
            return true;
        }

        return false;
    }
};

/*
    Helper
*/

namespace detail
{
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

        std::for_each(buffer.begin(), buffer.end(),
            [](char& c)
        {
            c = std::tolower(c);
        });

        std::istringstream iss(buffer);
        iss.imbue({ iss.getloc(), new boolyesno() });

        iss >> std::boolalpha >> object;
    }

    template<>
    inline void parse(const std::string& cell, std::string& object)
    {
        object.assign(cell.begin(), cell.end());
    }
}

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

    enum class IgnoreType : std::size_t
    {
        Invalid = 0,
        NoColumn,
        ExtraColumn,
        MissingColumn,
        MaxCount
    };

    CSVParser(const std::string& file_name)
    {
        header_columns.resize(column_num);

        file.open(file_name);

        if (!file)
        {
            throw std::runtime_error("CSV file not found!");
        }
    }

    template<class ...ColumnTypes>
    void readHeader(IgnoreType ignore_column, ColumnTypes... header_columns)
    {
        constexpr std::size_t column_count = sizeof...(ColumnTypes);

        static_assert(column_count >= column_num, "Not enough columns specified");
        static_assert(column_count <= column_num, "Too many columns specified");

        setHeaderColumns(std::forward<ColumnTypes>(header_columns)...);

        std::string line;

        do
        {
            std::getline(file, line);

            if (line.empty())
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

            auto column_iter = std::find(header_columns.begin(), header_columns.end(), column);

            if (column_iter == header_columns.end())
            {
                if (ignore_column == IgnoreType::ExtraColumn)
                {
                    column_skip.push_back(-1);
                }
                else
                {
                    throw std::runtime_error("Column not found!");
                }
            }

            std::size_t index = std::distance(header_columns.begin(), column_iter);

            found_columns[index] = true;

            column_skip.push_back(index);
        }

        if (ignore_column != IgnoreType::MissingColumn)
        {
            std::for_each(found_columns.begin(), found_columns.end(), 
                [](bool found_column) 
            {
                if (!found_column)
                {
                    throw std::runtime_error("Missing column in header file");
                }
            });
        }
    }

    template<class ...ColumnTypes>
    bool readRow(ColumnTypes&... column_types)
    {
        constexpr std::size_t column_cout = sizeof...(ColumnTypes);

        static_assert(column_cout >= column_num, "Not enough columns specified");
        static_assert(column_cout <= column_num, "Too many columns specified");

        std::string line;

        do
        {
            std::getline(file, line);

            if (line.empty())
            {
                return false;
            }
        } while (CommentPolicy::isComment(line));

        std::for_each(column_skip.begin(), column_skip.end(), 
            [&](std::int32_t skip_column) 
        {
            std::string cell = QuotePolicy::split(line);

            if (skip_column < 0)
            {
                row_columns.push_back(std::string());
            }
            else
            {
                TrimPolicy::trim(cell);
                QuotePolicy::unescape(cell);

                row_columns.push_back(cell);
            }
        });

        setRowColumns(0, column_types...);

        row_columns.clear();

        return true;
    }

private:

    std::ifstream file;

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
            detail::parse<T>(cell, object);
        }

        setRowColumns(index + 1, column_types...);
    }

    void setRowColumns(std::size_t index)
    {
    }

};
