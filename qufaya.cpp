#include "qufaya.hpp"

#include <algorithm>
#include <iostream>
#include <set>

// #include <xlnt/xlnt.hpp>
#include "csv/section.hpp"

struct WechatConfig {
    size_t type;
    size_t time;
    size_t category;
    size_t amount;
    size_t comment;
    size_t product;
};

struct AlipayConfig {
    size_t type;
    size_t time;
    size_t seller;
    size_t product;
    size_t amount;
    size_t comment;
};


std::string trim(const std::string& str, const std::string& whitespace = " \t") {
    const auto strBegin = str.find_first_not_of(whitespace);
    if (strBegin == std::string::npos)
        return ""; // no content

    const auto strEnd = str.find_last_not_of(whitespace);
    const auto strRange = strEnd - strBegin + 1;

    return str.substr(strBegin, strRange);
}

Qufaya::Qufaya(const std::map<std::string, std::string> &categoryMap)
  : categoryMap_(categoryMap) {
}

Qufaya::~Qufaya() {}

void Qufaya::LoadWechatBill(const std::string &path) {
    csv::Section wb = csv::Section::Load(path);

    size_t header_idx = 0;
    for (size_t i = 0; i < wb.rows(); ++i) {
        const csv::Row &row = wb[i];
        if (row[0].find("微信支付账单明细列表") != std::string::npos) {
            header_idx = i + 1;
        }
    }

    const csv::Row &header = wb[header_idx];

    WechatConfig config;
    for (size_t i = 0; i < header.size(); ++i) {
        std::string header_item = trim(header[i]);
        if (header_item == "交易时间") {
            config.time = i;
        } else if (header_item == "交易对方") {
            config.category = i;
        } else if (header_item == "收/支") {
            config.type = i;
        } else if (header_item == "金额(元)") {
            config.amount = i;
        } else if (header_item == "备注") {
            config.comment = i;
        } else if (header_item == "商品") {
            config.product = i;
        }
    }

    size_t rows = wb.rows() - header_idx - 1;

    sec_.reset(new csv::Section(rows + 1, 5));
    csv::Section &sec = *sec_;

    csv::Row &header_row = sec[0];
    header_row[0] = "类型";
    header_row[1] = "时间";
    header_row[2] = "类别";
    header_row[3] = "金额";
    header_row[4] = "备注";


    std::map<std::string, std::string> unknow_set;

    for (size_t i = 0; i < rows; ++i) {
        const csv::Row &wechat_row = wb[header_idx + 1 + i];
        csv::Row &q_row = sec[i + 1];
        q_row[0] = wechat_row[config.type];
        q_row[1] = wechat_row[config.time];
        std::string category = wechat_row[config.category]; 
        if (categoryMap_.find(category) == categoryMap_.end()) {
            if (unknow_set.find(category) == unknow_set.end()) {
                unknow_set[category] = trim(wechat_row[config.product]) + ";";
            } else {
                unknow_set[category] += std::string(trim(wechat_row[config.product]) + ";");
            }

            category = "default";
        }
        q_row[2] = categoryMap_[category];

        std::string amount = wechat_row[config.amount];
        if (amount.find("¥") == 0) {
            amount = amount.substr(std::string("¥").length());
        }
        q_row[3] = amount;
        q_row[4] = wechat_row[config.category];


        if (categoryMap_.find(category) == categoryMap_.end() &&
            wechat_row[config.product] != "/") {
            q_row[4] = wechat_row[config.category] + "-" + wechat_row[config.product];
        }
    }

    for (auto &unknow_cat : unknow_set) {
        std::cout << unknow_cat.first << ":" << unknow_cat.second << "\n";
    }
}

void Qufaya::LoadAlipayBill(const std::string &path) {
    csv::Section wb = csv::Section::Load(path);

    size_t header_idx = 0;
    for (size_t i = 0; i < wb.rows(); ++i) {
        const csv::Row &row = wb[i];
        if (row[0].find("交易记录明细列表") != std::string::npos) {
            header_idx = i + 1;
        }
    }

    const csv::Row &header = wb[header_idx];

    AlipayConfig config;
    for (size_t i = 0; i < header.size(); ++i) {
        std::string header_item = trim(header[i]);
        if (header_item == "交易创建时间") {
            config.time = i;
        } else if (header_item == "交易对方") {
            config.seller = i;
        } else if (header_item == "商品名称") {
            config.product = i;
        } else if (header_item == "收/支") {
            config.type = i;
        } else if (header_item == "金额（元）") {
            config.amount = i;
        } else if (header_item == "备注") {
            config.comment = i;
        }
    }

    size_t rows = wb.rows() - header_idx - 1;

    sec_.reset(new csv::Section(0, 5));
    csv::Section &sec = *sec_;

    csv::Row header_row(5);
    header_row[0] = "类型";
    header_row[1] = "时间";
    header_row[2] = "类别";
    header_row[3] = "金额";
    header_row[4] = "备注";
    sec.Append(header_row);

    std::map<std::string, std::string> unknow_set;

    for (size_t i = 0; i < rows; ++i) {
        const csv::Row &ali_row = wb[header_idx + 1 + i];
        if (ali_row.size() < header.size()) {
            continue;
        }
        std::string type = trim(ali_row[config.type]);
        if (type != "支出") {
            continue;
        }
        std::string amount_str = trim(ali_row[config.amount]);
        double amount = std::stod(amount_str);
        if (amount <= 0.1) {
            continue;
        }

        std::string description = trim(ali_row[config.product]);
        std::string category = trim(ali_row[config.seller]); 
        if (categoryMap_.find(category) == categoryMap_.end() ||
            categoryMap_[category].empty()) {
            description += (std::string(" - ") + trim(ali_row[config.seller]));
            if (unknow_set.find(category) == unknow_set.end()) {
                unknow_set[category] = trim(ali_row[config.product]) + ";";
            } else {
                unknow_set[category] += std::string(trim(ali_row[config.product]) + ";");
            }
            category = "default";
        }

        csv::Row q_row(5);
        q_row[0] = type;
        q_row[1] = trim(ali_row[config.time]);
        q_row[2] = categoryMap_[category];
        q_row[3] = amount_str;
        q_row[4] = description;
        sec.Append(q_row);
    }

    for (auto &unknow_cat : unknow_set) {
        std::cout << unknow_cat.first << ":" << unknow_cat.second << "\n";
    }
}

void Qufaya::Save(const std::string &path) {
    sec_->Save(path);
}
