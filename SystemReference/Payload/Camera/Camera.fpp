module Payload {

    enum ImgResolution { SIZE_640x480 = 0 , SIZE_800x600 = 1 }
    enum CameraAction { SAVE = 0, PROCESS = 1 }

    @ Component for Camera
    active component Camera {
        # ----------------------------------------------------------------------
        # General ports
        # ----------------------------------------------------------------------

        @ Sends photo to another component to get processed
        output port process: ImageData

        @ Allocates memory to hold photo buffers
        output port allocate: Fw.BufferGet

        @ Save photo to disk
        output port $save: Fw.BufferSend

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

        @ Set the action that camera should take
        async command TakeAction(
            cameraAction: CameraAction @< State where camera either saves or takes photo
            ) \
        opcode 0x01

        @ Set the exposure time
        async command ExposureTime(
            $time: U32
            ) \
        opcode 0x02

        @ Command to configure image
        async command ConfigImg(
            resolution: ImgResolution
            $format: ColorFormat
            ) \
        opcode 0x03

        # ----------------------------------------------------------------------
        # Events
        # ----------------------------------------------------------------------

        @ Event where error occurred when setting up camera
        event CameraOpenError \
        severity warning high \
        format "Camera failed to open" \

        event CameraSave \
        severity activity low \
        format "Image was saved"

        event CameraProcess \
        severity activity low \
        format "Image will be processed" \

        @ Event where exposure time is set
        event ExposureTimeSet(
            $time: U32 @< Exposure time
            ) \
        severity activity high \
        format "The exposure time has been set to {} seconds" \

        @ Event image configuration has been set
        event SetImgConfig(
            resolution: ImgResolution @< Image size,
            $format: ColorFormat @< Image format
            ) \
        severity activity high \
        format "The image has size {}, and the color format {}" \

        @ Error event where given format for image configuration is invalid
        event InvalidFormatCmd(
            $format: ColorFormat @< Image format
            ) \
        severity warning high \
        format "{} is an invalid color format" \

        @ Error event where given size for image configuration is invalid
        event InvalidSizeCmd(
            resolution: ImgResolution @< Image size
            ) \
        severity warning high \
        format "{} is an invalid size" \

        @ Error event for invalid time for exposure time
        event InvalidExposureTimeCmd(
            $time: U32 @< Exposure time
            ) \
        severity warning high \
        format "{} is an invalid time"

        event BlankFrame \
        severity warning high \
        format "Error: Blank frame was grabbed"

        event SaveError \
        severity warning high \
        format "Failed to save image"

        event InvalidTakeCmd \
        severity warning high \
        format "Invalid camera take command"

        # ----------------------------------------------------------------------
        # Telemetry
        # ----------------------------------------------------------------------

        @ Total number of files captured
        telemetry photosTaken: U32 id 0 update on change

        @ Total number of commands received
        telemetry commandNum: U32 id 1 update on change

    }
}
