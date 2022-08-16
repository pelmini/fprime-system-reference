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

TEST(nominal, invalidFormat) {
  Payload::Tester tester;
  tester.testInvalidFormat();
}

TEST(nominal, invalidSize) {
  Payload::Tester tester;
  tester.testInvalidSize();
}

//TEST(nominal, SetFormatError){
//  Payload::Tester tester;
//  tester.testSetFormatError();
//}

TEST(nominal, invalidExposureTime){
  Payload::Tester tester;
  tester.testInvalidTime();
}

TEST(nominal, setup){
  Payload::Tester tester;
  tester.testSetup();
}

TEST(nominal, setupError){
  Payload::Tester tester;
  tester.testSetupError();
}

TEST(nominal, SetFormatError){
  Payload::Tester tester;
  tester.testSetFormatError();
}

TEST(nominal, PartialRead){
  Payload::Tester tester;
  tester.testPartialImgCapture();
}

TEST(nominal, InvalidFrame){
  Payload::Tester tester;
  tester.testInvalidFrame();
}

TEST(nominal, RetryRead){
  Payload::Tester tester;
  tester.testRetryRead();
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    STest::Random::seed();
    return RUN_ALL_TESTS();
}
