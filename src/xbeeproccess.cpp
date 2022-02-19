#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <thread>
#include <iostream>
#include <string.h>
#include <fstream>
#include "xbeeinterface.h"




std::ofstream myfile;
std::ifstream rfile;
char byte[40]; //40 is an arbitray number which I choosed. I am sending line by line, so 40 would be enough.
char B1[40]; //B1 and B2 should be same size as byte.
char B2[40];
char empty[40]={0,};
char echos[70]; //echos would be the linux command used in system. Size should be larger than byte.
int bm=1; //buffer mode. Either 1 or 2.
int s=1; //Sending mode.
int n=0;
int count=0;
int mi=0; // counting '---'.
char T2all_l[1000][40]; // Number of lines in rT2.txt should not exceed 1000!
char rbyte[40]; //Here I restore what I received from Near!
/*
void combineT2(int n, char *buffer)
{
  strcpy(T2all_l[n], buffer);
}
*/
void changebm()
{
    if(bm==1)
    {
        bm=2;
    }
    else
    {
        bm=1;
    }
}

void send()
{
    char echos[70];
    char tail[6];
    system("stty -F /dev/ttyUSB0 speed 9600 cs8 -cstopb -parenb");
    while(!getRestart())
    {
        if(s==1)// Sending T2
        {
            std::string msg=getmsgToSend();
            strcpy(byte,msg.c_str()); //In my original python script, the shape of T2all_l was quite different. However, I had to change like this. Now T2all_l stands for 1 line.
            if(byte[0]=='-')
            {
                printf("tail\n");
                strcpy(tail,"tail");
                sprintf(echos,"echo %s > /dev/ttyUSB0",tail);
                system(echos); //Tail stands for the signal that one chunck is end.
                //sleep(1); //We need to sleep when we observe '-'. This means it is the end of T2 chunk. We don't need this sleep when we receice data in real time.
            }
            else
            {
                printf("%s\n",byte);
                myfile.open("/dev/ttyUSB0");
                myfile << byte;
                // sprintf(echos,"echo %s > /dev/ttyUSB0",byte); // When I send, I do not need to add \n. It is atomatically added when I receive each time.
                //system(echos); I do not know why this is not working.
                myfile.close();
            }
        }
        else //s is not 1, Sending SLEEPING
        {
            strcpy(byte,T2all_l[n]);
            if(byte[0]=='-')
            {
                printf("SLEEPING\n");
                strcpy(byte,"SLEEPING\n");
                /*
              sprintf(echos,"echo %s > /dev/ttyUSB0",byte);
              system(echos);
                */
                myfile.open("/dev/ttyUSB0");
                myfile << byte;
                myfile.close();
                //sleep(1); // We also do not need this.
            }
        }
        n=n+1; // n is keep going up regardless of send is sending sleeping or T2
        if(bm==1)
        {
            strcpy(B1,byte);
            strcpy(byte,empty);//We should have null string.
        }
        else
        {
            strcpy(B2,byte);
            strcpy(byte,empty);
        }
        changebm();
    }
}

void readfromNear()
{
    char rbyte[100];
    //char firstchar='a'; //Test for printing.
    //string readline;
    FILE* rfp = popen("cat /dev/ttyUSB0","r");
    //rfile.open("/dev/ttyUSB");
    //rfile.getline(rbyte,100);
    while(!getRestart())
    {
        //rfile.open("/dev/ttyUSB");
        if(sizeof(rfp)>1)//This is necessary because we are only using reader when there is something to read.
        {
            fgets(rbyte, sizeof(rbyte), rfp);
            //printf("%s : %d\n",rbyte, sizeof(rbyte));
            printf("Size of rfp : %d\n",sizeof(rfp));
            printf("From NEAR : %s\n",rbyte);
            if(rbyte[0]=='S')
            {
                s=0;
            }
            else if(rbyte[0]=='T')
            {
                s=1;
            }
            else //Initalize the buffer if I get the proper signal.
            {
                if(bm==2)
                {
                    strcpy(B1,empty);
                }
                else
                {
                    strcpy(B2,empty);
                }
            }
            /*
            if(pn==1)
              {
                printf("From NEAR : %s\n",rbyte);
              }
            */
        }
    }
    //pclose(rfp);
}

bool startXbee()
{
    printf("Let's Start!\n");
    /*
    FILE* fp = fopen("rT2.txt","r");
    char buffer[1000]={0,}; //1000 is also another arbitray number.
    while(mi<30) //Here I assume rT2.txt contain 30 chuncks. This number could be changed in future.
      {
        fgets(buffer,sizeof(buffer),fp);
        combineT2(count,buffer);
        count++;
        if(buffer[0]=='-')
      mi++;
      }
    fclose(fp);
    */
    std:: thread _readfromNear(readfromNear);
    std::thread _send(send);
    _readfromNear.join();
    _send.join();
    return 0;
}
