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
string encrypt(const Generalmsg& msg) ;
Generalmsg decrypt(const string& msg);
bool send_t3();

bool fpt();
class FarProccess{
public:

    char *const EKITPORT = getenv("EKITPORT"); //enviormental varible

    int USB{};


int start();

//comands to cdas
[[nodiscard]] string startCDAS() const;
[[nodiscard]] string startDataCollection() const;
[[nodiscard]] string rebootStation() const;
[[nodiscard]] string rebootBrodcast() const;
[[nodiscard]] string t3Random() const;
[[nodiscard]] string t3Time(const basic_string<char>& time) const;
[[nodiscard]] string os9cmd(const string& cmd) const;
[[nodiscard]] string stopCDAS() const;



static Generalmsg getmsgToProccess();
static void addmsgtoPack(const Generalmsg& outgoing);



//piCommand
static int bashCmd(const string& cmd);



string getT2();

string getT3();




};
