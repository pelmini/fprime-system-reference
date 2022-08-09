// ----------------------------------------------------------------------
// TestMain.cpp
// ----------------------------------------------------------------------

#include "Tester.hpp"

TEST(Nominal, exposureTime) {
    Payload::Tester tester;
    tester.testExposureTime();
}

TEST(Nominal, imgConfig) {
  Payload::Tester tester;
  tester.testImgConfiguration();
}

TEST(Nominal, savePhoto) {
  Payload::Tester tester;
  tester.testSavePhoto();
}

TEST(Nominal, takePhoto) {
  Payload::Tester tester;
  tester.testTakePhoto();
}

TEST(nominal, invalidFormat) {
  Payload::Tester tester;
  tester.testInvalidFormat();
}

TEST(nominal, invalidSize) {
  Payload::Tester tester;
  tester.testInvalidSize();
}

TEST(nominal, invalidExposureTime){
  Payload::Tester tester;
  tester.testInvalidTime();
}

TEST(nominal, setup){
  Payload::Tester tester;
  tester.testSetup();
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    STest::Random::seed();
    return RUN_ALL_TESTS();
}
