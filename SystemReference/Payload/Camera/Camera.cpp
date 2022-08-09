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
    m_imgSize(0), m_fileDescriptor(-1){}

void Camera ::init(const NATIVE_INT_TYPE queueDepth,
                   const NATIVE_INT_TYPE instance) {
  CameraComponentBase::init(queueDepth, instance);
}

void Camera ::open(const char *dev_name, const FwOpcodeType opCode, const U32 cmdSeq) {
  if ((init_device(dev_name, m_fileDescriptor)) ||
          (open_device(dev_name, m_fileDescriptor)) != 0) {
    this->log_WARNING_HI_CameraError(dev_name);
  }
}
Camera ::~Camera() {
  close_device(m_fileDescriptor);
}

// ----------------------------------------------------------------------
// Command handler implementations
// ----------------------------------------------------------------------

void Camera ::Save_cmdHandler(const FwOpcodeType opCode, const U32 cmdSeq) {
  Fw::Buffer imgBuffer = readImage();
  this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
  this->tlmWrite_commandNum(m_cmdCount++);
}

void Camera ::Take_cmdHandler(const FwOpcodeType opCode, const U32 cmdSeq) {
  Fw::Buffer imgBuffer = readImage();
  this->sendPhoto_out(0, imgBuffer);
  this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
  this->tlmWrite_photosTaken(m_photoCount++);
  this->tlmWrite_commandNum(m_cmdCount++);
}
void Camera ::ExposureTime_cmdHandler(const FwOpcodeType opCode,
                                      const U32 cmdSeq, uint32_t time) {
//  time = 100;
  if (time <= MAX_EXPOSURE_TIME){
    set_exposure_time(time, m_fileDescriptor);
    this->log_ACTIVITY_HI_ExposureTimeSet(time); //Make activity
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
    this->tlmWrite_commandNum(m_cmdCount++);
  } else {
    this->log_WARNING_HI_InvalidExposureTime(time);
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::EXECUTION_ERROR);
  }
}

void Camera ::ConfigImg_cmdHandler(const FwOpcodeType opCode, const U32 cmdSeq,
                                   ImgResolution resolution, ImgFormat format) {
  uint32_t V4L2Format = 0;
  uint32_t width = 0;
  uint32_t height = 0;
  bool validCommand = true;


  if (format == ImgFormat::YUYV){
    V4L2Format = V4L2_PIX_FMT_YUYV;
  } else if (format == ImgFormat::RGB){
    V4L2Format = V4L2_PIX_FMT_RGB24;
  } else {
    this->log_WARNING_HI_InvalidFormat(format);
    validCommand = false;
  }

  if(resolution == ImgResolution::SIZE_640x480){
    width = 640;
    height = 480;
  } else if (resolution == ImgResolution::SIZE_800x600){
    width = 800;
    height = 600;
  } else {
    this->log_WARNING_HI_InvalidSize(resolution);
    validCommand = false;
  }

  if(validCommand){
    m_imgSize = set_format(height, width, V4L2Format, m_fileDescriptor);
    this->log_ACTIVITY_HI_SetImgConfig(resolution, format);
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
  } else{
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::EXECUTION_ERROR);
  }
  this->tlmWrite_commandNum(m_cmdCount++);

}

// ----------------------------------------------------------------------
// Helper Functions
// ----------------------------------------------------------------------

Fw::Buffer Camera::readImage() {
  size_t readSize = 0;
  Fw::Buffer imageBuffer = this->allocate_out(0, m_imgSize);
  read_frame(imageBuffer.getData(), m_imgSize, &readSize, m_fileDescriptor);
  return imageBuffer;
}

} // end namespace Payload
