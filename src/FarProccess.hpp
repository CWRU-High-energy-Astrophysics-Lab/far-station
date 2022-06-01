//
// Created by robin on 10/22/21.
//

#include <iostream>
#include <string>
#include <cstdlib>
#include <queue>

#include "msg_outline/Generalmsg.h"
#include "msg_outline/T3msg.h"
#include "msg_outline/T2msg.h"
#include "msg_outline/Cmdmsg.h"
#include "msg_outline/MsgHistory.h"
#include "msg_outline/Logmsg.h"
/*
extern "C" {
#include "../rsato_su_emu/src/cdas_calls.h"
}*/


#ifndef FAR_STATION_ROBIN_EAD_H
#define FAR_STATION_ROBIN_EAD_H

#endif //FAR_STATION_ROBIN_EAD_H


std::priority_queue<Generalmsg> msgToProccess;
std::priority_queue<Generalmsg> msgToPack;
std::priority_queue<std::string> msgToSend;
std::priority_queue<std::string> msgToUnPack;

bool msgtoProccessEmpty();

std::string encrypt(const Generalmsg &msg);

Generalmsg decrypt(const std::string &msg);

bool send_t3();

bool fpt();


namespace farProccess {

    char *const EKITPORT = getenv("EKITPORT"); //enviormental varible

    int USB{};


    int start();

//comands to cdas
    std::string startCDAS();

     std::string startDataCollection() ;

    std::string rebootStation() ;

   std::string rebootBrodcast() ;

     std::string t3Random() ;

    std::string t3Time(const std::basic_string<char> &time) ;

    std::string os9cmd(const std::string &cmd) ;

     std::string stopCDAS();



    static Generalmsg getmsgToProccess();


//piCommand
    static int bashCmd(const std::string &cmd);


    std::string getT2();

    std::string getT3();


};
