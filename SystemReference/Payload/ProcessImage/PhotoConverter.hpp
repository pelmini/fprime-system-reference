// ======================================================================
// \title  PhotoConverter.hpp
// \author vbai
// \brief  hpp file for PhotoConverter component implementation class
// ======================================================================

#ifndef PhotoConverter_HPP
#define PhotoConverter_HPP

#include "SystemReference/Payload/PhotoConverter/PhotoConverterComponentAc.hpp"

namespace Payload {

class PhotoConverter : public PhotoConverterComponentBase {

public:
  // ----------------------------------------------------------------------
  // Construction, initialization, and destruction
  // ----------------------------------------------------------------------

  //! Construct object PhotoConverter
  //!
  PhotoConverter(const char *const compName /*!< The component name*/
  );

  //! Initialize object PhotoConverter
  //!
  void init(const NATIVE_INT_TYPE queueDepth,  /*!< The queue depth*/
            const NATIVE_INT_TYPE instance = 0 /*!< The instance number*/
  );

  //! Destroy object PhotoConverter
  //!
  ~PhotoConverter();

  PRIVATE :

      // ----------------------------------------------------------------------
      // Handler implementations for user-defined typed input ports
      // ----------------------------------------------------------------------

      //! Handler implementation for preProcess
      //!
      void
      preProcess_handler(const NATIVE_INT_TYPE portNum, /*!< The port number*/
                         Fw::Buffer &fwBuffer);

  PRIVATE :

      // ----------------------------------------------------------------------
      // Command handler implementations
      // ----------------------------------------------------------------------

      //! Implementation for SetFormat command handler
      //! Set the format to convert to from raw image file
      void
      SetFormat_cmdHandler(
          const FwOpcodeType opCode, /*!< The opcode*/
          const U32 cmdSeq,          /*!< The command sequence number*/
          Payload::FileFormat
              fileFormat /*!< Type of file format to convert to*/
      );

  const char *m_fileFormat;
};

} // end namespace Payload

#endif
