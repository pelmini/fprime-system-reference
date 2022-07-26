// ======================================================================
// \title  Camera.cpp
// \author vbai
// \brief  cpp file for Camera component implementation class
// ======================================================================


#include <SystemReference/Payload/Camera/Camera.hpp>
#include "Fw/Types/BasicTypes.hpp"

namespace Payload {

  // ----------------------------------------------------------------------
  // Construction, initialization, and destruction
  // ----------------------------------------------------------------------

  Camera ::
    Camera(
        const char *const compName
    ) : CameraComponentBase(compName)
  {

  }

  void Camera ::
    init(
        const NATIVE_INT_TYPE queueDepth,
        const NATIVE_INT_TYPE instance
    )
  {
    CameraComponentBase::init(queueDepth, instance);
  }

  Camera ::
    ~Camera()
  {

  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------

  void Camera ::
    Run_handler(
        const NATIVE_INT_TYPE portNum,
        NATIVE_UINT_TYPE context
    )
  {
    // TODO
  }

  // ----------------------------------------------------------------------
  // Command handler implementations
  // ----------------------------------------------------------------------

  void Camera ::
    Save_cmdHandler(
        const FwOpcodeType opCode,
        const U32 cmdSeq
    )
  {
    // TODO
    this->cmdResponse_out(opCode,cmdSeq,Fw::CmdResponse::OK);
  }

  void Camera ::
    Take_cmdHandler(
        const FwOpcodeType opCode,
        const U32 cmdSeq
    )
  {
    // TODO
    this->cmdResponse_out(opCode,cmdSeq,Fw::CmdResponse::OK);
  }

} // end namespace Payload
