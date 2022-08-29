module Payload {
    enum ColorFormat { RGB = 0, YUYV = 1 }

    struct RawImageData{
        height: U32 @< height of image
        width: U32 @< width of image
        typeFormat: I32 @< pixel format
        imgData: Fw.Buffer @< buffer that contains image data
    }

    @ Port for receiving image data
    port ImageData(
        ref ImageData: RawImageData @< Port that carries raw image data
    )
}