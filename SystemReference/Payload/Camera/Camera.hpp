// ======================================================================
// \title  Camera.hpp
// \author vbai
// \brief  hpp file for Camera component implementation class
// ======================================================================

#ifndef Camera_HPP
#define Camera_HPP

#include "SystemReference/Payload/Camera/CameraComponentAc.hpp"
#include <opencv2/opencv.hpp>


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

      //! Startup camera device
      //!
      bool open();


      //! Destroy object Camera
      //!
      ~Camera();

    PRIVATE:

      // ----------------------------------------------------------------------
      // Command handler implementations
      // ----------------------------------------------------------------------

      //! Implementation for TakeAction command handler
      //! Set the action that camera should take
      void TakeAction_cmdHandler(
          const FwOpcodeType opCode, /*!< The opcode*/
          const U32 cmdSeq, /*!< The command sequence number*/
          Payload::CameraAction cameraAction /*!< 
          State where camera either saves or takes photo
          */
      );

      //! Implementation for ExposureTime command handler
      //! Set the exposure time
      void ExposureTime_cmdHandler(
          const FwOpcodeType opCode, /*!< The opcode*/
          const U32 cmdSeq, /*!< The command sequence number*/
          U32 time 
      );

      //! Implementation for ConfigImg command handler
      //! Command to configure image
      void ConfigImg_cmdHandler(
          const FwOpcodeType opCode, /*!< The opcode*/
          const U32 cmdSeq, /*!< The command sequence number*/
          Payload::ImgResolution resolution, 
          Payload::ColorFormat format 
      );

      U32 m_cmdCount;
      U32 m_photoCount;
      bool m_validCommand;
      cv::VideoCapture m_capture;
      cv::Mat m_imgFrame;
      U32 m_width;
      U32 m_height;
    };

} // end namespace Payload

#endif
