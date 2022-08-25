// ======================================================================
// \title  PhotoConverter.cpp
// \author vbai
// \brief  cpp file for PhotoConverter component implementation class
// ======================================================================

#include "Fw/Types/BasicTypes.hpp"
#include "opencv2/imgcodecs.hpp"
#include <SystemReference/Payload/PhotoConverter/PhotoConverter.hpp>
#include <opencv2/opencv.hpp>

namespace Payload {

// ----------------------------------------------------------------------
// Construction, initialization, and destruction
// ----------------------------------------------------------------------

PhotoConverter ::PhotoConverter(const char *const compName)
    : PhotoConverterComponentBase(compName) {}

void PhotoConverter ::init(const NATIVE_INT_TYPE queueDepth,
                           const NATIVE_INT_TYPE instance) {
  PhotoConverterComponentBase::init(queueDepth, instance);
}

PhotoConverter ::~PhotoConverter() {}

// ----------------------------------------------------------------------
// Handler implementations for user-defined typed input ports
// ---------------------------------------------------------------------+

void PhotoConverter ::preProcess_handler(const NATIVE_INT_TYPE portNum,
                                         Fw::Buffer &fwBuffer) {
  char name[] = "image";
  char *fileName;
  bool saveStatus = false;
  Fw::Buffer imgBuffer;
  std::vector<uchar> buffer;
  // create an opencv object
  cv::Mat rawImage(1, fwBuffer.getSize(), CV_8UC3);

  // read the image from memery
  cv::Mat image = cv::imdecode(rawImage, cv::ImreadModes::IMREAD_UNCHANGED);

  if (!image.data) {
    this->log_WARNING_HI_ProcessError();
  }

  // Save file to disk and save to memory buffer
  if (strcmp(m_fileFormat, ".jpg") == 0) {
    fileName = strcat(name, m_fileFormat);
    FW_ASSERT(fileName == "image.jpg");
    saveStatus = cv::imwrite(fileName, image);

    // compresses the image and stores it in buffer
    // that is resized to fit the result
    cv::imencode(m_fileFormat, image, buffer);
    imgBuffer.setData(buffer.data());
    imgBuffer.setSize(buffer.size());

  } else if (strcmp(m_fileFormat, ".png") == 0) {
    fileName = strcat(name, m_fileFormat);
    FW_ASSERT(fileName == "image.png");

    saveStatus = cv::imwrite(fileName, image);
    cv::imencode(m_fileFormat, image, buffer);
    imgBuffer.setData(buffer.data());
    imgBuffer.setSize(buffer.size());
  }

  if (!saveStatus) {
    this->log_WARNING_HI_SaveError();
  } else {
    this->postProcess_out(0, imgBuffer);
  }
  //    namedWindow( "output", cv::WINDOW_AUTOSIZE);
  //    cv::imshow("output", image);
  //    cv::waitKey(0);
}

// ----------------------------------------------------------------------
// Command handler implementations
// ----------------------------------------------------------------------

void PhotoConverter ::SetFormat_cmdHandler(const FwOpcodeType opCode,
                                           const U32 cmdSeq,
                                           Payload::FileFormat fileFormat) {
  bool validFormat = true;

  if (fileFormat == FileFormat::JPG) {
    m_fileFormat = ".jpg";
  } else if (fileFormat == FileFormat::PNG) {
    m_fileFormat = ".png";
  } else {
    this->log_WARNING_HI_InvalidFormatCmd(fileFormat);
    validFormat = false;
  }

  this->log_ACTIVITY_HI_SetFileFormat(fileFormat);
  this->cmdResponse_out(opCode, cmdSeq,
                        validFormat ? Fw::CmdResponse::OK
                                    : Fw::CmdResponse::EXECUTION_ERROR);
}

} // end namespace Payload
