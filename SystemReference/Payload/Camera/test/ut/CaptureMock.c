#include "SystemReference/Payload/Camera/Capture.h"
#include "SystemReference/Payload/Camera/test/ut/CaptureMock.h"


int open_device(const char *testDeviceName, int fd) {
  testDeviceName = "/dev/video0";
  return 0;
}
int init_device(const char *testDeviceName, int fd) {
  testDeviceName = "/dev/video0";
  return 0;
}
u_int32_t set_format(u_int32_t height, u_int32_t width, u_int32_t imgFormat,
                int fd) {
  return 0;
}
int close_device(int fd) {
  return 0;
}
int read_frame(void *cameraBuffer, u_int32_t size, size_t *readSize, int fd) {
  return 0;
}
void set_exposure_time(u_int32_t exposureTime, int fd) {
}
