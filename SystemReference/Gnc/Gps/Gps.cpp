// ======================================================================
// \title  Gps.cpp
// \author lestarch, djwait, vbai
// \brief  cpp file for Gps component implementation class
// ======================================================================

#include "Fw/Logger/Logger.hpp"
#include "Fw/Com/ComPacket.hpp"
#include "Fw/Types/BasicTypes.hpp"
#include <SystemReference/Gnc/Gps/Gps.hpp>
#include <cstring>



namespace Gnc {

// ----------------------------------------------------------------------
// Construction, initialization, and destruction
// ----------------------------------------------------------------------

Gps ::Gps(const char *const compName)
    : GpsComponentBase(compName), m_locked(false),
      m_inRing(m_ringUartBuffer, UART_READ_BUFF_SIZE) {}

void Gps ::init(const NATIVE_INT_TYPE queueDepth,
                const NATIVE_INT_TYPE instance) {
  GpsComponentBase::init(queueDepth, instance);
}

Gps ::~Gps() {}

// ----------------------------------------------------------------------
// Handler implementations for user-defined typed input ports
// ----------------------------------------------------------------------
Gnc::GpsData Gps ::getAltitude_handler(const NATIVE_INT_TYPE portNum) {
  GpsData altitude = m_gpsData.getaltitude();
  return altitude;
}

Gnc::GpsData Gps ::getCoordinates_handler(const NATIVE_INT_TYPE portNum) {
  return m_gpsData.getcoordinates();;
}

Gnc::GpsData Gps ::getSatelliteCount_handler(const NATIVE_INT_TYPE portNum) {
  GpsData satelliteCount = m_satelliteCount;
  return satelliteCount;
}

// Read data from sensor, comes in as serializable recvBuffer
void Gps ::gpsIn_handler(const NATIVE_INT_TYPE portNum, Fw::Buffer &recvBuffer,
                         const Drv::RecvStatus &recvStatus) {
  // Check whether there is data to process
  if (recvStatus.e == Drv::RecvStatus::RECV_OK) {
    // There is: process the data
    processBuffer(recvBuffer);
  }
  // Deallocate the buffer
  this->gpsBuffer_out(0, recvBuffer);
}

// ----------------------------------------------------------------------
// Command handler implementations
// ----------------------------------------------------------------------

void Gps ::GpsReportLockStatus_cmdHandler(const FwOpcodeType opCode,
                                          const U32 cmdSeq) {
  // Generate EVR of present lock status
  if (m_locked) {
    this->log_ACTIVITY_HI_GpsLockAcquired();
  } else {
    this->log_WARNING_HI_GpsLockLost();
  }
  // complete command
  this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}

// ----------------------------------------------------------------------
// Helper Functions
// ----------------------------------------------------------------------
void Gps::processBuffer(Fw::Buffer &buffer) {
  const U32 bufferSize = buffer.getSize();
  U8 *const bufferData = buffer.getData();
  // Current offset into buffer
  U32 offset = 0;
  // Remaining data in buffer
  U32 remaining = bufferSize;

  for (U32 i = 0; i < bufferSize; ++i) {
    // If there is no data left, exit the loop
    if (remaining == 0) {
      break;
    }
    // Compute the size of data to serialize
    const NATIVE_UINT_TYPE ringFreeSize = m_inRing.get_free_size();
    const NATIVE_UINT_TYPE serSize = (ringFreeSize <= remaining) ?
                                                                 ringFreeSize : static_cast<NATIVE_UINT_TYPE>(remaining);
    // Serialize data into the ring buffer
    const Fw::SerializeStatus status =
        m_inRing.serialize(&bufferData[offset], serSize);
    // If data does not fit, there is a coding error
    FW_ASSERT(status == Fw::FW_SERIALIZE_OK, status, offset, serSize);
    // Process the data
    processRing();
    // Update buffer offset and remaining
    offset += serSize;
    remaining -= serSize;
  }

  // In every iteration, either remaining == 0 and we break out
  // of the loop, or we consume at least one byte from the buffer.
  // So there should be no data left in the buffer.
  FW_ASSERT(remaining == 0, remaining);
}

void Gps::processRing() {
  U8 gpggaString[7];
  // no magic number change into a global/macro
  U8 gpsString[83];
  // The number of remaining bytes in the ring buffer
  int statusGPGGA = 0;
  GpsPacket packet;

  // Process the ring buffer looking for GPGGA
  for (U32 i = 0; i < m_inRing.get_allocated_size(); i++) {
    U8 startToken;
    m_inRing.peek(startToken);
    // check status
    if(startToken != '$'){
      m_inRing.rotate(1);
      continue;
    }

    // gotta peak gpgga string

    if(strcmp(reinterpret_cast<const char *>(gpggaString), "$GPGGA") != 0){
      m_inRing.rotate(1);
      continue;
    }


    // peeksize is number of data we want to read out
    U32 peekSize = FW_MIN(sizeof (gpsString), m_inRing.get_allocated_size());
    m_inRing.peek(gpsString, peekSize);
    gpsString[peekSize] = 0;

    statusGPGGA =
        sscanf(reinterpret_cast<char *>(gpsString), "$GPGGA,%f,%f,%c,%f,%c,%u,%u,%f", &packet.utcTime,
               &packet.dmNS, &packet.northSouth, &packet.dmEW, &packet.eastWest,
               &packet.lock, &packet.count, &packet.altitude);

    if (statusGPGGA == 8) {
      getGpsData(packet);
      m_inRing.rotate(peekSize);
      break;
    } else if  (statusGPGGA == 0) {
      m_inRing.rotate(1);
    } else {
      this->log_WARNING_HI_ParseFail();
      m_inRing.rotate(1);
    }
  }
}

void Gps::getGpsData(GpsPacket packet){
  Coordinates coordinates;
  float lat = 0.0f;
  float lon = 0.0f;

  //GPS packet locations are of the form: ddmm.mmmm
  //We will convert to lat/lon in degrees only before downlinking
  //Latitude degrees, add on minutes (converted to degrees), multiply by direction
  lat = (U32)(packet.dmNS/100.0f);
  lat = lat + (packet.dmNS - (lat * 100.0f))/60.0f;
  lat = lat * ((packet.northSouth == 'N') ? 1 : -1);
  //Longitude degrees, add on minutes (converted to degrees), multiply by direction
  lon = (U32)(packet.dmEW/100.0f);
  lon = lon + (packet.dmEW - (lon * 100.0f))/60.f;
  lon = lon * ((packet.eastWest == 'E') ? 1 : -1);

  coordinates[0] = lat;
  coordinates[1] = lon;
  m_gpsData.setcoordinates(coordinates);
  m_gpsData.setsatelliteCount(packet.count);
  m_gpsData.setaltitude(packet.altitude);

  this->tlmWrite_satelliteCount(this->m_gpsData.getsatelliteCount());
  this->tlmWrite_altitude(this->m_gpsData.getaltitude());
  this->tlmWrite_coordinates(this->m_gpsData.getcoordinates());
}
} // namespace Gnc
