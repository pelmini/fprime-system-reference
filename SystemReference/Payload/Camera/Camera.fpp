module Payload {

    @ Component for Camera
    active component Camera {
        # ----------------------------------------------------------------------
        # General ports
        # ----------------------------------------------------------------------

        @ Port to send telemetry to ground
        async input port Run: Svc.Sched

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
        async command Save

        @ Command to take photo
        async command Take

        # ----------------------------------------------------------------------
        # Events
        # ----------------------------------------------------------------------

        @ Event where error occurred when setting up camera
        event CameraError \
        severity warning high \
        format "Camera setup failed" \

        # ----------------------------------------------------------------------
        # Telemetry
        # ----------------------------------------------------------------------

        @ Total number of files captured
        telemetry photosTaken: U32 id 0 update on change

        @ Total number of commands received
        telemetry commandNum: U32 id 1 update on change

    }
}