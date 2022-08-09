#include "SystemReference/Payload/Camera/Capture.h"
#include "SystemReference/Payload/Camera/test/ut/CaptureMock.h"

int open_device(const char *dev_name, int fd) {
//  testDeviceName = dev_name;
  return open_value;
}
int init_device(const char *dev_name, int fd) {
  testDeviceName = dev_name;
  return init_value;
}
uint32_t set_format(uint32_t height, uint32_t width, uint32_t imgFormat,
                int fd) {
  return 0;
}
int close_device(int fd) {
  return 0;
}
int read_frame(void *cameraBuffer, uint32_t size, size_t *readSize, int fd) {
  return 0;
}
void set_exposure_time(uint32_t exposureTime, int fd) {
}