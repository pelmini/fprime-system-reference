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
// rpi converts milliseconds to seconds
const int MAX_EXPOSURE_TIME_MS = 100000;

Camera ::Camera(const char *const compName)
    : CameraComponentBase(compName), m_photoCount(0){}

void Camera ::init(const NATIVE_INT_TYPE queueDepth,
                   const NATIVE_INT_TYPE instance) {
  CameraComponentBase::init(queueDepth, instance);
}

bool Camera::open(I32 deviceIndex) {

  if (m_capture.isOpened()) {
    this->log_ACTIVITY_LO_CameraAlreadyOpen();
    return true;
  }
  m_capture.open(deviceIndex);
  if (!m_capture.isOpened()) {
    this->log_WARNING_HI_CameraOpenError();
    return false;
  }
  return true;
}

Camera ::~Camera() {}

// ----------------------------------------------------------------------
// Command handler implementations
// ----------------------------------------------------------------------

void Camera ::TakeAction_cmdHandler(const FwOpcodeType opCode, const U32 cmdSeq,
                                    Payload::CameraAction cameraAction) {
  RawImageData rawImageData;
  std::vector<uchar> buffer;
  Fw::Buffer imgBuffer;
  cv::Mat frame;

  m_capture.read(frame);

  if (frame.empty()) {
    this->log_WARNING_HI_BlankFrame();
    return;
  }

  // Set up buffer for image data
  U32 imgSize = frame.rows * frame.cols * frame.elemSize();
  imgBuffer = allocate_out(0, imgSize);
  if (imgBuffer.getSize() < imgSize){
    this->log_WARNING_HI_InvalidBufferSizeError(imgBuffer.getSize(), imgSize);
    return;
  }
  FW_ASSERT(imgSize < frame.total()*frame.elemSize(), imgSize, frame.total()*frame.elemSize());

  if(!frame.isContinuous()){
    this->log_WARNING_HI_NotContinuous();
    return;
  }

  memcpy(imgBuffer.getData(), frame.data, imgSize);
  imgBuffer.setSize(imgSize);

  switch (cameraAction.e) {
    case CameraAction::PROCESS:
      rawImageData.setimgData(imgBuffer);
      rawImageData.setheight(frame.rows);
      rawImageData.setwidth(frame.cols);
      rawImageData.setpixelFormat(frame.type());
      this->log_ACTIVITY_LO_CameraProcess();
      this->process_out(0, rawImageData);
      break;
    case CameraAction::SAVE:
      this->save_out(0, imgBuffer);
      this->log_ACTIVITY_LO_CameraSave();
      break;
    default:
      this->log_WARNING_HI_InvalidTakeCmd();
      FW_ASSERT(0);
    }
  m_photoCount++;
  this->tlmWrite_photosTaken(m_photoCount);
  this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}

void Camera ::ExposureTime_cmdHandler(const FwOpcodeType opCode,
                                      const U32 cmdSeq, U32 time) {
  bool setExposureTime = false;
  bool setExposureMode = false;
  if (time <= MAX_EXPOSURE_TIME_MS) {
    // Set camera exposure to manual mode
    setExposureMode = m_capture.set(cv::CAP_PROP_AUTO_EXPOSURE, 0.25);
    setExposureTime = m_capture.set(cv::CAP_PROP_EXPOSURE, time);

    if(setExposureMode && setExposureTime){
      this->log_ACTIVITY_HI_ExposureTimeSet(time);
    } else {
      this->log_WARNING_HI_ExposureTimeFail(time);
    }
  } else {
    this->log_WARNING_HI_InvalidExposureTimeCmd(time);
  }
  this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}

void Camera ::ConfigImg_cmdHandler(const FwOpcodeType opCode, const U32 cmdSeq,
                                   Payload::ImgResolution resolution,
                                   Payload::ColorFormat format) {
  bool colorStatus = false;
  bool widthStatus = false;
  bool heightStatus = false;

  switch (format.e) {
    case ColorFormat::YUYV:
      colorStatus = m_capture.set(cv::CAP_PROP_FOURCC,
                  cv::VideoWriter::fourcc('Y', 'U', 'Y', 'V'));
      break;
    case ColorFormat::RGB:
      colorStatus = m_capture.set(cv::CAP_PROP_FOURCC,
                  cv::VideoWriter::fourcc('R', 'G', 'B', '3'));
      break;
    default:
      this->log_WARNING_HI_InvalidFormatCmd(format);
      FW_ASSERT(0);
  }

  switch (resolution.e) {
    case ImgResolution::SIZE_640x480:
      widthStatus = m_capture.set(cv::CAP_PROP_FRAME_WIDTH, 640);
      heightStatus = m_capture.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
      break;
    case ImgResolution::SIZE_800x600:
      widthStatus = m_capture.set(cv::CAP_PROP_FRAME_WIDTH, 800);
      heightStatus = m_capture.set(cv::CAP_PROP_FRAME_HEIGHT, 600);
      break;
    default:
      this->log_WARNING_HI_InvalidSizeCmd(resolution);
      FW_ASSERT(0);
  }

  if(colorStatus && heightStatus && widthStatus){
    this->log_ACTIVITY_HI_SetImgConfig(resolution, format);
  } else {
    this->log_WARNING_HI_ImgConfigSetFail(resolution, format);
  }

  this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}

} // end namespace Payload
