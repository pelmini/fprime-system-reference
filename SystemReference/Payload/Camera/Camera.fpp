enum ImgResolution { SIZE_640x480 = 0 , SIZE_800x600 = 1 }
enum ImgFormat { RGB = 0, YUYV = 1 }

module Payload {

    @ Component for Camera
    active component Camera {
        # ----------------------------------------------------------------------
        # General ports
        # ----------------------------------------------------------------------

        @ Port that outputs taken photo to be downloaded
        output port sendPhoto: Fw.BufferSend

        output port allocate: Fw.BufferGet

        output port deallocate: Fw.BufferSend
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

        @ Command to save photo
        async command Save opcode 0x00

        @ Command to take photo
        async command Take opcode 0x01

        @ Command to set the exposure time
        async command ExposureTime( $time: U32 ) opcode 0x02

        @ Command to configure image
        async command ConfigImg( resolution: ImgResolution, $format: ImgFormat) opcode 0x03

        # ----------------------------------------------------------------------
        # Events
        # ----------------------------------------------------------------------

        @ Event where error occurred when setting up camera
        event CameraError(
            device: string size 256 @< Device name
            ) \
        severity warning high \
        format "{} setup failed" \

        @ Event where exposure time is set
        event ExposureTimeSet(
            $time: U32 @< Exposure time
            ) \
        severity activity high \
        format "The exposure time has been set to {} seconds" \

        @ Event image configuration has been set
        event SetImgConfig(
            resolution: ImgResolution @< Image size,
            $format: ImgFormat @< Image format
            ) \
        severity activity high \
        format "The image has size {}, and the format {}" \

        @ Error event where given format for image configuration is invalid
        event InvalidFormat(
            $format: ImgFormat @< Image format
            ) \
        severity warning high \
        format "{} is an invalid format" \

        @ Error event where given size for image configuration is invalid
        event InvalidSize(
            resolution: ImgResolution @< Image size
            ) \
        severity warning high \
        format "{} is an invalid size" \

        @ Error event for invalid time for exposure time
        event InvalidExposureTime(
            $time: U32 @< Exposure time
            ) \
        severity warning high \
        format "{} is an invalid time"

        # ----------------------------------------------------------------------
        # Telemetry
        # ----------------------------------------------------------------------

        @ Total number of files captured
        telemetry photosTaken: U32 id 0 update on change

        @ Total number of commands received
        telemetry commandNum: U32 id 1 update on change

    }
}
