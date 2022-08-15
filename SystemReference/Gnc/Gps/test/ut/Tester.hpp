// ======================================================================
// \title  Gps/test/ut/Tester.hpp
// \author vbai
// \brief  hpp file for Gps test harness implementation class
// ======================================================================

#ifndef TESTER_HPP
#define TESTER_HPP

#include "GTestBase.hpp"
#include "SystemReference/Gnc/Gps/Gps.hpp"
#include "Fw/Types/BasicTypes.hpp"


namespace Gnc {

  class Tester :
    public GpsGTestBase
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

      //! To do
      //!
      void testGetGpsData();

      void testGetGpsData2();

      void testLockLost();

      void testLockStatusCmdLost();

      void testLockStatusCmdAcquired();

      void testBadGPSPacket();

      void testParseFail();

//      void testBadBuffSize();

    private:

      // ----------------------------------------------------------------------
      // Handlers for typed from ports
      // ----------------------------------------------------------------------

      //! Handler for from_serialBufferOut
      //!
      void from_serialBufferOut_handler(
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

    private:

      // ----------------------------------------------------------------------
      // Variables
      // ----------------------------------------------------------------------

      //! The component under test
      //!
      Gps component;

  };

} // end namespace Gnc

#endif
