// ----------------------------------------------------------------------
// TestMain.cpp
// ----------------------------------------------------------------------

#include "Tester.hpp"

TEST(Nominal, exposureTime) {
    Payload::Tester tester;
    tester.testExposureTime();
}

TEST(Nominal, imgConfig1) {
  Payload::Tester tester;
  tester.testImgConfiguration1();
}

TEST(Nominal, imgConfig2) {
  Payload::Tester tester;
  tester.testImgConfiguration2();
}

TEST(Nominal, imgConfig3) {
  Payload::Tester tester;
  tester.testImgConfiguration3();
}

TEST(Nominal, imgConfig4) {
  Payload::Tester tester;
  tester.testImgConfiguration4();
}

TEST(Nominal, cameraSave) {
  Payload::Tester tester;
  tester.testCameraActionSave();
}

TEST(Nominal, cameraProcess) {
  Payload::Tester tester;
  tester.testCameraActionProcess();
}

TEST(Nominal, invalidAction) {
  Payload::Tester tester;
  tester.testInvalidAction();
}

TEST(Nominal, invalidFormat) {
  Payload::Tester tester;
  tester.testInvalidFormat();
}

TEST(Nominal, invalidSize) {
  Payload::Tester tester;
  tester.testInvalidSize();
}

//TEST(nominal, SetFormatError){
//  Payload::Tester tester;
//  tester.testSetFormatError();
//}

TEST(Nominal, invalidExposureTime){
  Payload::Tester tester;
  tester.testInvalidTime();
}

TEST(Nominal, setup){
  Payload::Tester tester;
  tester.testSetup();
}

TEST(Nominal, setupError){
  Payload::Tester tester;
  tester.testSetupError();
}

TEST(Nominal, SetFormatError){
  Payload::Tester tester;
  tester.testSetFormatError();
}

TEST(Nominal, PartialRead){
  Payload::Tester tester;
  tester.testPartialImgCapture();
}

TEST(Nominal, InvalidFrame){
  Payload::Tester tester;
  tester.testInvalidFrame();
}

TEST(Nominal, RetryRead){
  Payload::Tester tester;
  tester.testRetryRead();
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    STest::Random::seed();
    return RUN_ALL_TESTS();
}
