// ======================================================================
// \title  Gps.cpp
// \author vbai, lestarch
// \brief  cpp file for Gps component implementation class
// ======================================================================

#include "Fw/Logger/Logger.hpp"
#include "Fw/Types/BasicTypes.hpp"
#include <SystemReference/Gnc/Gps/Gps.hpp>

#include <cstring>

namespace Gnc {

// ----------------------------------------------------------------------
// Construction, initialization, and destruction
// ----------------------------------------------------------------------

Gps ::Gps(const char *const compName)
    : GpsComponentBase(compName),
      // Initialize the lock to "false"
      m_locked(false) {}

void Gps ::init(const NATIVE_INT_TYPE queueDepth,
                const NATIVE_INT_TYPE instance) {
  GpsComponentBase::init(queueDepth, instance);
}

// The linux serial driver keeps its storage externally. This means that
// we need to supply it some buffers to work with.
void Gps::setup(void) {
  for (NATIVE_INT_TYPE buffer = 0; buffer < NUM_UART_BUFFERS; buffer++) {
    // Assign the raw data to the buffer. Make sure to include the side of the
    // region assigned.
    this->m_recvBuffers[buffer].setData((U8 *)this->m_uartBuffers[buffer]);
    this->m_recvBuffers[buffer].setSize(UART_READ_BUFF_SIZE);
    this->serialBufferOut_out(0, this->m_recvBuffers[buffer]);
  }
}

Gps ::~Gps() {}

// ----------------------------------------------------------------------
// Handler implementations for user-defined typed input ports
// ----------------------------------------------------------------------

void Gps ::serialReceive_handler(const NATIVE_INT_TYPE portNum,
                                 Fw::Buffer &serBuffer,
                                 Drv::SerialReadStatus &serialStatus) {
  int gpsPacketStatus = 0;
  float lat = 0.0f, lon = 0.0f;
  GpsPacket packet;

  U32 buffsize = static_cast<U32>(serBuffer.getSize());
  char *pointer = reinterpret_cast<char *>(serBuffer.getData());

  if (serialStatus != Drv::SerialReadStatus::SER_OK) {
    Fw::Logger::logMsg("[WARNING] Received buffer with bad packet: %d\n",
                       serialStatus.e);
    // Reset buffer for reuse
    serBuffer.setSize(UART_READ_BUFF_SIZE);
    this->serialBufferOut_out(0, serBuffer);
    return;
  }
  // If not enough data is available for a full message, return the buffer and
  // abort.
  else if (buffsize < 24) {
    serBuffer.setSize(UART_READ_BUFF_SIZE);
    this->serialBufferOut_out(0, serBuffer);
    return;
  }

  //  Parse the GPS message from the UART (looking for $GPGGA messages).
  //  This uses standard C functions to read all the defined protocol messages
  //  into GPS package struct. If all 9 items are parsed, we break. Otherwise,
  //  we continue to scan the block of data looking for messages further in.
  for (U32 i = 0; i < (buffsize - 24); i++) {
    gpsPacketStatus =
        sscanf(pointer, "$GPGGA,%f,%f,%c,%f,%c,%u,%u,%f,%f", &packet.utcTime,
               &packet.dmNS, &packet.northSouth, &packet.dmEW, &packet.eastWest,
               &packet.lock, &packet.count, &packet.filler, &packet.altitude);
    // Break when all GPS items are found
    if (gpsPacketStatus == 9) {
      break;
    }
    pointer = pointer + 1;
  }

  // If we failed to find the GPGGA then return the buffer and abort.
  if (gpsPacketStatus == 0) {
    serBuffer.setSize(UART_READ_BUFF_SIZE);
    this->serialBufferOut_out(0, serBuffer);
    return;
  } else if (gpsPacketStatus != 9) {
    Fw::Logger::logMsg("[ERROR] GPS parsing failed: %d\n", gpsPacketStatus);
    serBuffer.setSize(UART_READ_BUFF_SIZE);
    this->serialBufferOut_out(0, serBuffer);
    return;
  }

  // GPS packet locations are of the form: ddmm.mmmm
  // We will convert to lat/lon in degrees only before downlinking
  // Latitude degrees, add on minutes (converted to degrees), multiply by
  // direction
  lat = (U32)(packet.dmNS / 100.0f);
  lat = lat + (packet.dmNS - (lat * 100.0f)) / 60.0f;
  lat = lat * ((packet.northSouth == 'N') ? 1 : -1);
  // Longitude degrees, add on minutes (converted to degrees), multiply by
  // direction
  lon = (U32)(packet.dmEW / 100.0f);
  lon = lon + (packet.dmEW - (lon * 100.0f)) / 60.f;
  lon = lon * ((packet.eastWest == 'E') ? 1 : -1);

  this->tlmWrite_latitude(lat);
  this->tlmWrite_longitude(lon);
  this->tlmWrite_altitude(packet.altitude);
  this->tlmWrite_satelliteCount(packet.count);

  if (packet.lock == 0 && m_locked) {
    m_locked = false;
    this->log_WARNING_HI_GpsLockLost();
  } else if (packet.lock == 1 && !m_locked) {
    m_locked = true;
    this->log_ACTIVITY_HI_GpsLockAcquired();
  }

  serBuffer.setSize(UART_READ_BUFF_SIZE);
  this->serialBufferOut_out(0, serBuffer);
}

// ----------------------------------------------------------------------
// Command handler implementations
// ----------------------------------------------------------------------

void Gps ::GpsReportLockStatus_cmdHandler(const FwOpcodeType opCode,
                                          const U32 cmdSeq) {
  // Locked-force print
  if (m_locked) {
    this->log_ACTIVITY_HI_GpsLockAcquired();
  } else {
    this->log_WARNING_HI_GpsLockLost();
  }
  this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}
} // end namespace Gnc
