// ======================================================================
// \title  Camera.cpp
// \author vbai
// \brief  cpp file for Camera component implementation class
// ======================================================================

#include "Fw/Types/BasicTypes.hpp"
#include "opencv2/opencv.hpp"
#include <SystemReference/Payload/Camera/Camera.hpp>

namespace Payload {

// ----------------------------------------------------------------------
// Construction, initialization, and destruction
// ----------------------------------------------------------------------
const int MAX_EXPOSURE_TIME = 100000;

Camera ::Camera(const char *const compName)
    : CameraComponentBase(compName), m_cmdCount(0), m_photoCount(0),
      m_validCommand(true) {}

void Camera ::init(const NATIVE_INT_TYPE queueDepth,
                   const NATIVE_INT_TYPE instance) {
  CameraComponentBase::init(queueDepth, instance);
}

bool Camera::open() {
  // open default video camera based on device index
  m_capture.open(0);
  if (!m_capture.isOpened()) {
    this->log_WARNING_HI_CameraOpenError();
    return false;
  }
  return true;
}

Camera ::~Camera() {
  m_capture.release();
}

// ----------------------------------------------------------------------
// Command handler implementations
// ----------------------------------------------------------------------

void Camera ::TakeAction_cmdHandler(const FwOpcodeType opCode, const U32 cmdSeq,
                                    Payload::CameraAction cameraAction) {
  RawImageData rawImageData;
  std::vector<uchar> buffer;
  Fw::Buffer imgBuffer;
  cv::Mat frame;

  while (true) {
    m_capture.read(frame);

    if (frame.empty()) {
      this->log_WARNING_HI_BlankFrame();
      m_validCommand = false;
      break;
    }

    // Set up buffer for image data
    U32 imgSize = frame.rows*frame.cols*frame.channels();
    imgBuffer = allocate_out(0, imgSize);
    FW_ASSERT(imgBuffer.getSize() > 0);
    memcpy(imgBuffer.getData(), frame.data, imgSize);
    imgBuffer.setSize(imgSize);


    rawImageData.setimgData(imgBuffer);
    rawImageData.setheight(frame.rows);
    rawImageData.setwidth(frame.cols);
    rawImageData.setpixelFormat(frame.type());

    switch (cameraAction.e){
    case CameraAction::PROCESS:
      this->log_ACTIVITY_LO_CameraProcess();
      this->process_out(0, rawImageData);
      this->tlmWrite_photosTaken(m_photoCount++);
      break;
    case CameraAction::SAVE:
      this->save_out(0, imgBuffer);
      this->log_ACTIVITY_LO_CameraSave();
      this->tlmWrite_photosTaken(m_photoCount++);
      break;
    default:
      m_validCommand = false;
      this->log_WARNING_HI_InvalidTakeCmd();
      FW_ASSERT(0);
    }

  }
  this->tlmWrite_commandNum(m_cmdCount++);
  this->cmdResponse_out(opCode, cmdSeq,
                        m_validCommand ? Fw::CmdResponse::OK
                                       : Fw::CmdResponse::EXECUTION_ERROR);
}

void Camera ::ExposureTime_cmdHandler(const FwOpcodeType opCode,
                                      const U32 cmdSeq, U32 time) {
  if (time <= MAX_EXPOSURE_TIME) {
    // Set camera exposure to manual mode
    m_capture.set(cv::CAP_PROP_AUTO_EXPOSURE, 0.25);
    m_capture.set(cv::CAP_PROP_EXPOSURE, time);
    this->log_ACTIVITY_HI_ExposureTimeSet(time); // Make activity
    this->tlmWrite_commandNum(m_cmdCount++);
  } else {
    m_validCommand = false;
    this->log_WARNING_HI_InvalidExposureTimeCmd(time);
  }
  this->cmdResponse_out(opCode, cmdSeq,
                        m_validCommand ? Fw::CmdResponse::OK
                                       : Fw::CmdResponse::EXECUTION_ERROR);
}

void Camera ::ConfigImg_cmdHandler(const FwOpcodeType opCode, const U32 cmdSeq,
                                   Payload::ImgResolution resolution,
                                   Payload::ColorFormat format) {
  switch (format.e){
  case ColorFormat::YUYV:
    m_capture.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('Y', 'U', 'Y', 'V'));
    break;
  case ColorFormat::RGB:
    m_capture.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('R', 'G', 'B', '3'));
    break;
  default:
    m_validCommand = false;
    this->log_WARNING_HI_InvalidFormatCmd(format);
    FW_ASSERT(0);
  }

  switch (resolution.e){
  case ImgResolution::SIZE_640x480:
    m_capture.set(cv::CAP_PROP_FRAME_WIDTH, 640);
    m_capture.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
    break;
  case ImgResolution::SIZE_800x600:
    m_capture.set(cv::CAP_PROP_FRAME_WIDTH, 800);
    m_capture.set(cv::CAP_PROP_FRAME_HEIGHT, 600);
    break;
  default:
    this->log_WARNING_HI_InvalidSizeCmd(resolution);
    m_validCommand = false;
    FW_ASSERT(0);
  }

  if (m_validCommand) {
    this->log_ACTIVITY_HI_SetImgConfig(resolution, format);
  }
  this->tlmWrite_commandNum(m_cmdCount++);
  this->cmdResponse_out(opCode, cmdSeq,
                        m_validCommand ? Fw::CmdResponse::OK
                                       : Fw::CmdResponse::EXECUTION_ERROR);
}

} // end namespace Payload
