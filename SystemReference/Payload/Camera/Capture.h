//
// Created by Vivian Bai on 7/26/22.
//

#ifndef SYSTEMREFERENCE_CAPTURE_H
#define SYSTEMREFERENCE_CAPTURE_H

extern char * dev_name;


void open_device();
void init_device();
void start_capturing();
void mainloop();
void stop_capturing();
void uninit_device();
void close_device();
#endif // SYSTEMREFERENCE_CAPTURE_H
