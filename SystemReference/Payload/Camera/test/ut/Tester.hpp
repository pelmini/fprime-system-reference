// ======================================================================
// \title  Camera/test/ut/Tester.hpp
// \author pel
// \brief  hpp file for Camera test harness implementation class
// ======================================================================

#ifndef TESTER_HPP
#define TESTER_HPP

#include "GTestBase.hpp"
#include "SystemReference/Payload/Camera/Camera.hpp"
#include "STest/Random/Random.hpp"
#include "STest/Pick/Pick.hpp"
#include <Fw/Types/BasicTypes.hpp>

extern "C"{
#include "SystemReference/Payload/Camera/test/ut/CaptureMock.h"
};

namespace Payload {

  class Tester :
    public CameraGTestBase
  {

      // ----------------------------------------------------------------------
      // Construction and destruction
      // ----------------------------------------------------------------------

    public:

      //! Construct object Tester
      //!
      Tester();

      //! Destroy object Tester
      //!
      ~Tester();

    public:

      // ----------------------------------------------------------------------
      // Tests
      // ----------------------------------------------------------------------

      //! Test to check if image is properly configured
      //!
      void testImgConfiguration1();

      void testImgConfiguration2();

      void testImgConfiguration3();

      void testImgConfiguration4();

      //! Test to check if exposure time is properly set
      //!
      void testExposureTime();

      //! Test to check camera save command
      //!
      void testCameraActionSave();

      //! Check camera process command
      //!
      void testCameraActionProcess();

      //! Check setup
      //!
      void testSetup();

      //! Check setup error
      //!
      void testSetupError();

      //! Check invalid size error
      //!
      void testInvalidSize();

      //! Check invalid action command
      //!
      void testInvalidAction();

      //! Check invalid format error
      //!
      void testInvalidFormat();

      //! Check invalid time error
      //!
      void testInvalidTime();

      //! Check set format error
      //!
      void testSetFormatError();

      //! Check invalid frame error
      //!
      void testInvalidFrame();

      //! Check retry read
      //!
      void testRetryRead();

      //! Check partial image capture
      //!
      void testPartialImgCapture();

    private:

      // ----------------------------------------------------------------------
      // Handlers for typed from ports
      // ----------------------------------------------------------------------

      //! Handler for from_allocate
      //!
      Fw::Buffer from_allocate_handler(
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          U32 size 
      );

      //! Handler for from_process
      //!
      void from_process_handler(
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          Fw::Buffer &fwBuffer
      );

      //! Handler for from_save
      //!
      void from_save_handler(
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          Fw::Buffer &fwBuffer
      );

    private:

      // ----------------------------------------------------------------------
      // Helper methods
      // ----------------------------------------------------------------------

      //! Connect ports
      //!
      void connectPorts();

      //! Initialize components
      //!
      void initComponents();

      //! Pick an exposure time
      static NATIVE_UINT_TYPE pickExposureTime();

    private:

      // ----------------------------------------------------------------------
      // Variables
      // ----------------------------------------------------------------------

      //! The component under test
      //!
      Camera component;

  };

} // end namespace Payload

#endif
