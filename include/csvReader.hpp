#ifndef CSVREADER
#define CSVREADER

#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <any>

namespace csv {

using size_t = unsigned int;

class Row {
public:
    Row() = default;
    Row(const std::vector<std::string>);
    ~Row() = default;
    std::string operator[](std::string) const;
    std::string operator[](size_t) const;
    void push(std::string, std::string);
    friend std::ostream& operator<<(std::ostream&, Row);
private:
    const std::vector<std::string> _header;
    std::map<std::string, std::string> _data;
};

class csvData{
public:
    csvData() = default;
    ~csvData() = default;
    void extend(Row);
    void set_header(std::vector<std::string>);
    size_t row_count() const;
    size_t col_count() const;
    Row operator[](size_t) const;
    friend std::ostream& operator<<(std::ostream&, csvData);
private:
    std::vector<Row> _rows;
    std::vector<std::string> _header;
};

class csvReader {
private:
    static auto read_line(std::string line, char delim = ',');
    static std::vector<std::string> multi_header(std::vector<std::string>);
public:
    static csvData read_csv(std::string path, char delim = ',');
};

namespace sreader {
    auto read_line(std::string line, char delim = ',') -> std::vector<std::string>;
    auto read_csv(std::string path, char delim = ',') -> std::map<std::string, std::vector<std::string>>;
}

auto Double(std::string s) -> double;
auto Float(std::string s) -> float;
auto LLong(std::string s) -> long long;
auto Int(std::string s) -> int;

}

#endif