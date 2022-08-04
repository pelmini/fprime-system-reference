// ======================================================================
// \title  Camera.hpp
// \author pel
// \brief  cpp file for Camera test harness implementation class
// ======================================================================

#include "Tester.hpp"


#define INSTANCE 0
#define MAX_HISTORY_SIZE 10
#define QUEUE_DEPTH 10

namespace Payload {

  // ----------------------------------------------------------------------
  // Construction and destruction
  // ----------------------------------------------------------------------

  Tester ::
    Tester() :
      CameraGTestBase("Tester", MAX_HISTORY_SIZE),
      component("Camera")
  {
    this->initComponents();
    this->connectPorts();
  }

  Tester ::
    ~Tester()
  {

  }

  // ----------------------------------------------------------------------
  // Tests
  // ----------------------------------------------------------------------

  void Tester ::
    toDo()
  {
    // TODO
  }

  // ----------------------------------------------------------------------
  // Handlers for typed from ports
  // ----------------------------------------------------------------------

  Fw::Buffer Tester ::
    from_allocate_handler(
        const NATIVE_INT_TYPE portNum,
        U32 size
    )
  {
    this->pushFromPortEntry_allocate(size);
    // TODO: Return a value
  }

  void Tester ::
    from_deallocate_handler(
        const NATIVE_INT_TYPE portNum,
        Fw::Buffer &fwBuffer
    )
  {
    this->pushFromPortEntry_deallocate(fwBuffer);
  }

  void Tester ::
    from_sendPhoto_handler(
        const NATIVE_INT_TYPE portNum,
        Fw::Buffer &fwBuffer
    )
  {
    this->pushFromPortEntry_sendPhoto(fwBuffer);
  }

  // ----------------------------------------------------------------------
  // Helper methods
  // ----------------------------------------------------------------------

  void Tester ::
    connectPorts()
  {

    // Run
    this->connect_to_Run(
        0,
        this->component.get_Run_InputPort(0)
    );

    // cmdIn
    this->connect_to_cmdIn(
        0,
        this->component.get_cmdIn_InputPort(0)
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

    // allocate
    this->component.set_allocate_OutputPort(
        0,
        this->get_from_allocate(0)
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

    // deallocate
    this->component.set_deallocate_OutputPort(
        0,
        this->get_from_deallocate(0)
    );

    // sendPhoto
    this->component.set_sendPhoto_OutputPort(
        0,
        this->get_from_sendPhoto(0)
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

  ImgResolution Tester::pickImgResolution() {
    const ImgResolution imgResolution = static_cast<const ImgResolution::t>(
        STest::Pick::lowerUpper(0, 1));

    if(imgResolution.e == 0){
      ASSERT_EQ(imgResolution, ImgResolution::SIZE_640x480);
    } else if (imgResolution.e == 1){
      ASSERT_EQ(imgResolution, ImgResolution::SIZE_800x600);
    }
    return imgResolution;
  }
  ImgFormat Tester::pickImgFormat() {
    const ImgFormat imgFormat = static_cast<const ImgFormat::t>(
        STest::Pick::lowerUpper(0, 1));

    if(imgFormat.e == 0){
      ASSERT_EQ(imgFormat, ImgFormat::RGB);
    } else if (imgFormat.e == 1){
      ASSERT_EQ(imgFormat, ImgFormat::YUYV);
    }
    return imgFormat;
  }

  } // end namespace Payload
