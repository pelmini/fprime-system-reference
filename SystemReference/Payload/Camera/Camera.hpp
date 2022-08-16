// ======================================================================
// \title  Camera.hpp
// \author vbai
// \brief  hpp file for Camera component implementation class
// ======================================================================

#ifndef Camera_HPP
#define Camera_HPP

#include "SystemReference/Payload/Camera/CameraComponentAc.hpp"
#include "Fw/Types/MemAllocator.hpp"
extern "C" {
#include "SystemReference/Payload/Camera/Capture.h"
};

namespace Payload {

  class Camera :
    public CameraComponentBase
  {

    public:

      // ----------------------------------------------------------------------
      // Construction, initialization, and destruction
      // ----------------------------------------------------------------------

      //! Construct object Camera
      //!
      Camera(
          const char *const compName /*!< The component name*/
      );

      //! Initialize object Camera
      //!
      void init(
          const NATIVE_INT_TYPE queueDepth, /*!< The queue depth*/
          const NATIVE_INT_TYPE instance = 0 /*!< The instance number*/
      );

      void open(const char *dev_name);
      //! Destroy object Camera
      //!
      ~Camera();


    PRIVATE:

      // ----------------------------------------------------------------------
      // Command handler implementations
      // ----------------------------------------------------------------------

       //! Implementation for SetAction command handler
       //! Set camera action
       void SetAction_cmdHandler(
          const FwOpcodeType opCode, /*!< The opcode*/
          const U32 cmdSeq,          /*!< The command sequence number*/
          Payload::CameraAction cameraAction /*!< State where camera either
                                              * saves or takes photo */
       );


      //! Implementation for ExposureTime command handler
      //! Command to set the exposure time
      void ExposureTime_cmdHandler(
          const FwOpcodeType opCode, /*!< The opcode*/
          const U32 cmdSeq, /*!< The command sequence number*/
          uint32_t time /*!< The exposure time*/
      );

      //! Implementation for ConfigImg command handler
      //! Command to configure image
      void ConfigImg_cmdHandler(
          const FwOpcodeType opCode, /*!< The opcode*/
          const U32 cmdSeq, /*!< The command sequence number*/
          ImgResolution resolution, /*!< The image size*/
          ImgFormat format /*!< The image format*/
      );

      // ----------------------------------------------------------------------
      // Helper Functions
      // ----------------------------------------------------------------------

      Fw::Buffer readImage();

      U32 m_cmdCount;
      U32 m_photoCount;
      NATIVE_INT_TYPE m_imgSize;
      bool m_validCommand;
      NATIVE_INT_TYPE m_fileDescriptor;

    };

} // end namespace Payload

#endif
