#include "include/device_detector.h"
#include <stdio.h>
#include <termios.h>                    // port params 
#include <string.h>
#include <errno.h>
#include <fcntl.h>                      // functions to open COM ports, constants
#include <unistd.h>                     // port reading/writing
#include <sys/ioctl.h>                  // ioctl, constants

using namespace std;


// Base settings
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


// Returns true if device is found on port
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
    // Make required base settings
    setup_port(&port_settings, B9600);

    // TCSANOW - make changes immediately
    if (tcsetattr(fd, TCSANOW, &port_settings) != 0) {
        printf("Cannot set TCSANOW for port %s: %s\n", port_number, strerror(errno));
        close(fd);
        return false;
    }

    bool device_connected = false;

    device_connected = check_signal_lines(fd);
    if (!device_connected) {
        printf("No signal detected on control lines\n");
        device_connected = read_data(fd);
    }
    if (!device_connected) {
        printf("No data can be read from port\n");
        device_connected = check_control_line(fd);
    }
    if (!device_connected) {
        printf("No answer to DTR line up\n");
    }

    close(fd);
    return device_connected;
}


// Check if any of signal lines are up
bool check_signal_lines(int fd) {
    int lines_status;
    // reading signal line
    if (ioctl(fd, TIOCMGET, &lines_status) == 0) {
        printf("Flags: %b\n", lines_status);
        if ((lines_status & TIOCM_CD) ||   // DCD (Data Carrier Detect)
            (lines_status & TIOCM_DSR) ||  // DSR (Data Set Ready)
            (lines_status & TIOCM_CTS) ||  // Clear To Send
            (lines_status & TIOCM_RI)) {   // Ring Indicator
            return true;
        }
    }
    else {
        printf("Cannot get signal info: %s\n", strerror(errno));
    }
    return false;
}


// Just read some data from port
bool read_data(int fd) {
    char buffer[32];
    int bytes = read(fd, buffer, sizeof(buffer));
    if (bytes > 0) return true;
    return false;
}


// Check if setting DTR will change DSR from device side
bool check_control_line(int fd) {
    int lines_status;
    // reading signal line
    if (ioctl(fd, TIOCMGET, &lines_status) == 0) {
        lines_status |= TIOCM_DTR;         // Change DTR (Data Transmission Ready)
        ioctl(fd, TIOCMSET, &lines_status);
        usleep(100000);  // wait for 100ms
        ioctl(fd, TIOCMSET, &lines_status);
        if (lines_status & TIOCM_DSR) return true;  // Check DSR
    }
    return false;
}
