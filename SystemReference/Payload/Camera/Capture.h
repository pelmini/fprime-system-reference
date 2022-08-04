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
#include <linux/videodev2.h>

extern const char *dev_name;


void open_device();
int init_device();
//void start_capturing();
//void mainloop();
//void stop_capturing();
u_int32_t set_format(u_int32_t height, u_int32_t width, u_int32_t imgFormat);
static void uninit_device();
void close_device();
int read_frame(void *cameraBuffer,u_int32_t size, size_t *readSize);
void set_exposure_time(u_int32_t exposureTime);
#endif // CAPTURE_H
