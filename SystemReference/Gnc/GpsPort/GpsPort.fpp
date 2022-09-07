module Gnc {

    @ Latitude at index 0, and longitude at index 1
    array Coordinates = [2] F32

    struct GpsData{
        altitude: F32
        coordinates: Coordinates
        satelliteCount: U32
    }

    @ Port for receiving current gps data
    port GpsDataPort() -> GpsData

}