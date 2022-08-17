// ======================================================================
// \title  Camera.cpp
// \author vbai
// \brief  cpp file for Camera component implementation class
// ======================================================================

#include "Fw/Types/BasicTypes.hpp"
#include <SystemReference/Payload/Camera/Camera.hpp>
#include <SystemReference/Payload/Camera/Capture.h>

const int MAX_EXPOSURE_TIME = 100000;

namespace Payload {

// ----------------------------------------------------------------------
// Construction, initialization, and destruction
// ----------------------------------------------------------------------

Camera ::Camera(const char *const compName)
    : CameraComponentBase(compName), m_cmdCount(0), m_photoCount(0),
      m_imgSize(0), m_fileDescriptor(-1), m_validCommand(true) {}

void Camera ::init(const NATIVE_INT_TYPE queueDepth,
                   const NATIVE_INT_TYPE instance) {
  CameraComponentBase::init(queueDepth, instance);
}

bool Camera ::open(const char *dev_name) {
  if ((init_device(dev_name, m_fileDescriptor)) ||
      (open_device(dev_name, m_fileDescriptor)) != 0) {
    this->log_WARNING_HI_CameraOpenError(dev_name);
    return false;
  }
  return true;
}
Camera ::~Camera() { close_device(m_fileDescriptor); }

// ----------------------------------------------------------------------
// Command handler implementations
// ----------------------------------------------------------------------

void Camera ::SetAction_cmdHandler(const FwOpcodeType opCode, const U32 cmdSeq,
                                   Payload::CameraAction cameraAction) {
  Fw::Buffer imgBuffer = readImage();
  if (cameraAction == CameraAction::PROCESS) {
    this->process_out(0, imgBuffer);
    this->log_ACTIVITY_LO_CameraProcess();
  } else if (cameraAction == CameraAction::SAVE) {
    this->save_out(0, imgBuffer);
    this->log_ACTIVITY_LO_CameraSave();
  } else {
    m_validCommand = false;
  }
  this->tlmWrite_photosTaken(m_photoCount++);
  this->tlmWrite_commandNum(m_cmdCount++);
  this->cmdResponse_out(opCode, cmdSeq,
                        m_validCommand ? Fw::CmdResponse::OK
                                       : Fw::CmdResponse::EXECUTION_ERROR);
}

void Camera ::ExposureTime_cmdHandler(const FwOpcodeType opCode,
                                      const U32 cmdSeq, uint32_t time) {
  if (time <= MAX_EXPOSURE_TIME) {
    set_exposure_time(time, m_fileDescriptor);
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
                                   ImgResolution resolution, ImgFormat format) {
  uint32_t V4L2Format = 0;
  uint32_t width = 0;
  uint32_t height = 0;

  if (format == ImgFormat::YUYV) {
    V4L2Format = V4L2_PIX_FMT_YUYV;
  } else if (format == ImgFormat::RGB) {
    V4L2Format = V4L2_PIX_FMT_RGB24;
  } else {
    this->log_WARNING_HI_InvalidFormatCmd(format);
    m_validCommand = false;
  }

  if (resolution == ImgResolution::SIZE_640x480) {
    width = 640;
    height = 480;
  } else if (resolution == ImgResolution::SIZE_800x600) {
    width = 800;
    height = 600;
  } else {
    this->log_WARNING_HI_InvalidSizeCmd(resolution);
    m_validCommand = false;
  }

  if (m_validCommand) {
    m_imgSize = set_format(height, width, V4L2Format, m_fileDescriptor);
    if (m_imgSize < 0) {
      this->log_WARNING_HI_SetFormatError(m_imgSize);
    } else {
      this->log_ACTIVITY_HI_SetImgConfig(resolution, format);
    }

    this->tlmWrite_commandNum(m_cmdCount++);
  }
  this->cmdResponse_out(opCode, cmdSeq,
                        m_validCommand ? Fw::CmdResponse::OK
                                       : Fw::CmdResponse::EXECUTION_ERROR);
}

// ----------------------------------------------------------------------
// Helper Functions
// ----------------------------------------------------------------------

Fw::Buffer Camera::readImage() {
  size_t readSize = 0;
  int readStatus = 0;
  Fw::Buffer imageBuffer = this->allocate_out(0, m_imgSize);

  readStatus =
      read_frame(imageBuffer.getData(), m_imgSize, &readSize, m_fileDescriptor);

  if (readStatus == -1) {
    this->log_WARNING_HI_invalidFrame(readSize);
    imageBuffer.setSize(0);
  } else if (readStatus == -2) {
    this->log_WARNING_LO_retryRead();
    imageBuffer.setSize(0);
  } else if (readSize != m_imgSize) {
    this->log_WARNING_LO_partialImgCapture(readSize);
  }

  return imageBuffer;
}

} // end namespace Payload
