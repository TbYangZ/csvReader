#include "csvReader.hpp"

#include <sstream>
#include <iomanip>

namespace csv {

Row::Row(const std::vector<std::string> header):
    _header(header) {}

std::string Row::operator[](std::string index_str) const {
    return _data.at(index_str);
}

std::string Row::operator[](size_t index) const  {
    return this->operator[](_header[index]);
}

void Row::push(std::string col, std::string data) {
    _data[col] = data;
}

std::ostream& operator<<(std::ostream& out, Row row) {
    size_t sz = row._data.size();
    for (int i = 0; i < sz; ++i) {
        out << row._data[row._header[i]];
        if (i + 1 != sz) out << " | ";
    }
    return out;
}

void csvData::extend(Row row) {
    _rows.push_back(row);
}

size_t csvData::row_count() const { return _rows.size(); }

size_t csvData::col_count() const { return _header.size(); }

Row csvData::operator[](size_t index) const {
    return _rows[index];
}

void csvData::set_header(std::vector<std::string> header) {
    _header = header;
}

std::ostream& operator<<(std::ostream& out, csvData csv) {
    std::size_t maxlen = 0, m = csv.col_count();
    for (auto head : csv._header) {
        maxlen = std::max(maxlen, head.length());
    }
    for (auto row : csv._rows) {
        for (int i = 0; i < m; ++i) {
            maxlen = std::max(maxlen, row[i].length());
        }
    }
    out << std::left;
    for (int i = 0; i < m; ++i) {
        out << std::setw(maxlen) << std::setfill(' ') << csv._header[i] << " | "[i + 1 != m];
    }
    out << std::endl;
    for (auto row : csv._rows) {
        for (int i = 0; i < m; ++i) {
            out << std::setw(maxlen) << std::setfill(' ') << row[i] << " | "[i + 1 != m];
        }
        out << std::endl;
    }
    return out;
}

std::vector<std::string> csvReader::multi_header(std::vector<std::string> header) {
    std::map<std::string, int> count;
    for (auto& str : header) {
        if (count[str] != 0) {
            int cnt = count[str];
            ++count[str];
            std::stringstream ss;
            std::string s;
            ss << cnt;
            ss >> s;
            str = str + "." + s;
        } else ++count[str];
    }
    return header;
}

auto csvReader::read_line(std::string line, char delim) {
    std::vector<std::string> row;
    int len = line.length();
    for (int i = 0, j = 0; i < len; i = j + 1) {
        int st = i, ed = i;
        std::string s;
        if (line[i] == '"') {
            j = i + 1;
            while (j < len) {
                if (line[j] == '"' && j + 1 != len && line[j + 1] == delim) break;
                else if (line[j] == '"' && j + 1 == len) break;
                if (line[j] == '"' && line[j + 1] == '"') {
                    s.push_back(line[j]);
                    j += 2;
                    continue;
                } else s.push_back(line[j]);
                ++j;
            }
            if (j == len) {
                throw std::runtime_error("Missing paired quotes.");
            }
            ++j;
        } else {
            j = i;
            while (j < len && line[j] != delim) {
                s.push_back(line[j]);
                ++j;
            }
        }
        row.push_back(s);
    }
    return row;
}

csvData csvReader::read_csv(std::string path, char delim) {
    std::ifstream fin(path);
    if (!fin.is_open()) {
        throw std::runtime_error("Can't open the file.");
    }
    std::string str;
    std::getline(fin, str, '\n');
    auto header = read_line(str, delim);
    header = multi_header(header);
    csvData csv;
    csv.set_header(header);
    while (std::getline(fin, str, '\n')) {
        auto line = read_line(str, delim);
        Row row(header);
        int mn = std::min(header.size(), line.size());
        for (int i = 0; i < mn; ++i) {
            row.push(header[i], line[i]);
        }
        for (int i = mn; i < header.size(); ++i) {
            row.push(header[i], "");
        }
        csv.extend(row);
    }
    return csv;
}

namespace sreader {
    auto read_line(std::string line, char delim) -> std::vector<std::string> {
        std::vector<std::string> ln;
        int len = line.length();
        for (int i = 0, j = 0; i < len; i = j + 1) {
            int st = i, ed = i;
            std::string s;
            if (line[i] == '"') {
                j = i + 1;
                while (j < len) {
                    if (line[j] == '"' && j + 1 != len && line[j + 1] == delim) break;
                    else if (line[j] == '"' && j + 1 == len) break;
                    if (line[j] == '"' && line[j + 1] == '"') {
                        s.push_back(line[j]);
                        j += 2;
                        continue;
                    } else s.push_back(line[j]);
                    ++j;
                }
                if (j == len) {
                    throw std::runtime_error("Missing paired quotes.");
                }
                ++j;
            } else {
                j = i;
                while (j < len && line[j] != delim) {
                    s.push_back(line[j]);
                    ++j;
                }
            }
            ln.push_back(s);
        }
        return ln;
    }
    auto read_csv(std::string path, char delim) -> std::map<std::string, std::vector<std::string>> {
        std::ifstream fin(path);
        if (!fin.is_open()) {
            throw std::runtime_error("Can't open the file.");
        }
        std::string column;
        std::getline(fin, column, '\n');
        auto cols = read_line(column, delim);
        std::string line;
        std::map<std::string, std::vector<std::string>> csv;
        while (std::getline(fin, line, '\n')) {
            auto row = read_line(line, delim);
            int mn = std::min(cols.size(), row.size());
            for (int i = 0; i < mn; ++i) {
                csv[cols[i]].push_back(row[i]);
            }
            for (int i = mn; i < cols.size(); ++i) csv[cols[i]].push_back("");
        }
        return csv;
    }
}

auto Double(std::string s) -> double {
    std::stringstream ss(s);
    double value;
    ss >> value;
    return value;
}

auto Float(std::string s) -> float {
    std::stringstream ss(s);
    float value;
    ss >> value;
    return value;
}

auto LLong(std::string s) -> long long {
    std::stringstream ss(s);
    long long value;
    ss >> value;
    return value;
}

auto Int(std::string s) -> int {
    std::stringstream ss(s);
    int value;
    ss >> value;
    return value;
}

}