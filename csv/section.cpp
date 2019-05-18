#include "section.hpp"

#include <fstream>
#include <string>

namespace csv {

Row::Row() : Row(0) {}
Row::Row(size_t cols) : values_(std::vector<Cell>(cols)) {}
Row::Row(const std::vector<Cell> &values) : values_(values) {}
Row::Row(std::vector<Cell> &&values) : values_(values) {}

Row Row::range(size_t begin, size_t end) const {
    std::vector<Cell> values(end - begin);
    std::copy(values_.begin() + begin, values_.begin() + end, values.begin());
    return Row(std::move(values));
}

Section Section::Load(const std::string &path) {
    Section sec;

    std::ifstream is(path);
    if (!is) {
        return sec;
    }

    std::string line;
    while(std::getline(is, line)) {
        size_t pos = 0;
        std::vector<Cell> cells;
        while ((pos = line.find(',')) != std::string::npos) {
            cells.push_back(line.substr(0, pos));
            line.erase(0, pos + 1);
        }
        cells.push_back(line.substr(0, pos));
        sec.Append(Row(cells));
    }

    return sec;
}

Section::Section() : Section(0, 0) {}
Section::Section(size_t rows, size_t cols)
    : rows_(std::vector<Row>(rows)),
      colsize_(cols) {
    for (size_t i = 0; i < rows; i++) {
        rows_[i] = Row(cols);
    }
}

Section Section::range(size_t row, size_t col, size_t rows, size_t cols) {
    Section sec(rows, cols);
    for (size_t i = 0; i < rows; i++) {
        const Row &originrow = rows_[i + row];
        Row newcol(cols);
        for (size_t j = 0; j < cols; j++) {
            newcol[j] = originrow[col + j];
        }
        sec[i] = newcol;
    }
    return sec;
}

void Section::Append(const Row &row) {
    if (colsize_ < row.size()) {
        colsize_ = row.size();
    }
    rows_.push_back(row);
}
void Section::Append(const Column &column) {
    // TODO:
}

void Section::Save(const std::string &path) {
    std::ofstream os(path);
    for (const Row &row: rows_) {
        for (size_t col = 0; col < row.size() - 1; col++) {
            os << row[col] << ",";
        }
        os << row[row.size() - 1] << "\n";
    }
}

}
