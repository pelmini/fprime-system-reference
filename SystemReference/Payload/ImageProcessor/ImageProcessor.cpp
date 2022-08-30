// ======================================================================
// \title  ProcessImage.cpp
// \author vbai
// \brief  cpp file for ProcessImage component implementation class
// ======================================================================

#include "Fw/Types/BasicTypes.hpp"
#include "opencv2/imgcodecs.hpp"
#include <SystemReference/Payload/ImageProcessor/ImageProcessor.hpp>
#include <opencv2/opencv.hpp>

static const NATIVE_INT_TYPE BUFFER_SIZE = 10*1024*1024;
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

  std::vector<uchar> buffer(BUFFER_SIZE);
  cv::Mat image(ImageData.getheight(), ImageData.getwidth(), ImageData.getpixelFormat(), (void *)ImageData.getimgData().getData());
  if (!image.data) {
    this->log_WARNING_HI_ProcessError();
  }
  cv::imencode(m_fileFormat, image, buffer);
  memcpy(ImageData.getimgData().getData(), buffer.data(), buffer.size());
  const_cast<Fw::Buffer &>(ImageData.getimgData()).setSize(buffer.size());
  this->postProcess_out(0, const_cast<Fw::Buffer &>(ImageData.getimgData()));
}

// ----------------------------------------------------------------------
// Command handler implementations
// ----------------------------------------------------------------------

void ImageProcessor ::SetFormat_cmdHandler(const FwOpcodeType opCode,
                                           const U32 cmdSeq,
                                           Payload::FileFormat fileFormat) {
  bool validFormat = false;

  switch (fileFormat.e){
      case FileFormat::JPG:
      m_fileFormat = ".jpg";
      validFormat = true;
      this->log_ACTIVITY_HI_SetFileFormat(fileFormat);
      break;
    case FileFormat::PNG:
      m_fileFormat = ".png";
      validFormat = true;
      this->log_ACTIVITY_HI_SetFileFormat(fileFormat);
      break;
    default:
      this->log_WARNING_HI_InvalidFormatCmd(fileFormat);
      FW_ASSERT(0);
    }
    this->cmdResponse_out(opCode, cmdSeq,
                        validFormat ? Fw::CmdResponse::OK
                                    : Fw::CmdResponse::EXECUTION_ERROR);
}

} // end namespace Payload
