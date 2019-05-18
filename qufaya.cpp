#include "qufaya.hpp"

#include <algorithm>

#include <xlnt/xlnt.hpp>
#include "csv/section.hpp"

struct WechatConfig {
    size_t type;
    size_t time;
    size_t category;
    size_t amount;
    size_t comment;
};

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
        if (header[i] == "交易时间") {
            config.time = i;
        } else if (header[i] == "交易对方") {
            config.category = i;
        } else if (header[i] == "收/支") {
            config.type = i;
        } else if (header[i] == "金额(元)") {
            config.amount = i;
        } else if (header[i] == "备注") {
            config.comment = i;
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

    for (size_t i = 0; i < rows; ++i) {
        const csv::Row &wechat_row = wb[header_idx + 1 + i];
        csv::Row &q_row = sec[i + 1];
        q_row[0] = wechat_row[config.type];
        q_row[1] = wechat_row[config.time];
        std::string category = wechat_row[config.category]; 
        if (categoryMap_.find(category) == categoryMap_.end()) {
            category = "default";
        }
        q_row[2] = categoryMap_[category];

        std::string amount = wechat_row[config.amount];
        if (amount.find("¥") == 0) {
            amount = amount.substr(std::string("¥").length());
        }
        q_row[3] = amount;
        q_row[4] = wechat_row[config.category];
    }
}

void Qufaya::LoadAlipayBill(const std::string &path) {

}

void Qufaya::Save(const std::string &path) {
    sec_->Save(path);
}