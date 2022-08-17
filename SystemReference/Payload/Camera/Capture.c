/*
 *  V4L2 video capture example
 *
 *  This program can be used and distributed without restrictions.
 *
 *      This program is provided with the V4L2 API
 * see https://linuxtv.org/docs.php for more information
 */

#include "Capture.h"

//const char *device_name;
//int fd;

#define CLEAR(x) memset(&(x), 0, sizeof(x))


static int xioctl(int fh, int request, void *arg) {
  int r;

  do {
    r = ioctl(fh, request, arg);
  } while (-1 == r && EINTR == errno);

  return r;
}

int read_frame(void *cameraBuffer, uint32_t size, size_t *readSize, int fd) {
  *readSize = read(fd, cameraBuffer, size);
  if (-1 == *readSize && errno != EAGAIN) {
    return -1;
  } else if (-1 == *readSize){
    // need to try again
    return -2;
  }
  return 0;
}

int init_device(const char *device_name, int fd) {
  struct v4l2_capability cap;
  struct v4l2_cropcap cropcap;
  struct v4l2_crop crop;

  if (-1 == xioctl(fd, VIDIOC_QUERYCAP, &cap)) {
    if (EINVAL == errno) {
      fprintf(stderr, "%s is no V4L2 device\n", device_name);
      return -1;
    } else {
      fprintf(stderr, "ERROR: VIDIOC_QUERYCAP: %s \n" , strerror(errno));
      return -1;
    }
  }

  //if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
  //  fprintf(stderr, "%s is not a video capture device \n", device_name);
  //  return -1;
  //}

  if (!(cap.capabilities & V4L2_CAP_READWRITE)) {
    fprintf(stderr, "%s does not support read i/o \n", device_name);
    return -1;
  }

  /* Select video input, video standard and tune here. */

  CLEAR(cropcap);

  cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

  if (0 == xioctl(fd, VIDIOC_CROPCAP, &cropcap)) {
    crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    crop.c = cropcap.defrect; /* reset to default */

    if (-1 == xioctl(fd, VIDIOC_S_CROP, &crop)) {
      return -1;
    }
  }

  return 0;
}

int set_format(uint32_t height, uint32_t width, uint32_t imgFormat, int fd) {
  struct v4l2_format fmt;
  unsigned int min;

  CLEAR(fmt);

  fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  fmt.fmt.pix.width = width;
  fmt.fmt.pix.height = height;
  if (imgFormat == V4L2_PIX_FMT_YUYV) {
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
  } else {
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_RGB24;
  }
  fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;

  if (-1 == xioctl(fd, VIDIOC_S_FMT, &fmt)) {
    printf("ERROR: VIDIOC_S_FMT: %s\n", strerror(errno));
    return -1;
  }

  /* Buggy driver paranoia. */
  min = fmt.fmt.pix.width * 2;
  if (fmt.fmt.pix.bytesperline < min) {
    fmt.fmt.pix.bytesperline = min;
  }
  min = fmt.fmt.pix.bytesperline * fmt.fmt.pix.height;
  if (fmt.fmt.pix.sizeimage < min) {
    fmt.fmt.pix.sizeimage = min;
  }
  return fmt.fmt.pix.sizeimage;
}

// https://stackoverflow.com/questions/61581125/v4l2-absolute-exposure-setting-has-almost-not-effect
// https://forums.raspberrypi.com/viewtopic.php?t=281994
// https://linuxtv.org/downloads/v4l-dvb-apis-new/userspace-api/v4l/ext-ctrls-camera.html?highlight=exposure
void set_exposure_time(uint32_t exposureTime, int fd) {
  struct v4l2_control control;
  control.id = V4L2_CID_EXPOSURE_ABSOLUTE;
  control.value = exposureTime;
  xioctl(fd, VIDIOC_S_CTRL, &control);
}

int close_device(int fd) {
  if (-1 == close(fd))
    return -1;
  fd = -1;
  return 0;
}

int open_device(const char *device_name) {
  struct stat st;
  int fd = -1;

  if (-1 == stat(device_name, &st)) {
    fprintf(stderr, "Cannot identify '%s': %d, %s\\n", device_name, errno,
            strerror(errno));
    return -1;
  }

  if (!S_ISCHR(st.st_mode)) {
    fprintf(stderr, "%s is no device", device_name);
    return -1;
  }

  fd = open(device_name, O_RDWR /* required */ | O_NONBLOCK, 0);

  if (-1 == fd) {
    fprintf(stderr, "Cannot open '%s': %d, %s\\n", device_name, errno,
            strerror(errno));
    return -1;
  }
  return fd;
}
