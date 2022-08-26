// ======================================================================
// \title  ProcessImage.cpp
// \author vbai
// \brief  cpp file for ProcessImage component implementation class
// ======================================================================

#include "Fw/Types/BasicTypes.hpp"
#include "opencv2/imgcodecs.hpp"
#include <SystemReference/Payload/ImageProcessor/ImageProcessor.hpp>
#include <opencv2/opencv.hpp>

namespace Payload {

// ----------------------------------------------------------------------
// Construction, initialization, and destruction
// ----------------------------------------------------------------------

ImageProcessor ::ImageProcessor(const char *const compName)
    : ImageProcessorComponentBase(compName) {}

void ImageProcessor ::init(const NATIVE_INT_TYPE queueDepth,
                           const NATIVE_INT_TYPE instance) {
  ImageProcessorComponentBase::init(queueDepth, instance);
}

ImageProcessor ::~ImageProcessor() {}

// ----------------------------------------------------------------------
// Handler implementations for user-defined typed input ports
// ---------------------------------------------------------------------+

void ImageProcessor ::imageData_handler(const NATIVE_INT_TYPE portNum,
                                        Payload::RawImageData &ImageData) {

  Fw::Buffer imgBuffer;
  std::vector<uchar> buffer;
  int type = 0;

  switch (ImageData.getcolorFormat().e){
    case Payload::ColorFormat::YUYV:
      type = CV_8UC2;
      break;
    case Payload::ColorFormat::RGB:
      type = CV_8UC3;
      break;
    default:
      FW_ASSERT(0);
  }

  cv::Mat image(ImageData.getheight(), ImageData.getwidth(), type, (void *)ImageData.getimgData().getData());

  if (!image.data) {
    this->log_WARNING_HI_ProcessError();
  }

  cv::imencode(m_fileFormat, image, buffer);

  //debug purposes
  char *name = "image";
  cv::imwrite(strcat(name, m_fileFormat), image);

  FW_ASSERT(imgBuffer.getSize() >= buffer.size());

  memcpy(imgBuffer.getData(), buffer.data(), buffer.size());
  imgBuffer.setSize(buffer.size());

  this->postProcess_out(0, imgBuffer);
}

// ----------------------------------------------------------------------
// Command handler implementations
// ----------------------------------------------------------------------

void ImageProcessor ::SetFormat_cmdHandler(const FwOpcodeType opCode,
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
