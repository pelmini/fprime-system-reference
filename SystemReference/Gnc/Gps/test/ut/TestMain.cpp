// ----------------------------------------------------------------------
// TestMain.cpp
// ----------------------------------------------------------------------

#include "Tester.hpp"

//TEST(Nominal, gpsData) {
//    Gnc::Tester tester;
//    tester.testGetGpsData();
//}
//
//TEST(Nominal, gpsData2) {
//  Gnc::Tester tester;
//  tester.testGetGpsData2();
//}
//
//TEST(Nominal, lockLost) {
//  Gnc::Tester tester;
//  tester.testLockLost();
//}
//
//TEST(Nominal, lockStatusLost) {
//  Gnc::Tester tester;
//  tester.testLockStatusCmdLost();
//}
//
//TEST(Nominal, lockStatusAcquired) {
//  Gnc::Tester tester;
//  tester.testLockStatusCmdAcquired();
//}
//
//TEST(Nominal, PacketError) {
//  Gnc::Tester tester;
//  tester.testBadGPSPacket();
//}

TEST(Nominal, BufferError) {
  Gnc::Tester tester;
  tester.testParseFail();
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
