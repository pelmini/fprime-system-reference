// ======================================================================
// \title  Gps.cpp
// \author vbai, lestarch
// \brief  cpp file for Gps component implementation class
// ======================================================================

#include "Fw/Logger/Logger.hpp"
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
  printf("HELLO !!!\n");

  if (serialStatus != Drv::SerialReadStatus::SER_OK) {
    Fw::Logger::logMsg("[WARNING] Received buffer with bad packet: %d\n",
                       serialStatus.e);
    this->log_WARNING_HI_BadGpsPacket();
    printf("HELLO 1 \n");
  } else if (buffsize < 24) {
    printf("HELLO 2 \n");
    this->log_WARNING_HI_BadBufferSize();
  }

  //  Parse the GPS message from the UART (looking for $GPGGA messages).
  //  This uses standard C functions to read all the defined protocol messages
  //  into GPS package struct. If all 9 items are parsed, we break. Otherwise,
  //  we continue to scan the block of data looking for messages further in.
//  printf("BRUH %d \n", 6-24);
  for (U32 i = 0; i < (buffsize - 24); i++) {
    gpsPacketStatus =
        sscanf(pointer, "$GPGGA,%f,%f,%c,%f,%c,%u,%u,%f,%f", &packet.utcTime,
               &packet.dmNS, &packet.northSouth, &packet.dmEW, &packet.eastWest,
               &packet.lock, &packet.count, &packet.filler, &packet.altitude);
    if (gpsPacketStatus == 9) {
      break;
    }
    pointer = pointer + 1;
//    printf("POINTER: %s \n", pointer);
//    printf("Index: %d \n", i);
  }

  // If we failed to find the GPGGA then return the buffer and abort.
  if (gpsPacketStatus == 0) {
    printf("HELLO 3 \n");
    this->log_WARNING_HI_InvalidGPGGA();
  } else if (gpsPacketStatus != 9) {
    Fw::Logger::logMsg("[ERROR] GPS parsing failed: %d\n", gpsPacketStatus);
    printf("HELLO 4 \n");
    this->log_WARNING_HI_ParseFail();
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

//  printf("THIS IS PACKET LOCK %d \n", packet.lock);
//  printf("THIS IS M_LOCKED %d \n", m_locked);
  if (packet.lock == 0 && m_locked) {
    m_locked = false;
    this->log_WARNING_HI_GpsLockLost();
  } else if (packet.lock == 1 && !m_locked) {
    m_locked = true;
    this->log_ACTIVITY_HI_GpsLockAcquired();
  }

  this->serialBufferOut_out(0, serBuffer);
}

// ----------------------------------------------------------------------
// Command handler implementations
// ----------------------------------------------------------------------

void Gps ::GpsReportLockStatus_cmdHandler(const FwOpcodeType opCode,
                                          const U32 cmdSeq) {
  // Locked-force print
//  printf("THIS IS M_LOCKED %d \n", m_locked);
  if (m_locked) {
//    printf("HELLO 1 \n");
    this->log_ACTIVITY_HI_GpsLockAcquired();
  } else {
//    printf("HELLO 2 \n");
    this->log_WARNING_HI_GpsLockLost();
  }
  this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}
} // end namespace Gnc
