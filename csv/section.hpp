#pragma once

#include <string>
#include <vector>

namespace csv {

using Cell = std::string;

class Row {

public:
    Row();
    Row(size_t cols);
    Row(const std::vector<Cell> &values);
    Row(std::vector<Cell> &&values);
    Row(const Row &o) = default;
    Row(Row &&o) = default;

    Row &operator=(const Row &o) = default;
    Row &operator=(Row &&o) = default;

    Cell &operator[](size_t idx) { return values_[idx]; }
    const Cell &operator[](size_t idx) const { return values_[idx]; }

    std::vector<Cell>::const_iterator begin() const { return values_.begin(); }
    std::vector<Cell>::const_iterator end() const { return values_.end(); }

    std::vector<Cell>::iterator begin() { return values_.begin(); }
    std::vector<Cell>::iterator end() { return values_.end(); }

    size_t size() const { return values_.size(); }

    Row range(size_t begin, size_t end) const;

private:
    std::vector<Cell> values_;

};

class Column {

public:
    Cell &operator[](size_t idx) { return values_[idx]; }
    const Cell &operator[](size_t idx) const { return values_[idx]; }

    std::vector<Cell>::const_iterator begin() const { return values_.begin(); }
    std::vector<Cell>::const_iterator end() const { return values_.end(); }

    std::vector<Cell>::iterator begin() { return values_.begin(); }
    std::vector<Cell>::iterator end() { return values_.end(); }

    size_t size() const { return values_.size(); }

    Column range(size_t begin, size_t end) const;

private:
    std::vector<Cell> values_;

};

class Section {

public:
    static Section Load(const std::string &path);

    Section();
    Section(size_t rows, size_t cols);


    Row &operator[](size_t idx) {
        return rows_[idx];
    }
    const Row &operator[](size_t idx) const {
        return rows_[idx];
    }

    std::vector<Row>::const_iterator begin() const { return rows_.begin(); }
    std::vector<Row>::const_iterator end() const { return rows_.end(); }

    std::vector<Row>::iterator begin() { return rows_.begin(); }
    std::vector<Row>::iterator end() { return rows_.end(); }

    size_t rows() const { return rows_.size(); }
    size_t cols() const { return colsize_; }

    Section range(size_t row, size_t col, size_t rows, size_t cols);
    Section rowrange(size_t row, size_t rows);
    Section colrange(size_t col, size_t cols);
    void Append(const Row &row);
    void Append(const Column &column);

    void Save(const std::string &path);

private:
    std::vector<Row> rows_;
    size_t colsize_;
};

}
