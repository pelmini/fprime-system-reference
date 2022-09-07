// ======================================================================
// \title  Gps.hpp
// \author lestarch, djwait, vbai
// \brief  hpp file for Gps component implementation class
// ======================================================================

#ifndef Gps_HPP
#define Gps_HPP

#include "SystemReference/Gnc/Gps/GpsComponentAc.hpp"
#include "Utils/Types/CircularBuffer.hpp"

#define UART_READ_BUFF_SIZE 1024

namespace Gnc {

class Gps : public GpsComponentBase {

public:
  // ----------------------------------------------------------------------
  // Types
  // ----------------------------------------------------------------------

  /*** @brief GpsPacket:
   * * A structured containing the information in the GPS location packet
   * * received via a NEMA GPS receiver
   * * units are as-read from receiver, before parsing*/
  struct GpsPacket {
    float utcTime;     // UTC of position hhmmss.ss
    float dmNS;         // Latitude (DDmm.mm)
    char northSouth;    // Latitude direction: (N = North, S = South)
    float dmEW;         // Longitude (DDDmm.mm)
    char eastWest;      // Longitude direction: (E = East, W = West)
    float altitude;     // Antenna altitude above/below mean sea level
    unsigned int lock;  // GPS quality; 0 = Fix not available or invalid
    unsigned int count; // Number of satellites in use
  };

  // ----------------------------------------------------------------------
  // Construction, initialization, and destruction
  // ----------------------------------------------------------------------

  //! Construct object Gps
  //!
  Gps(const char *const compName /*!< The component name*/
  );

  //! Initialize object Gps
  //!
  void init(const NATIVE_INT_TYPE queueDepth,  /*!< The queue depth*/
            const NATIVE_INT_TYPE instance = 0 /*!< The instance number*/
  );

  //! Destroy object Gps
  //!
  ~Gps();
  PRIVATE:

      // ----------------------------------------------------------------------
      // Handler implementations for user-defined typed input ports
      // ----------------------------------------------------------------------

      //! Handler implementation for getAltitude
      //!
      Gnc::GpsData getAltitude_handler(
          const NATIVE_INT_TYPE portNum /*!< The port number*/
      );

  //! Handler implementation for getCoordinates
  //!
  Gnc::GpsData getCoordinates_handler(
      const NATIVE_INT_TYPE portNum /*!< The port number*/
  );

  //! Handler implementation for getSatelliteCount
  //!
  Gnc::GpsData getSatelliteCount_handler(
      const NATIVE_INT_TYPE portNum /*!< The port number*/
  );

  //! Handler implementation for gpsIn
  //!
  void gpsIn_handler(
      const NATIVE_INT_TYPE portNum, /*!< The port number*/
      Fw::Buffer &recvBuffer,
      const Drv::RecvStatus &recvStatus
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

    // ----------------------------------------------------------------------
    // Helper Functions
    // ----------------------------------------------------------------------
    void setup();

    void processBuffer(Fw::Buffer& buffer);

    void processRing();

    void getGpsData(GpsPacket packet);

    // ----------------------------------------------------------------------
    // Member Variables
    // ----------------------------------------------------------------------
    //!< Lock status
    bool m_locked;

    U32 m_uartReadBytes;
    Fw::TlmString m_lastUartMsg;


    //! Buffer that holds UART messages
    U8 m_ringUartBuffer[UART_READ_BUFF_SIZE];

    //! The circular buffer
    Types::CircularBuffer m_inRing;

    Gnc::GpsData m_gpsData;
    Gnc::GpsData m_altitude;
    Gnc::GpsData m_satelliteCount;
};

} // end namespace Gnc

#endif