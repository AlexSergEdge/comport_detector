#ifndef DEVICE_DETECTOR_H
#define DEVICE_DETECTOR_H

#include <iostream>
#include <termios.h>

void setup_port(termios *port_settings, int speed);
bool is_device_connected(const char *port_number);

bool check_signal_lines(int fd);


#endif // DEVICE_DETECTOR_H