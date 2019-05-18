#pragma once

#include <map>
#include <string>
#include <memory>

namespace csv {
class Section;
}

class Qufaya {
public:
    Qufaya(const std::map<std::string, std::string> &categoryMap);
    ~Qufaya();

    void LoadWechatBill(const std::string &path);

    void LoadAlipayBill(const std::string &path);

    void Save(const std::string &path);

    csv::Section *Content() const { return sec_.get(); }

private:
    std::map<std::string, std::string> categoryMap_;
    std::unique_ptr<csv::Section> sec_;
};