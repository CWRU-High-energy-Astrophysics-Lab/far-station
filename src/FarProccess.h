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


#ifndef FAR_STATION_ROBIN_EAD_H
#define FAR_STATION_ROBIN_EAD_H

#endif //FAR_STATION_ROBIN_EAD_H

using namespace std;
priority_queue<Generalmsg> msgToProccess;
priority_queue<Generalmsg> msgToPack;
priority_queue<Generalmsg> msgToSend;
priority_queue<Generalmsg> msgToUnPack
;
bool msgtoProccessEmpty();
string encrypt(Generalmsg msg) ;
Generalmsg decrypt(string msg);
bool send_t3();

bool fpt();
class FarProccess{
public:

    char *const EKITPORT = getenv("EKITPORT"); //enviormental varible

    int USB;


int start();

//comands to cdas
string startCDAS();
string startDataCollection();
string rebootStation();
string rebootBrodcast();
string t3Random();
string t3Time(basic_string<char> time);
string os9cmd(string cmd);
string stopCDAS();



Generalmsg getmsgToProccess();
void addmsgtoPack(Generalmsg outgoing);



//piCommand
int bashCmd(string cmd);



string getT2();

string getT3();




};
