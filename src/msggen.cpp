//
// Created by Robin James on 5/31/2022.
//
#include <cstdio>
#include <cstring>
#include <random>
#include <chrono>
#include <vector>
#include <fstream>
#include <iostream>

#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()
#include "msg_outline/Generalmsg.h"
#include "msg_outline/augermsg.h"
#include "msg_outline/Cmdmsg.h"
#include "msg_outline/Logmsg.h"
#include "msg_outline/MsgHistory.h"
#include "msg_outline/T2msg.h"
#include "msg_outline/T3msg.h"
#include "packinterface.h"
#include "xbeeinterface.h"




std::string gen_random_str(const int len) {
    const std::string CHARACTERS =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz"
            ".?:!,";
    std::string tmp_s;
    std::random_device random_device;
    std::mt19937 generator(random_device());
    std::uniform_int_distribution<> distribution(0, CHARACTERS.size() - 1);

    tmp_s.reserve(len);

    for (int i = 0; i < len; ++i) {
        tmp_s += CHARACTERS[distribution(generator)];
    }

    return tmp_s;
}


std::string t2_gen() {
    std::string msg;



        auto currentsec = std::to_string(std::chrono::duration_cast<std::chrono::microseconds>
                                                 (std::chrono::high_resolution_clock::now().time_since_epoch()).count()).substr(
                0, 10);
         msg = "Sec,nt2,scaler: " + currentsec + " ## " + "Scaler\n";
        int j = 0;
        while (currentsec == std::to_string(std::chrono::duration_cast<std::chrono::microseconds>
                                                    (std::chrono::high_resolution_clock::now().time_since_epoch()).count()).substr(
                0, 10)) {
            if (rand() % 1000000 < 100) {
                auto timestamp = std::to_string(std::chrono::duration_cast<std::chrono::microseconds>
                                                        (std::chrono::high_resolution_clock::now().time_since_epoch()).count()).substr(
                        11);

                auto add = std::to_string(j) + " 1:" + timestamp + '\n';

                msg += add;

                j++;

            }


        }

        msg.append(1, '\n');






    return msg;
}


int msggenmain() {
    while (!getRestart()){
        std::cout<< "testpoint1"<< std::endl;
        if(gettingt2()){
            std::cout<< "testpoint2"<< std::endl;
            addmsgtoSend(t2_gen());
        }
    }


    return 0;





}


