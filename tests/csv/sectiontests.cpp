#include <iostream>

#include <ree/unittest.h>

#include "csv/section.hpp"

static std::string cvs_path() {
    std::string srcpath = __FILE__;
    std::string dir = "";
    size_t pos = srcpath.rfind('/');
    if (pos != std::string::npos) {
        dir = srcpath.substr(0, pos + 1);
    }
    return dir + "sample.csv";
}

R_TEST_F(Section, Load) {
    csv::Section sec = csv::Section::Load(cvs_path());

    R_ASSERT_EQ(sec.rows(), 7);
    R_ASSERT_EQ(sec.cols(), 5);

    R_ASSERT_EQ(sec[0][0], std::string("h1"));
    R_ASSERT_EQ(sec[3][1], std::string("v32"));
}

R_TEST_F(Section, range) {
    csv::Section wsec = csv::Section::Load(cvs_path());
    R_ASSERT_EQ(wsec.rows(), 7);
    R_ASSERT_EQ(wsec.cols(), 5);

    csv::Section sec = wsec.range(1, 2, 3, 2);

    R_ASSERT_EQ(sec.rows(), 3);
    R_ASSERT_EQ(sec.cols(), 2);

    R_ASSERT_EQ(sec[0][0], std::string("v13"));
    R_ASSERT_EQ(sec[2][1], std::string("v34"));
}
