module Payload {

    enum FileFormat { JPG = 0, PNG = 1 }

    @ Component for processing raw images
    active component PhotoConverter {
        # ----------------------------------------------------------------------
        # General ports
        # ----------------------------------------------------------------------

        @ Sends photo to another component to get processed
        async input port preProcess: Fw.BufferSend

        @ Save photo to disk via buffer logger
        output port postProcess: Fw.BufferSend

        # ----------------------------------------------------------------------
        # Special ports
        # ----------------------------------------------------------------------

        @ Command receive
        command recv port cmdIn

        @ Command registration
        command reg port cmdRegOut

        @ Command response
        command resp port cmdResponseOut

        @ Port for emitting events
        event port Log

        @ Port for emitting text events
        text event port LogText

        @ Port for getting the time
        time get port Time

        @ Telemetry port
        telemetry port Tlm

        # ----------------------------------------------------------------------
        # Commands
        # ----------------------------------------------------------------------

        @ Set the format to convert to from raw image file
        async command SetFormat(
            fileFormat: FileFormat @< Type of file format to convert to
            ) \
        opcode 0x01

        # ----------------------------------------------------------------------
        # Events
        # ----------------------------------------------------------------------

        @ Event where error occurred when setting up camera
        event ProcessError \
        severity warning high \
        format "Process Failure" \

        @ Event file format has been set
        event SetFileFormat(
            fileFormat: FileFormat @< file format to convert to
            ) \
        severity activity high \
        format "The image will be converted to {}" \

        @ Error event where given format for image configuration is invalid
        event InvalidFormatCmd(
            fileFormat: FileFormat @< file format to convert to
            ) \
        severity warning high \
        format "{} is an invalid file format" \

        # ----------------------------------------------------------------------
        # Telemetry
        # ----------------------------------------------------------------------

        @ Total number of files captured
        telemetry photosProcessed: U32 id 0 update on change

    }
}
