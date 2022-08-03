// ======================================================================
// \title  Camera.cpp
// \author vbai
// \brief  cpp file for Camera component implementation class
// ======================================================================

#include "Fw/Types/BasicTypes.hpp"
#include <SystemReference/Payload/Camera/Camera.hpp>
#include <SystemReference/Payload/Camera/Capture.h>

namespace Payload {

// ----------------------------------------------------------------------
// Construction, initialization, and destruction
// ----------------------------------------------------------------------

Camera ::Camera(const char *const compName)
    : CameraComponentBase(compName), m_cmdCount(0), m_photoCount(0),
    m_imgSize(0), m_exposureTime(100000){}

void Camera ::init(const NATIVE_INT_TYPE queueDepth,
                   const NATIVE_INT_TYPE instance) {
  CameraComponentBase::init(queueDepth, instance);
}

void Camera ::open(const char *dev_name) {
  dev_name = "/dev/video0";
  init_device();
  open_device();
}
Camera ::~Camera() {
  uninit_device();
  close_device();
}
// ----------------------------------------------------------------------
// Handler implementations for user-defined typed input ports
// ----------------------------------------------------------------------

void Camera ::Run_handler(const NATIVE_INT_TYPE portNum,
                          NATIVE_UINT_TYPE context) {}

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
  time = m_exposureTime;
  set_exposure_time(time);
  this->log_WARNING_HI_ExposureTimeSet(time);
  this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}

void Camera ::ConfigImg_cmdHandler(const FwOpcodeType opCode, const U32 cmdSeq,
                                   ImgResolution resolution, ImgFormat format) {
  uint32_t V4L2Format;
  uint32_t width;
  uint32_t height;

  if (m_imgFormat == format.YUYV){
    V4L2Format = V4L2_PIX_FMT_YUYV;
  } else if (m_imgFormat == format.RGB){
    V4L2Format = V4L2_PIX_FMT_RGB24;
  }

  if(m_imgResolution == resolution.SIZE_640x480 ){
    width = 640;
    height = 480;
  } else if (m_imgResolution == resolution.SIZE_800x600){
    width = 800;
    height = 600;
  }

  m_imgSize = set_format(height, width, V4L2Format);
  this->log_WARNING_HI_SetImgConfig(m_imgResolution, format);
  this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}

// ----------------------------------------------------------------------
// Helper Functions
// ----------------------------------------------------------------------

Fw::Buffer Camera::readImage() {
  size_t readSize = 0;
  Fw::Buffer imageBuffer = this->allocate_out(0, m_imgSize);
  read_frame(imageBuffer.getData(), m_imgSize, &readSize);
  return imageBuffer;
}

} // end namespace Payload
