// ======================================================================
// \title  Camera.hpp
// \author pel
// \brief  cpp file for Camera test harness implementation class
// ======================================================================

#include "Tester.hpp"

#define INSTANCE 0
#define MAX_HISTORY_SIZE 100
#define QUEUE_DEPTH 10

const char *testDeviceName = 0;
int open_value = 0;
int init_value = 0;
int format_value = 0;
int frame_value = 0;
int fd = 0;
int testSize = 0;

// enum ImgResolution { SIZE_640x480 = 0 , SIZE_800x600 = 1,  ERROR};
//
// enum InvalidResolution { SIZE_350x480 };

namespace Payload {

// ----------------------------------------------------------------------
// Construction and destruction
// ----------------------------------------------------------------------

Tester ::Tester()
    : CameraGTestBase("Tester", MAX_HISTORY_SIZE), component("Camera") {
  this->initComponents();
  this->connectPorts();
  this->component.open(testDeviceName);
}

Tester ::~Tester() {}

// ----------------------------------------------------------------------
// Tests
// ----------------------------------------------------------------------

void Tester::testImgConfiguration1() {
  ColorFormat imgFormat =  ColorFormat::RGB;
  ImgResolution imgResolution = ImgResolution::SIZE_640x480;

  this->sendCmd_ConfigImg(0, 0, imgResolution, imgFormat);
  this->component.doDispatch();

  ASSERT_CMD_RESPONSE_SIZE(1);
  ASSERT_CMD_RESPONSE(0, Camera::OPCODE_CONFIGIMG, 0, Fw::CmdResponse::OK);
  ASSERT_EVENTS_SIZE(1);
  ASSERT_EVENTS_SetImgConfig(0, imgResolution, imgFormat);
  ASSERT_TLM_SIZE(1);
  ASSERT_TLM_commandNum_SIZE(1);
  this->clearHistory();
}


void Tester::testImgConfiguration2() {
  ColorFormat imgFormat =  ColorFormat::YUYV;
  ImgResolution imgResolution = ImgResolution::SIZE_640x480;

  this->sendCmd_ConfigImg(0, 0, imgResolution, imgFormat);
  this->component.doDispatch();

  ASSERT_CMD_RESPONSE_SIZE(1);
  ASSERT_CMD_RESPONSE(0, Camera::OPCODE_CONFIGIMG, 0, Fw::CmdResponse::OK);
  ASSERT_EVENTS_SIZE(1);
  ASSERT_EVENTS_SetImgConfig(0, imgResolution, imgFormat);
  ASSERT_TLM_SIZE(1);
  ASSERT_TLM_commandNum_SIZE(1);
  this->clearHistory();
}

void Tester::testImgConfiguration3() {
  ColorFormat imgFormat =  ColorFormat::RGB;
  ImgResolution imgResolution = ImgResolution::SIZE_800x600;

  this->sendCmd_ConfigImg(0, 0, imgResolution, imgFormat);
  this->component.doDispatch();

  ASSERT_CMD_RESPONSE_SIZE(1);
  ASSERT_CMD_RESPONSE(0, Camera::OPCODE_CONFIGIMG, 0, Fw::CmdResponse::OK);
  ASSERT_EVENTS_SIZE(1);
  ASSERT_EVENTS_SetImgConfig(0, imgResolution, imgFormat);
  ASSERT_TLM_SIZE(1);
  ASSERT_TLM_commandNum_SIZE(1);
  this->clearHistory();
}

void Tester::testImgConfiguration4() {
  ColorFormat imgFormat = ColorFormat::YUYV;
  ImgResolution imgResolution = ImgResolution::SIZE_800x600;

  this->sendCmd_ConfigImg(0, 0, imgResolution, imgFormat);
  this->component.doDispatch();

  ASSERT_CMD_RESPONSE_SIZE(1);
  ASSERT_CMD_RESPONSE(0, Camera::OPCODE_CONFIGIMG, 0, Fw::CmdResponse::OK);
  ASSERT_EVENTS_SIZE(1);
  ASSERT_EVENTS_SetImgConfig(0, imgResolution, imgFormat);
  ASSERT_TLM_SIZE(1);
  ASSERT_TLM_commandNum_SIZE(1);
  this->clearHistory();
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
}

void Tester::testCameraActionSave(){
  CameraAction cameraAction = CameraAction::SAVE;
  this->sendCmd_TakeAction(0, 0, cameraAction);
  this->component.doDispatch();

  ASSERT_CMD_RESPONSE_SIZE(1);
  ASSERT_CMD_RESPONSE(0, Camera::OPCODE_TAKEACTION, 0, Fw::CmdResponse::OK);
  ASSERT_TLM_SIZE(2);
  ASSERT_TLM_commandNum_SIZE(1);
  ASSERT_TLM_photosTaken_SIZE(1);
  ASSERT_EVENTS_SIZE(1);
  ASSERT_EVENTS_CameraSave_SIZE(1);
  this->clearHistory();
}

void Tester::testCameraActionProcess() {
  CameraAction cameraAction = CameraAction::PROCESS;

  this->sendCmd_TakeAction(0, 0, cameraAction);
  this->component.doDispatch();

  ASSERT_CMD_RESPONSE_SIZE(1);
  ASSERT_CMD_RESPONSE(0, Camera::OPCODE_TAKEACTION, 0, Fw::CmdResponse::OK);
  ASSERT_TLM_SIZE(2);
  ASSERT_TLM_commandNum_SIZE(1);
  ASSERT_TLM_photosTaken_SIZE(1);
  this->clearHistory();
}

void Tester::testInvalidAction() {
  CameraAction invalidAction = static_cast<const CameraAction::t>(2);
  this->sendCmd_TakeAction(0, 0, invalidAction);
  this->component.doDispatch();

  ASSERT_CMD_RESPONSE_SIZE(1);
  ASSERT_CMD_RESPONSE(0, Camera::OPCODE_TAKEACTION, 0,
                      Fw::CmdResponse::EXECUTION_ERROR);
  this->clearHistory();
}


void Tester::testInvalidFormat() {
  ColorFormat invalidFormat = static_cast<const ColorFormat::t>(3);
  ImgResolution imgResolution = ImgResolution::SIZE_800x600;
  this->sendCmd_ConfigImg(0, 0, imgResolution, invalidFormat);
  this->component.doDispatch();

  ASSERT_EVENTS_SIZE(1);
  ASSERT_EVENTS_InvalidFormatCmd_SIZE(1);
  ASSERT_EVENTS_InvalidFormatCmd(0, invalidFormat);
  ASSERT_CMD_RESPONSE_SIZE(1);
  ASSERT_CMD_RESPONSE(0, Camera::OPCODE_CONFIGIMG, 0,
                      Fw::CmdResponse::EXECUTION_ERROR);
  this->clearHistory();
}

void Tester::testInvalidSize() {
  ImgResolution invalidResolution = static_cast<const ImgResolution::t>(2);
  ColorFormat imgFormat =  ColorFormat::RGB;
  this->sendCmd_ConfigImg(0, 0, invalidResolution, imgFormat);
  this->component.doDispatch();

  ASSERT_EVENTS_SIZE(1);
  ASSERT_EVENTS_InvalidSizeCmd_SIZE(1);
  ASSERT_EVENTS_InvalidSizeCmd(0, invalidResolution);
  ASSERT_CMD_RESPONSE_SIZE(1);
  ASSERT_CMD_RESPONSE(0, Camera::OPCODE_CONFIGIMG, 0,
                      Fw::CmdResponse::EXECUTION_ERROR);
  this->clearHistory();
  this->clearEvents();
}

void Tester::testInvalidTime() {
  NATIVE_UINT_TYPE time = 100001;
  this->sendCmd_ExposureTime(0, 0, time);
  this->component.doDispatch();

  ASSERT_EVENTS_SIZE(1);
  ASSERT_EVENTS_InvalidExposureTimeCmd(0, time);
  ASSERT_EVENTS_InvalidExposureTimeCmd_SIZE(1);
  this->clearHistory();
  this->clearEvents();
}

void Tester::testSetup() {
  component.open("dev/videoTEST");
  ASSERT_EQ(testDeviceName, "dev/videoTEST");
  this->clearHistory();
}

void Tester::testSetupError() {
  open_value = -1;
  component.open("dev/Err");
  ASSERT_EVENTS_SIZE(1);
  ASSERT_EVENTS_CameraOpenError_SIZE(1);
  ASSERT_EVENTS_CameraOpenError(0, "dev/Err");
  this->clearHistory();
  open_value = 0;
}

void Tester::testSetFormatError() {
  ColorFormat imgFormat =  ColorFormat::YUYV;
  ImgResolution imgResolution = ImgResolution::SIZE_800x600;
  open_value = 1;
  format_value = -1;
  this->sendCmd_ConfigImg(0, 0, imgResolution, imgFormat);
  component.doDispatch();

  ASSERT_EVENTS_SetFormatError_SIZE(1);
  ASSERT_EVENTS_SetFormatError(0, format_value);
  ASSERT_EVENTS_SIZE(1);
  this->clearHistory();
}

void Tester::testPartialImgCapture() {
  ColorFormat imgFormat =  ColorFormat::YUYV;
  ImgResolution imgResolution = ImgResolution::SIZE_800x600;
  testSize = 4;
  this->sendCmd_ConfigImg(0, 0, imgResolution, imgFormat);
  component.doDispatch();
  component.readImage();
  ASSERT_EVENTS_partialImgCapture_SIZE(1);
  ASSERT_EVENTS_partialImgCapture(0, testSize);
}

void Tester::testInvalidFrame() {
  frame_value = -1;
  component.readImage();
  ASSERT_EVENTS_SIZE(1);
  ASSERT_EVENTS_invalidFrame_SIZE(1);
  ASSERT_EVENTS_invalidFrame(0, testSize);
  this->clearHistory();
}

void Tester::testRetryRead() {
  frame_value = -2;
  component.readImage();
  ASSERT_EVENTS_retryRead_SIZE(1);
}

// ----------------------------------------------------------------------
// Handlers for typed from ports
// ----------------------------------------------------------------------

Fw::Buffer Tester ::from_allocate_handler(const NATIVE_INT_TYPE portNum,
                                          U32 size) {
  this->pushFromPortEntry_allocate(size);
  Fw::Buffer buffer(new U8[size], size);
  return buffer;
}

void Tester ::from_process_handler(const NATIVE_INT_TYPE portNum,
                                   Fw::Buffer &fwBuffer) {
  this->pushFromPortEntry_process(fwBuffer);
}

void Tester ::from_save_handler(const NATIVE_INT_TYPE portNum,
                                Fw::Buffer &fwBuffer) {
  this->pushFromPortEntry_save(fwBuffer);
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

  // process
  this->component.set_process_OutputPort(0, this->get_from_process(0));

  // save
  this->component.set_save_OutputPort(0, this->get_from_save(0));
}

void Tester ::initComponents() {
  this->init();
  this->component.init(QUEUE_DEPTH, INSTANCE);
}

NATIVE_UINT_TYPE Tester::pickExposureTime() {
  NATIVE_UINT_TYPE time = STest::Pick::lowerUpper(0, 10);
  return time;
}

} // end namespace Payload
