#include "include/device_detector.h"
#include <stdio.h>
#include <termios.h>  
#include <string.h>   // strerror
#include <errno.h>    // errno
#include <fcntl.h>  // function to open COM ports, flags
#include <unistd.h>

using namespace std;


/* Base settings */
void setup_port(termios *port_settings, int speed) {

    cfsetispeed(port_settings, speed);                                  // input baud rate
    cfsetospeed(port_settings, speed);                                  // output baud rate
    
    port_settings->c_cflag = (port_settings->c_cflag & ~CSIZE) | CS8;   // 8-bit characters
    port_settings->c_iflag &= ~IGNBRK;                                  // disable break processing
    port_settings->c_lflag = 0;                                         // no signaling chars, no echo, no canonical processing
    port_settings->c_oflag = 0;                                         // no remapping, no delays
    port_settings->c_cc[VMIN] = 0;                                      // read doesn't block
    port_settings->c_cc[VTIME] = 5;                                     // read timeout 0.5 secs

    port_settings->c_iflag &= ~(IXON | IXOFF | IXANY);                  // shut off xon/xoff ctrl

    port_settings->c_cflag |= (CLOCAL | CREAD);                         // ignore modem controls, enable reading
    port_settings->c_cflag &= ~(PARENB | PARODD);                       // No parity
    port_settings->c_cflag &= ~CSTOPB;                                  // 2 stop bits, else 1
    port_settings->c_cflag &= ~CRTSCTS;                                 // no hardware flow control
}


bool is_device_connected(const char *port_number) {
    printf("Searching for devices on port %s\n", port_number);

    // O_RDWR - allow read/write (some checks require write)
    // O_NOCTTY - If pathname refers to a terminal device (tty) 
    //     it will not become the process's controlling terminal even if the process does not have one.
    // O_NONBLOCK - all read/write commands won't block if device is not responding
    int fd = open(port_number, O_RDWR | O_NOCTTY | O_NONBLOCK);

    if (fd < 0) {
        printf("Cannot connect to port %s: %s\n", port_number, strerror(errno));
        return false;
    }

    // Creating settings structure
    struct termios port_settings;
    // Putting state of fd into port_settings
    if (tcgetattr(fd, &port_settings) != 0) {
        printf("Error from tcgetattr for port %s: %s\n", port_number, strerror(errno));
        close(fd);
        return false;
    }
    // Make required settings
    setup_port(&port_settings, B9600);

    if (tcsetattr(fd, TCSANOW, &port_settings) != 0) {
        printf("Cannot set TCSANOW for port %s: %s\n", port_number, strerror(errno));
        close(fd);
        return false;
    }



    close(fd);
    return false;
}



