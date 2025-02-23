#include "include/device_detector.h"


int main(int arg_count, char *argv[]) {
    if (arg_count > 1) {
        // using char * to avoid convertions in other functions 
        // (termios accepts const char * as port)
        const char *port_number = argv[1];
        bool device_connected = is_device_connected(port_number);
        if (device_connected) {
            printf("Device is found on port %s\n", port_number);
        }
        else {
            printf("Util was not able to detect any device on port %s\n", port_number);
        }
    }
    else {
        printf("Port number not supplied\n");
    }
    return 0;
}
