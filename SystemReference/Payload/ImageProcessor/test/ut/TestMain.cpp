// ----------------------------------------------------------------------
// TestMain.cpp
// ----------------------------------------------------------------------

#include "Tester.hpp"

TEST(Nominal, testJpgFormat) {
    Payload::Tester tester;
    tester.testSetFormatJpg();
}

TEST(Nominal, testPngFormat) {
  Payload::Tester tester;
  tester.testSetFormatPng();
}

TEST(Nominal, testProcess) {
  Payload::Tester tester;
  tester.testImgProcess();
}

TEST(Nominal, testBadBuffer) {
  Payload::Tester tester;
  tester.testBadBufferProcess();
}

TEST(Nominal, testNoImgData) {
  Payload::Tester tester;
  tester.testNoImgData();
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
