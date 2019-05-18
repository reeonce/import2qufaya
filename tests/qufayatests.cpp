#include <iostream>

#include <ree/unittest.h>

#include "qufaya.hpp"
#include "csv/section.hpp"

static std::string wechat_cvs_path() {
    std::string srcpath = __FILE__;
    std::string dir = "";
    size_t pos = srcpath.rfind('/');
    if (pos != std::string::npos) {
        dir = srcpath.substr(0, pos + 1);
    }
    return dir + "wechat.csv";
}

R_TEST_F(qufaya, loadwechat) {
    std::map<std::string, std::string> map = {
        {"北京摩拜科技有限公司", "交通"},
        {"深圳通", "地铁"},
    };

    Qufaya q(map);
    q.LoadWechatBill(wechat_cvs_path());

    csv::Section &sec = *q.Content();

    R_ASSERT_EQ(sec.rows(), 3);
    R_ASSERT_EQ(sec.cols(), 5);

    sec.Save("output.csv");
}
