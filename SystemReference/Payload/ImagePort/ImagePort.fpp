module Payload {
    struct RawImageData{
        height: U32 @< height of image
        width: U32 @< width of image
        imgData: Fw.Buffer @< buffer that contains image data
    }

    @ Port for receiving current gps data
    port ImageData(
        ref ImageData: RawImageData @< Port that carries raw image data
    )
}