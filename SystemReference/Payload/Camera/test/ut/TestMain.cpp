// ----------------------------------------------------------------------
// TestMain.cpp
// ----------------------------------------------------------------------

#include "Tester.hpp"

//TEST(Nominal, exposureTime) {
//    Payload::Tester tester;
//    tester.testExposureTime();
//}

TEST(Nominal, imgConfig) {
  Payload::Tester tester;
  tester.testImgConfiguration();
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    STest::Random::seed();
    return RUN_ALL_TESTS();
}
