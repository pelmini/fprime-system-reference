//
// Created by Vivian Bai on 7/26/22.
//

#ifndef CAPTURE_H
#define CAPTURE_H

extern const char *dev_name;


void open_device();
int init_device();
//void start_capturing();
//void mainloop();
//void stop_capturing();
void uninit_device();
void close_device();
int read_frame(void *cameraBuffer, int size, int *readSize);
#endif // CAPTURE_H
