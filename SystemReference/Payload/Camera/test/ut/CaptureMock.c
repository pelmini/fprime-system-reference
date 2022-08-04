#include "CaptureMock.h"

const char *testDeviceName;
int fd;


void mock_open_device(const char *testDeviceName, int fd) {
  testDeviceName = "/dev/video0";
}
void mock_init_device(const char *testDeviceName, int fd) {
  testDeviceName = "/dev/video0";
}
void mock_set_format(u_int32_t height, u_int32_t width, u_int32_t imgFormat,
                int fd) {

}
void mock_close_device(int fd) {

}
void mock_read_frame(void *cameraBuffer, u_int32_t size, size_t *readSize, int fd) {

}
void mock_set_exposure_time(u_int32_t exposureTime, int fd) {

}
