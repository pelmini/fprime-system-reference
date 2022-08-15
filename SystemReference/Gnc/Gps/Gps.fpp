module Gnc {

    @ Component for Gps
    active component Gps {
        # ----------------------------------------------------------------------
        # General ports
        # ----------------------------------------------------------------------

        @ Port to receive serial data
        async input port serialReceive: Drv.SerialRead

        @ Port to provide buffers for serial driver to use
        output port serialBufferOut: Fw.BufferSend

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

        @ A command to force an EVR reporting lock status
        async command GpsReportLockStatus opcode 0x00

        # ----------------------------------------------------------------------
        # Events
        # ----------------------------------------------------------------------

        @ Event where a notification on GPS lock acquired
        event GpsLockAcquired \
        severity activity high \
        format "GPS lock acquired" \

        @ A warning on GPS lock lost
        event GpsLockLost \
        severity warning high \
        format "GPS lock lost" \

        @ Error when GPS data packet is bad
        event BadGpsPacket \
        severity warning high \
        format "Bad GPS packet" \

        @ Error for invalid buffer size
        event BadBufferSize \
        severity warning high \
        format "Bad Buffer Size"

        @ Error for invalid GPGGA
        event InvalidGPGGA \
        severity warning high \
        format "Invalid GPGGA"

        @ Error for parse failure
        event ParseFail \
        severity warning high \
        format "GPS parsing failed"

        # ----------------------------------------------------------------------
        # Telemetry
        # ----------------------------------------------------------------------

        @ The current latitude
        telemetry latitude: F32 update always

        @ The current longitude
        telemetry longitude: F32 update always

        @ The current altitude
        telemetry altitude: F32 update always

        @ The current number of satellites
        telemetry satelliteCount: U32 update always

    }
}
