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
      m_imgSize(0) {}

void Camera ::init(const NATIVE_INT_TYPE queueDepth,
                   const NATIVE_INT_TYPE instance) {
  CameraComponentBase::init(queueDepth, instance);
}

void Camera ::open(const char *dev_name) {
  dev_name = "/dev/video0";
  m_imgSize = init_device();
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
                                      const U32 cmdSeq, U32 time) {
  // TODO
  this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}

void Camera ::ConfigImg_cmdHandler(const FwOpcodeType opCode, const U32 cmdSeq,
                                   ImgSize size, ImgFormat format) {
  // TODO
  this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}

Fw::Buffer Camera::readImage() {
  int readSize = 0;
  Fw::Buffer imageBuffer = this->allocate_out(0, m_imgSize);
  read_frame(imageBuffer.getData(), m_imgSize, &readSize);
  return imageBuffer;
}

} // end namespace Payload
