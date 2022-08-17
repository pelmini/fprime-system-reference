// ======================================================================
// \title  PhotoConverter.cpp
// \author vbai
// \brief  cpp file for PhotoConverter component implementation class
// ======================================================================


#include <SystemReference/Payload/PhotoConverter/PhotoConverter.hpp>
#include "Fw/Types/BasicTypes.hpp"

namespace Payload {

  // ----------------------------------------------------------------------
  // Construction, initialization, and destruction
  // ----------------------------------------------------------------------

  PhotoConverter ::
    PhotoConverter(
        const char *const compName
    ) : PhotoConverterComponentBase(compName)
  {

  }

  void PhotoConverter ::
    init(
        const NATIVE_INT_TYPE queueDepth,
        const NATIVE_INT_TYPE instance
    )
  {
    PhotoConverterComponentBase::init(queueDepth, instance);
  }

  PhotoConverter ::
    ~PhotoConverter()
  {

  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------

  void PhotoConverter ::
    preProcess_handler(
        const NATIVE_INT_TYPE portNum,
        Fw::Buffer &fwBuffer
    )
  {
    this->postProcess_out(0, fwBuffer);
  }

  // ----------------------------------------------------------------------
  // Command handler implementations
  // ----------------------------------------------------------------------

  void PhotoConverter ::
    SetFormat_cmdHandler(
        const FwOpcodeType opCode,
        const U32 cmdSeq,
        Payload::FileFormat fileFormat
    )
  {
    bool validFormat = true;

    if(fileFormat == FileFormat::JPG) {

    } else if (fileFormat == FileFormat::PNG){

    } else {
      validFormat = false;
    }

    this->log_ACTIVITY_HI_SetFileFormat(fileFormat);
    this->cmdResponse_out(opCode, cmdSeq,
                          validFormat ? Fw::CmdResponse::OK
                                         : Fw::CmdResponse::EXECUTION_ERROR);
  }

} // end namespace Payload
