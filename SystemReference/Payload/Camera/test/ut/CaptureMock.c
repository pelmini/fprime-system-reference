#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <getopt.h>             /* getopt_long() */

#include <fcntl.h>              /* low-level i/o */
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>

const char *testDeviceName;
int fd;

void open_device(const char *testDeviceName, int fd){
    testDeviceName = "/dev/video0";
}
void init_device(const char *testDeviceName, int fd){
  testDeviceName = "/dev/video0";
}
void set_format(u_int32_t height, u_int32_t width, u_int32_t imgFormat, int fd){

}
void close_device(int fd){

}
void read_frame(void *cameraBuffer,u_int32_t size, size_t *readSize, int fd){

}
void set_exposure_time(u_int32_t exposureTime, int fd){

}