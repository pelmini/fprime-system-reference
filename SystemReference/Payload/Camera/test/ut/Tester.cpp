// ======================================================================
// \title  Camera.hpp
// \author pel
// \brief  cpp file for Camera test harness implementation class
// ======================================================================

#include "Tester.hpp"

#define INSTANCE 0
#define MAX_HISTORY_SIZE 100
#define QUEUE_DEPTH 10

const char *testDeviceName;

//enum ImgResolution { SIZE_640x480 = 0 , SIZE_800x600 = 1,  ERROR};
//
//enum InvalidResolution { SIZE_350x480 };

namespace Payload {

// ----------------------------------------------------------------------
// Construction and destruction
// ----------------------------------------------------------------------

Tester ::Tester()
    : CameraGTestBase("Tester", MAX_HISTORY_SIZE), component("Camera") {
  this->initComponents();
  this->connectPorts();
  this->component.open(testDeviceName, 0, 0);
}

Tester ::~Tester() {}

// ----------------------------------------------------------------------
// Tests
// ----------------------------------------------------------------------

void Tester::testImgConfiguration() {
  ImgFormat imgFormat = pickImgFormat();
  ImgResolution imgResolution = pickImgResolution();

  this->sendCmd_ConfigImg(0, 0, imgResolution, imgFormat);
  this->component.doDispatch();

  ASSERT_CMD_RESPONSE_SIZE(1);
  ASSERT_CMD_RESPONSE(0, Camera::OPCODE_CONFIGIMG, 0, Fw::CmdResponse::OK);
  ASSERT_EVENTS_SIZE(1);
  ASSERT_EVENTS_SetImgConfig(0, imgResolution, imgFormat);
  ASSERT_TLM_SIZE(1);
  ASSERT_TLM_commandNum_SIZE(1);
  this->clearHistory();
  this->clearEvents();
}

void Tester::testExposureTime() {
  NATIVE_UINT_TYPE time;
  time = pickExposureTime();

  this->sendCmd_ExposureTime(0, 0, time);
  this->component.doDispatch();

  ASSERT_CMD_RESPONSE_SIZE(1);
  ASSERT_CMD_RESPONSE(0, Camera::OPCODE_EXPOSURETIME, 0, Fw::CmdResponse::OK);
  ASSERT_EVENTS_SIZE(1);
  ASSERT_EVENTS_ExposureTimeSet(0, time);
  ASSERT_TLM_SIZE(1);
  ASSERT_TLM_commandNum_SIZE(1);
  this->clearHistory();
  this->clearEvents();
}

void Tester::testSavePhoto() {
  this->sendCmd_Save(0, 0);
  this->component.doDispatch();

  ASSERT_CMD_RESPONSE_SIZE(1);
  ASSERT_CMD_RESPONSE(0, Camera::OPCODE_SAVE, 0, Fw::CmdResponse::OK);
  ASSERT_TLM_SIZE(1);
  ASSERT_TLM_commandNum_SIZE(1);
  this->clearHistory();
  this->clearEvents();
}

void Tester::testTakePhoto() {
  this->sendCmd_Take(0, 0);
  this->component.doDispatch();

  ASSERT_CMD_RESPONSE_SIZE(1);
  ASSERT_CMD_RESPONSE(0, Camera::OPCODE_TAKE, 0, Fw::CmdResponse::OK);
  ASSERT_TLM_SIZE(2);
  ASSERT_TLM_commandNum_SIZE(1);
  ASSERT_TLM_photosTaken_SIZE(1);
  this->clearHistory();
  this->clearEvents();
}

void Tester::testInvalidFormat() {
  ImgFormat invalidFormat = static_cast<const ImgFormat::t>(3);
  ImgResolution imgResolution = pickImgResolution();
  this->sendCmd_ConfigImg(0, 0, imgResolution,
                          invalidFormat);
  this->component.doDispatch();

  ASSERT_EVENTS_SIZE(1);
  ASSERT_EVENTS_InvalidFormat_SIZE(1);
  ASSERT_EVENTS_InvalidFormat(0, invalidFormat);
  this->clearHistory();
  this->clearEvents();
}

void Tester::testInvalidSize() {
  ImgResolution invalidResolution = static_cast<const ImgResolution::t>(2);
  ImgFormat imgFormat = pickImgFormat();
  this->sendCmd_ConfigImg(0, 0, invalidResolution, imgFormat);
  this->component.doDispatch();

  ASSERT_EVENTS_SIZE(1);
  ASSERT_EVENTS_InvalidSize_SIZE(1);
  ASSERT_EVENTS_InvalidSize(0, invalidResolution);
  this->clearHistory();
  this->clearEvents();
}

void Tester::testInvalidTime() {
  NATIVE_UINT_TYPE time = 100001;
  this->sendCmd_ExposureTime(0, 0, time);
  this->component.doDispatch();

  ASSERT_EVENTS_SIZE(1);
  ASSERT_EVENTS_InvalidExposureTime(0, time);
  ASSERT_EVENTS_InvalidExposureTime_SIZE(1);
  this->clearHistory();
  this->clearEvents();
}

void Tester::testSetup() {
  component.open("dev/videoTEST", 0, 0);
  ASSERT_EQ(testDeviceName, "dev/videoTEST");
}

//void Tester::testSetupError(){
//  open_value = -1;
//
//}
// ----------------------------------------------------------------------
// Handlers for typed from ports
// ----------------------------------------------------------------------

Fw::Buffer Tester ::from_allocate_handler(const NATIVE_INT_TYPE portNum,
                                          U32 size) {
  this->pushFromPortEntry_allocate(size);
  Fw::Buffer buffer(new U8[size], size);
  return buffer;
}

void Tester ::from_deallocate_handler(const NATIVE_INT_TYPE portNum,
                                      Fw::Buffer &fwBuffer) {
  this->pushFromPortEntry_deallocate(fwBuffer);
}

void Tester ::from_sendPhoto_handler(const NATIVE_INT_TYPE portNum,
                                     Fw::Buffer &fwBuffer) {
  this->pushFromPortEntry_sendPhoto(fwBuffer);
  ASSERT_from_sendPhoto(0, fwBuffer);
}

// ----------------------------------------------------------------------
// Helper methods
// ----------------------------------------------------------------------

void Tester ::connectPorts() {

  // cmdIn
  this->connect_to_cmdIn(0, this->component.get_cmdIn_InputPort(0));

  // Log
  this->component.set_Log_OutputPort(0, this->get_from_Log(0));

  // LogText
  this->component.set_LogText_OutputPort(0, this->get_from_LogText(0));

  // Time
  this->component.set_Time_OutputPort(0, this->get_from_Time(0));

  // Tlm
  this->component.set_Tlm_OutputPort(0, this->get_from_Tlm(0));

  // allocate
  this->component.set_allocate_OutputPort(0, this->get_from_allocate(0));

  // cmdRegOut
  this->component.set_cmdRegOut_OutputPort(0, this->get_from_cmdRegOut(0));

  // cmdResponseOut
  this->component.set_cmdResponseOut_OutputPort(
      0, this->get_from_cmdResponseOut(0));

  // deallocate
  this->component.set_deallocate_OutputPort(0, this->get_from_deallocate(0));

  // sendPhoto
  this->component.set_sendPhoto_OutputPort(0, this->get_from_sendPhoto(0));
}

void Tester ::initComponents() {
  this->init();
  this->component.init(QUEUE_DEPTH, INSTANCE);
}

ImgResolution Tester::pickImgResolution() {
  const ImgResolution imgResolution =
      static_cast<const ImgResolution::t>(STest::Pick::lowerUpper(0, 1));

  if (imgResolution.e == 0) {
    FW_ASSERT(imgResolution == ImgResolution::SIZE_640x480);
  } else if (imgResolution.e == 1) {
    FW_ASSERT(imgResolution == ImgResolution::SIZE_800x600);
  }
  return imgResolution;
}

ImgFormat Tester::pickImgFormat() {
  const ImgFormat imgFormat =
      static_cast<const ImgFormat::t>(STest::Pick::lowerUpper(0, 1));

  if (imgFormat.e == 0) {
    FW_ASSERT(imgFormat == ImgFormat::RGB);
  } else if (imgFormat.e == 1) {
    FW_ASSERT(imgFormat == ImgFormat::YUYV);
  }
  return imgFormat;
}

NATIVE_UINT_TYPE Tester::pickExposureTime() {
  NATIVE_UINT_TYPE time = STest::Pick::lowerUpper(0, 10);
  return time;
}

} // end namespace Payload
