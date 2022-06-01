//
// Created by robin on 1/28/22.
//
#include "xbeeinterface.h"


#include <cstring>


int xbee_port;

#include <stdio.h>
#include <string.h>

// Linux headers
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()

#include <thread>
#include <iostream>

const char *port = "/dev/Xbee";
using std::thread;

int serial_port;
struct termios tty;
int s = 1; //sending mode. s==0 when sleeping mode.

int n = 0;

int bm = 1; //buffer mode. Either 1 or 2.
int tsize = 0;


bool setup() {
    if(tcgetattr(serial_port, &tty) != 0)
    {
        printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
        //return 1;
    }

    tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
    tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
    tty.c_cflag &= ~CSIZE; // Clear all bits that set the data size
    tty.c_cflag |= CS8; // 8 bits per byte (most common)
    tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
    tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

    tty.c_lflag &= ~ICANON;
    tty.c_lflag &= ~ECHO; // Disable echo
    tty.c_lflag &= ~ECHOE; // Disable erasure
    tty.c_lflag &= ~ECHONL; // Disable new-line echo
    tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
    tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes

    tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
    tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
    // tty.c_oflag &= ~OXTABS; // Prevent conversion of tabs to spaces (NOT PRESENT ON LINUX)
    // tty.c_oflag &= ~ONOEOT; // Prevent removal of C-d chars (0x004) in output (NOT PRESENT ON LINUX)

    tty.c_cc[VTIME] = 0;    // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
    tty.c_cc[VMIN] = 1;

    // Set in/out baud rate to be 9600
    cfsetispeed(&tty, B9600);
    cfsetospeed(&tty, B9600);

    // Save tty settings, also checking for error
    if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
        printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
        return false;
    }
    return true;
}
void send()
{
    std::string msg=getmsgToSend();
    std::cout<<"testsend"<<std::endl;

    write(serial_port, static_cast< const void*>(msg.c_str()), msg.size() + 1);

}



std::string read()
{
    // The following variables could be made parameters if needed
    // expected message size

    constexpr int k_initial_buf_len = 256;
    // max message size (throws error if exceeded)
    constexpr int k_max_msg_len = 512;

    std::string res;
    res.reserve(k_initial_buf_len);

    int total_bytes = 0;
    char current;

    // reads one byte from the serial eport at a time
    // exits once a newline is reached or there are no more bytes to read in the buffer.
    while(1) {
        if(read(serial_port, &current, 1)) {

            // done if we see a newline or a null termination
            if( current == '\0') {

                break;
            } else if(total_bytes >= k_max_msg_len) {

                // throw an error here bc message too long
                std::cerr << "Message too long!" << std::endl;
                return "";
            } else {

                res.append(1,current);

                ++total_bytes;
            }
        } else {
            break;
        }
    }

    // shrink the string down in order to conserve memory
    res.shrink_to_fit();

    return res;
}




bool xbeeLoop() {

    serial_port = open(port, O_RDWR);
    if (!setup()) {

        return false; }
    while(!getRestart()){

        auto msg = read();

        addmsgtoUnpack(msg);
        send();

    }
        return true;
}
