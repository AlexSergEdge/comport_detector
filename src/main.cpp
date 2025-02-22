#include "include/device_detector.h"

int main(int arg_count, char *args[]) {
    
    if (arg_count > 1) {
        // std::string port_number(args[1]);
        const char *port_number = args[1];
        is_device_connected(port_number);
    }
    else {
        std::cout << "Port number not supplied\n";
    }
    return 0;
}
