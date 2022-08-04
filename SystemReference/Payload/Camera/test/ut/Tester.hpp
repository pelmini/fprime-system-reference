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
      void testImgConfiguration();

      //! Test to check if exposure time is properly set
      //!
      void testExposureTime();

      void testTakePhoto();

      void testSavePhoto();

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

      //! Handler for from_deallocate
      //!
      void from_deallocate_handler(
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          Fw::Buffer &fwBuffer 
      );

      //! Handler for from_sendPhoto
      //!
      void from_sendPhoto_handler(
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

      //! Pick an image size
      static ImgResolution pickImgResolution();

      //! Pick an image format
      static ImgFormat pickImgFormat();

      //! Pick an exposure time
      static NATIVE_UINT_TYPE pickExposureTime();

    private:

      // ----------------------------------------------------------------------
      // Variables
      // ----------------------------------------------------------------------

      //! The component under test
      //!
      Camera component;

      ImgResolution m_imgSize;
      ImgFormat m_imgFormat;
      U32 m_exposure_time;

  };

} // end namespace Payload

#endif
