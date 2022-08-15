// ======================================================================
// \title  Gps.hpp
// \author vbai, lestarch
// \brief  hpp file for Gps component implementation class
// ======================================================================

#ifndef Gps_HPP
#define Gps_HPP

#include "SystemReference/Gnc/Gps/GpsComponentAc.hpp"


namespace Gnc {

  class Gps :
    public GpsComponentBase
  {
    /**
       * GpsPacket:
       *   A structure containing the information in the GPS location packet
       * received via the NMEA GPS receiver.
     */
    struct GpsPacket {
      float utcTime;
      float dmNS;
      char northSouth;
      float dmEW;
      char eastWest;
      unsigned int lock;
      unsigned int count;
      float filler;
      float altitude;
    };

    public:

      // ----------------------------------------------------------------------
      // Construction, initialization, and destruction
      // ----------------------------------------------------------------------

      //! Construct object Gps
      //!
      Gps(
          const char *const compName /*!< The component name*/
      );

      //! Initialize object Gps
      //!
      void init(
          const NATIVE_INT_TYPE queueDepth, /*!< The queue depth*/
          const NATIVE_INT_TYPE instance = 0 /*!< The instance number*/
      );

      //! Setup buffers to be used be the linux serial driver
      //!
//      void setup(void);

      //! Destroy object Gps
      //!
      ~Gps();

    PRIVATE:

      // ----------------------------------------------------------------------
      // Handler implementations for user-defined typed input ports
      // ----------------------------------------------------------------------

      //! Handler implementation for serialReceive
      //!
      void serialReceive_handler(
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          Fw::Buffer &serBuffer, /*!<Buffer containing data*/
          Drv::SerialReadStatus &serialStatus /*!<Status of read*/
      );

    PRIVATE:

      // ----------------------------------------------------------------------
      // Command handler implementations
      // ----------------------------------------------------------------------

      //! Implementation for GpsReportLockStatus command handler
      //! A command to force an EVR reporting lock status
      void GpsReportLockStatus_cmdHandler(
          const FwOpcodeType opCode, /*!< The opcode*/
          const U32 cmdSeq /*!< The command sequence number*/
      );

    //!< Flag to indicate if device has acquired GPS lock
    bool m_locked;

    };

} // end namespace Gnc

#endif
