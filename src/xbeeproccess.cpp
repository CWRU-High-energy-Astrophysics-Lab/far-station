#include <cstdio>
#include <cstdlib>


#include <iostream>
#include <cstring>
#include <fstream>
#include "xbeeinterface.h"




char byte[40]; //40 is an arbitray number which I choosed. I am sending line by line, so 40 would be enough.
char B1[40]; //B1 and B2 should be same size as byte.
char B2[40];
char empty[40] = {0,};

int bm = 1; //buffer mode. Either 1 or 2.
int s = 1; //Sending mode.
int n = 0;

char T2all_l[1000][40]; // Number of lines in rT2.txt should not exceed 1000!

/*
void combineT2(int n, char *buffer)
{
  strcpy(T2all_l[n], buffer);
}
*/
void changebm() {
    if (bm == 1) {
        bm = 2;
    } else {
        bm = 1;
    }
}

void send() {
    char echos[70];
    char tail[6];
    std::ofstream myfile;

    if (s == 1)// Sending T2
    {
        std::string msg = getmsgToSend();
        strcpy(byte,
               msg.c_str()); //In my original python script, the shape of T2all_l was quite different. However, I had to change like this. Now T2all_l stands for 1 line.
        if (byte[0] == '-') {
            printf("tail\n");
            strcpy(tail, "tail");
            sprintf(echos, "echo %s >  /dev/XBEE", tail);
            system(echos); //Tail stands for the signal that one chunck is end.
            //sleep(1); //We need to sleep when we observe '-'. This means it is the end of T2 chunk. We don't need this sleep when we receice data in real time.
        } else {
            printf("%s\n", byte);
            myfile.open(" /dev/XBEE");
            myfile << byte;
            // sprintf(echos,"echo %s > /dev/ttyUSB0",byte); // When I send, I do not need to add \n. It is atomatically added when I receive each time.
            //system(echos); I do not know why this is not working.
            myfile.close();
        }
    } else //s is not 1, Sending SLEEPING
    {
        strcpy(byte, T2all_l[n]);
        if (byte[0] == '-') {
            printf("SLEEPING\n");
            strcpy(byte, "SLEEPING\n");

            myfile.open(" /dev/XBEE");
            myfile << byte;
            myfile.close();

        }
    }
    n = n + 1; // n is keep going up regardless of send is sending sleeping or T2
    if (bm == 1) {
        strcpy(B1, byte);
        strcpy(byte, empty);//We should have null string.
    } else {
        strcpy(B2, byte);
        strcpy(byte, empty);
    }
    changebm();
}


void readfromNear() {
    char rbyte[100];


    FILE *rfp = popen("cat /dev/XBEE", "r");

    fgets(rbyte, sizeof(rbyte), rfp);//printf("%s : %d\n",rbyte, sizeof(rbyte));
    printf("Size of rfp : %ld\n", sizeof(rfp));
    printf("From NEAR : %s\n", rbyte);
    if (rbyte[0] == 'S') {
        s = 0;
    } else if (rbyte[0] == 'T') {
        s = 1;
    } else //Initalize the buffer if I get the proper signal.
    {
        if (bm == 2) {
            strcpy(B1, empty);
        } else {
            strcpy(B2, empty);
        }
    }
}

bool startXbee() {
    printf("Let's Start!\n");
    system("stty -F /dev/ttyUSB0 speed 9600 cs8 -cstopb -parenb");
    while (!getRestart()) {
        readfromNear();
        send();
    }

    return false;
}
