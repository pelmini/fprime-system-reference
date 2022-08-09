//
// Created by Vivian Bai on 7/26/22.
//

#ifndef CAPTURE_H
#define CAPTURE_H
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

#if TGT_OS_TYPE_LINUX
   #include <linux/videodev2.h>
#else
   #define V4L2_PIX_FMT_YUYV 0
   #define V4L2_PIX_FMT_RGB24 1
#endif

int open_device(const char *device_name, int fd);
int init_device(const char *device_name, int fd);
uint32_t set_format(uint32_t height, uint32_t width, uint32_t imgFormat, int fd);
int close_device(int fd);
int read_frame(void *cameraBuffer,uint32_t size, size_t *readSize, int fd);
void set_exposure_time(uint32_t exposureTime, int fd);
#endif // CAPTURE_H
