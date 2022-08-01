//
// Created by Vivian Bai on 7/26/22.
//

#ifndef SYSTEMREFERENCE_CAPTURE_H
#define SYSTEMREFERENCE_CAPTURE_H

extern const char *dev_name;


void open_device();
int init_device();
//void start_capturing();
//void mainloop();
//void stop_capturing();
void uninit_device();
void close_device();
static int read_frame(void *cameraBuffer, int size, int *readSize);
#endif // SYSTEMREFERENCE_CAPTURE_H
