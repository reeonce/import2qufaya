#include <cassert>
#include <iostream>
#include <string>

#include "qufaya.hpp"
#include "csv/section.hpp"

std::map<std::string, std::string> loadcategorymap(const std::string &path);

int main(int argc, char const *argv[]) {
    assert(argc > 1);

    std::string inpath = argv[1];
    std::string mappath = "map.csv";

    if (argc == 3) {
        mappath = argv[1];
        inpath = argv[2];
    }
    std::string outputpath = inpath + ".out.csv";

    auto catMap = loadcategorymap(mappath);
    Qufaya bill(catMap);
    bill.LoadWechatBill(inpath);
    bill.Save(outputpath);

    return 0;
}

std::map<std::string, std::string> loadcategorymap(const std::string &path) {
    csv::Section section = csv::Section::Load(path);
    std::map<std::string, std::string> map;
    for (const auto &row: section) {
        map[row[0]] = row[1];
    }
    return map;
}