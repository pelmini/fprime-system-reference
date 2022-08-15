// ======================================================================
// \title  Gps.hpp
// \author vbai
// \brief  cpp file for Gps test harness implementation class
// ======================================================================

#include "Tester.hpp"
#include "Fw/Types/BasicTypes.hpp"

#define INSTANCE 0
#define MAX_HISTORY_SIZE 10
#define QUEUE_DEPTH 10

namespace Gnc {

  // ----------------------------------------------------------------------
  // Construction and destruction
  // ----------------------------------------------------------------------

  Tester ::
    Tester() :
      GpsGTestBase("Tester", MAX_HISTORY_SIZE),
      component("Gps")
  {
    this->initComponents();
    this->connectPorts();
//    this->component.setup();
  }

  Tester ::
    ~Tester()
  {

  }

  // ----------------------------------------------------------------------
  // Tests
  // ----------------------------------------------------------------------

  void Tester ::
      testGetGpsData()
  {
    Fw::Buffer serBuffer;
   char data[] = {"$GPGGA,141202.00,3000.1234567,N,06111.8901234,W,1,10,1.10,123.456,M,19.876,M,0.11,0000*20"};

    Drv::SerialReadStatus status = Drv::SerialReadStatus::SER_OK;
    serBuffer.setData(reinterpret_cast<U8*>(data));
    serBuffer.setSize(sizeof(data));
    invoke_to_serialReceive(0, serBuffer, status);
    this->component.doDispatch();

    ASSERT_TLM_SIZE(4);
    ASSERT_TLM_altitude(0, 123.45600128173828);
    ASSERT_TLM_altitude_SIZE(1);

    ASSERT_TLM_latitude(0, 30.002058029174805);
    ASSERT_TLM_latitude_SIZE(1);

    ASSERT_TLM_longitude(0, -61.198169708251953);
    ASSERT_TLM_longitude_SIZE(1);

    ASSERT_TLM_satelliteCount(0, 10);
    ASSERT_TLM_satelliteCount_SIZE(1);

    ASSERT_EVENTS_SIZE(1);
    ASSERT_EVENTS_GpsLockAcquired_SIZE(1);

    this->clearEvents();
    this->clearTlm();
  }

  void Tester::testGetGpsData2() {
    Fw::Buffer serBuffer;
    char data[] = {"JUNK STRING$GPGGA,141202.00,3000.1234567,N,06111.8901234,W,1,10,1.10,123.456,M,19.876,M,0.11,0000*20"};
    Drv::SerialReadStatus status = Drv::SerialReadStatus::SER_OK;
    serBuffer.setData(reinterpret_cast<U8*>(data));
    serBuffer.setSize(sizeof(data));
    invoke_to_serialReceive(0, serBuffer, status);
    this->component.doDispatch();

    ASSERT_TLM_SIZE(4);
    ASSERT_TLM_altitude(0, 123.45600128173828);
    ASSERT_TLM_altitude_SIZE(1);

    ASSERT_TLM_latitude(0, 30.002058029174805);
    ASSERT_TLM_latitude_SIZE(1);

    ASSERT_TLM_longitude(0, -61.198169708251953);
    ASSERT_TLM_longitude_SIZE(1);

    ASSERT_TLM_satelliteCount(0, 10);
    ASSERT_TLM_satelliteCount_SIZE(1);

    ASSERT_EVENTS_SIZE(1);
    ASSERT_EVENTS_GpsLockAcquired_SIZE(1);

    this->clearEvents();
    this->clearTlm();
  }

  void Tester::testLockLost() {
    Fw::Buffer serBuffer;

    char data[] = {"$GPGGA,141202.00,3000.1234567,N,06111.8901234,W,1,10,1.10,123.456,M,19.876,M,0.11,0000*20"};
    Drv::SerialReadStatus status = Drv::SerialReadStatus::SER_OK;
    serBuffer.setData(reinterpret_cast<U8*>(data));
    serBuffer.setSize(sizeof(data));
    invoke_to_serialReceive(0, serBuffer, status);
    this->component.doDispatch();

    char data2[] = {"$GPGGA,141202.00,3000.1234567,N,06111.8901234,W,0,10,1.10,123.456,M,19.876,M,0.11,0000*20"};
    Drv::SerialReadStatus status2 = Drv::SerialReadStatus::SER_OK;
    serBuffer.setData(reinterpret_cast<U8*>(data2));
    serBuffer.setSize(sizeof(data2));
    invoke_to_serialReceive(0, serBuffer, status2);
    this->component.doDispatch();

    ASSERT_EVENTS_SIZE(2);
    ASSERT_EVENTS_GpsLockLost_SIZE(1);
    this->clearEvents();
  }

  void Tester::testLockStatusCmdLost() {
    this->sendCmd_GpsReportLockStatus(0,0);
    this->component.doDispatch();
    ASSERT_EVENTS_SIZE(1);
    ASSERT_EVENTS_GpsLockLost_SIZE(1);
    ASSERT_CMD_RESPONSE_SIZE(1);
    ASSERT_CMD_RESPONSE(0, Gps::OPCODE_GPSREPORTLOCKSTATUS, 0, Fw::CmdResponse::OK);
    this->clearEvents();
  }

  void Tester::testLockStatusCmdAcquired() {
    Fw::Buffer serBuffer;

    char data[] = {"$GPGGA,141202.00,3000.1234567,N,06111.8901234,W,1,10,1.10,123.456,M,19.876,M,0.11,0000*20"};
    Drv::SerialReadStatus status = Drv::SerialReadStatus::SER_OK;
    serBuffer.setData(reinterpret_cast<U8*>(data));
    serBuffer.setSize(sizeof(data));
    invoke_to_serialReceive(0, serBuffer, status);
    this->component.doDispatch();

    this->sendCmd_GpsReportLockStatus(0,0);
    this->component.doDispatch();
    ASSERT_EVENTS_SIZE(2);
    ASSERT_EVENTS_GpsLockAcquired_SIZE(2);
    ASSERT_CMD_RESPONSE_SIZE(1);
    ASSERT_CMD_RESPONSE(0, Gps::OPCODE_GPSREPORTLOCKSTATUS, 0, Fw::CmdResponse::OK);
    this->clearEvents();
  }

  void Tester::testBadGPSPacket() {
    Fw::Buffer serBuffer;
    char data[] = {"$GPGGA,141202.00,3000.1234567,W,0,10,1.10,123.456,M,19.876,M,0.11,0000*20"};
    Drv::SerialReadStatus status = Drv::SerialReadStatus::SER_OTHER_ERR;
    serBuffer.setData(reinterpret_cast<U8*>(data));
    serBuffer.setSize(sizeof(data));

    invoke_to_serialReceive(0, serBuffer, status);
    this->component.doDispatch();
    ASSERT_EVENTS_SIZE(2);
    ASSERT_EVENTS_BadGpsPacket_SIZE(1);
    ASSERT_EVENTS_InvalidGPGGA_SIZE(1);
  }

  void Tester::testParseFail() {
    Fw::Buffer serBuffer;
    char data[] = {"$GPGGA"};
    Drv::SerialReadStatus status = Drv::SerialReadStatus::SER_OK;
    invoke_to_serialReceive(0, serBuffer, status);
    serBuffer.setData(reinterpret_cast<U8*>(data));
    serBuffer.setSize(sizeof(data));
    this->component.doDispatch();

    ASSERT_EVENTS_SIZE(2);


  }


  // ----------------------------------------------------------------------
  // Handlers for typed from ports
  // ----------------------------------------------------------------------

  void Tester ::
    from_serialBufferOut_handler(
        const NATIVE_INT_TYPE portNum,
        Fw::Buffer &fwBuffer
    )
  {
    this->pushFromPortEntry_serialBufferOut(fwBuffer);
  }

  // ----------------------------------------------------------------------
  // Helper methods
  // ----------------------------------------------------------------------

  void Tester ::
    connectPorts()
  {

    // cmdIn
    this->connect_to_cmdIn(
        0,
        this->component.get_cmdIn_InputPort(0)
    );

    // serialReceive
    this->connect_to_serialReceive(
        0,
        this->component.get_serialReceive_InputPort(0)
    );

    // Log
    this->component.set_Log_OutputPort(
        0,
        this->get_from_Log(0)
    );

    // LogText
    this->component.set_LogText_OutputPort(
        0,
        this->get_from_LogText(0)
    );

    // Time
    this->component.set_Time_OutputPort(
        0,
        this->get_from_Time(0)
    );

    // Tlm
    this->component.set_Tlm_OutputPort(
        0,
        this->get_from_Tlm(0)
    );

    // cmdRegOut
    this->component.set_cmdRegOut_OutputPort(
        0,
        this->get_from_cmdRegOut(0)
    );

    // cmdResponseOut
    this->component.set_cmdResponseOut_OutputPort(
        0,
        this->get_from_cmdResponseOut(0)
    );

    // serialBufferOut
    this->component.set_serialBufferOut_OutputPort(
        0,
        this->get_from_serialBufferOut(0)
    );




  }

  void Tester ::
    initComponents()
  {
    this->init();
    this->component.init(
        QUEUE_DEPTH, INSTANCE
    );
  }

} // end namespace Gnc
