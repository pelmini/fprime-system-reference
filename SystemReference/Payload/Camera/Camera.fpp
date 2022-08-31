module Payload {

    enum ImgResolution { SIZE_640x480 = 0 , SIZE_800x600 = 1 }
    enum CameraAction { SAVE = 0, PROCESS = 1 }
    enum ColorFormat { RGB = 0, YUYV = 1 }

    @ Component to capture raw images
    active component Camera {
        # ----------------------------------------------------------------------
        # General ports
        # ----------------------------------------------------------------------

        @ Sends photo to another component to get processed
        output port process: ImageData

        @ Allocates memory to hold photo buffers
        output port allocate: Fw.BufferGet

        @ Save photo to disk, send image to buffer logger
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

        @ Event where camera is already open
        event CameraAlreadyOpen \
        severity activity low \
        format "The Camera is already open" \

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
        format "{} seconds is an invalid time" \

        @ Exposure time failed to set
        event ExposureTimeFail(
            $time: U32 @< Exposure time
            ) \
        severity warning high \
        format "Exposure time of {} seconds failed to set" \

        @ Failed to set size and color format
        event ImgConfigSetFail(
            resolution: ImgResolution @< Image size
            $format: ColorFormat @< Image format
            ) \
        severity warning high \
        format "Image resolution of {} and color format {} failed to set" \

        @ Blank frame Error
        event BlankFrame \
        severity warning high \
        format "Error: Blank frame was grabbed" \

        @ Image failed to save
        event SaveError \
        severity warning high \
        format "Failed to save image" \

        @ Invalid take command error
        event InvalidTakeCmd \
        severity warning high \
        format "Invalid camera take command" \

        @ Invalid buffer size error
        event InvalidBufferSizeError(
            imgBufferSize: U32 @< size of imgBuffer to hold image data
            imgSize: U32 @< size of image
        ) \
        severity warning high \
        format "imgBuffer of size {} is less than imgSize of size {}"

        @ Captured frane is not continuous
        event NotContinuous \
        severity warning high \
        format "Captured matrix frame has gaps."


        # ----------------------------------------------------------------------
        # Telemetry
        # ----------------------------------------------------------------------

        @ Total number of files captured
        telemetry photosTaken: U32 id 0 update on change

    }
}
