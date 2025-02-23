#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <errno.h>


int main(int argc, char *argv[]) {
    
    if (argc != 3) {
        printf("Usage: %s /dev/ttyX [dcd|dsr|cts|ri|all|none]\n", argv[0]);
        return 1;
    }

    int fd = open(argv[1], O_RDWR | O_NOCTTY);
    if (fd < 0) {
        printf("Cannot connect to port %s: %s\n", argv[1], strerror(errno));
        return 1;
    }

    int flags;
    
    if (ioctl(fd, TIOCMGET, &flags) == 0) { // Getting curent flags

        printf("Flags before: %b\n", flags);

        if (strcmp(argv[2], "dcd") == 0) flags |= TIOCM_CD;  // DCD (Data Carrier Detect)
        else if (strcmp(argv[2], "dsr") == 0) flags |= TIOCM_DSR; // DSR (Data Set Ready)
        else if (strcmp(argv[2], "cts") == 0) flags |= TIOCM_CTS; // Clear To Send
        else if (strcmp(argv[2], "ri")  == 0) flags |= TIOCM_RI;  // Ring Indicator
        else if (strcmp(argv[2], "all") == 0) flags |= (TIOCM_CD | TIOCM_DSR | TIOCM_CTS | TIOCM_RI);
        else if (strcmp(argv[2], "none") == 0) flags &= ~(TIOCM_CD | TIOCM_DSR | TIOCM_CTS | TIOCM_RI);
        else {
            printf("Unkhown line %s\n", argv[2]);
            return 0;
        }
        
        printf("Flags after: %b\n", flags);

        ioctl(fd, TIOCMSET, &flags); // Setting new flags
    }
    else {
        printf("Cannot get flags: %s\n", strerror(errno));
    }
    close(fd);
    
    return 0;
}