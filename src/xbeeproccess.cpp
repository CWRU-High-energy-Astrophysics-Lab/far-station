//
// Created by robin on 1/28/22.
//
#include "xbeeinterface.h"

#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()
#include <cstring>
struct termios tty;
const char *port="/dev/Xbee";

int xbee_port;
bool startXbee(){

    //ste up interface

    xbee_port = open(port, O_RDWR);

// Check for errors
    if (xbee_port < 0) {
        printf("Error %i from open: %s\n", errno, strerror(errno));
        return false;
    }

    if(tcgetattr(xbee_port, &tty) != 0) {
        printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
        return false;
    }

    cfsetospeed(&tty, B9600);

    if (tcsetattr(xbee_port, TCSANOW, &tty) != 0) {
        printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
    }


    return true;
}


bool xbeeLoop(){
    while(getRestart()){
        // kyle code here
        /* write command: write(xbee_port, msg, sizeof(msg))
         * read command:
         * char read_buf [256]
         * int n = read(serial_port, &read_buf, sizeof(read_buf)); */






    }
    return false;
}
