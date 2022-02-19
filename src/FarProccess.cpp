//
// Created by robin on 10/21/21.
//
#include "t2li.h"
#include "FarProccess.hpp"
#include <mutex>
#include <thread>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <utility>
#include "packinterface.h"
#include "xbeeinterface.h"



mutex mu;
bool restartingpi = false;
string user;

bool getRestart(){
    return restartingpi;
}
bool init() { //set baud rates and check file system layout
    msgToProccess = priority_queue<Generalmsg>();
    msgToPack = priority_queue<Generalmsg>();
    msgToUnPack = priority_queue<string>();
    msgToSend = priority_queue<string>();
     user= "/home/"+ string(getenv("USER"));
    try{
        filesystem::current_path(user);
        filesystem::current_path(user+"/data");
        filesystem::current_path(user+"/data/t2");
        filesystem::current_path(user+"/data/t3");
        filesystem::current_path(user+"/data/log");

        filesystem::current_path(user+"/rsato_su_emu");
        filesystem::current_path(user+"/rsato_su_emu/bin");
        filesystem::current_path(user+"/rsato_su_emu/src");
        system("make");

        filesystem::current_path(user);
    }
    catch (std::filesystem::filesystem_error const& ex) {
        cout<< "File system not as expected, please check\n";
        return false;
    }

    return true;
}

int main() {//this is called on pi boot
    if (init()) {

        thread processThread(fpt);
        thread cdasThread(cdas_su_emu_main);
        //Xbee thread
        thread xbeeThread(startXbee);

        xbeeThread.join();
        processThread.join();






    }
    return 1;
}

bool fpt() {
    FarProccess node = FarProccess();

    return node.start();


}

int FarProccess::start() {

    while (!restartingpi) {
        if (!msgtoProccessEmpty()) {
            Generalmsg msg = getmsgToProccess();
            string type = msg.gedID();
            if (type == "T3LI") {
                T3msg *msg = dynamic_cast<T3msg *>(msg);

                t3Time(msg->getPayload()); // calls cdas to send t3 for time


                //execute t3 collection
                send_t3();
                delete msg;
            } else if (type.substr(0, 2) == "CMD") {
                Cmdmsg *msg = dynamic_cast<Cmdmsg *>(msg);
                if (msg->gedID() == "CMDB") {// is os9 command
                    os9cmd(msg->getPayload());
                } else if (msg->gedID() == "CMDR") {
                    restartingpi = true;
                } else {
                    bashCmd(msg->getPayload());
                }
                delete msg;
            } else if (type == "HIST") {
                MsgHistory *msg = dynamic_cast<MsgHistory *>(msg);
                //history request, send history
                delete msg;
            } else if (type == "T2LI") {
                T2msg *msg = dynamic_cast<T2msg *>(msg);
                //should not be here, report to log
                delete msg;
            } else if (type == "LOGA") {
                Logmsg *msg = dynamic_cast<Logmsg *>(msg);
                // log request, send log file.
                delete msg;
            } else {
                Generalmsg *msg = msg;
                //add a report to log

                delete msg;
            }

        }
        // get t2 file
        addmsgtoPack(getT2Fromcdas());
        filesystem::current_path(user);


    }
    return restartingpi;
}

bool send_t3(){
    int size =0;

    //read and put into general msg
    return true;
}

bool msgtoProccessEmpty() {
    mu.lock();
    bool temp = msgToProccess.empty();
    mu.unlock();
    return temp;
}

//function to msgToProccess
void addmsgtoProccess(string incoming) {

    Generalmsg msg = decrypt(std::move(incoming));
    mu.lock();
    msgToProccess.push(msg);
    mu.unlock();
}

Generalmsg FarProccess::getmsgToProccess() {
    Generalmsg msg;
    mu.lock();
    msg = msgToProccess.top();
    msgToProccess.pop();
    mu.unlock();
    return msg;
}

// functions to msgToPack

void FarProccess::addmsgtoPack(const char msg[]) {
    mu.lock();
    msgToPack.push(T2msg(msg));
    mu.unlock();

};

string getmsgToPack() {
    mu.lock();
    string pack = encrypt(msgToPack.top());
    msgToPack.pop();
    mu.unlock();
    return pack;

}

//function to msgToSend
void addmsgtoSend(string outgoing) {
    mu.lock();
    msgToSend.push(outgoing);
    mu.unlock();

};

string getmsgToSend() {
    mu.lock();
    string pack = msgToSend.top();
    msgToSend.pop();
    mu.unlock();
    return pack;

}

//Functions to msgToUnpack
void addmsgtoUnpack(string incoming) {
    mu.lock();

    msgToUnPack.push(incoming);
    mu.unlock();

};

string getmsgToUnpack() {
    mu.lock();
    string pack =msgToUnPack.top();
    msgToUnPack.pop();
    mu.unlock();
    return pack;

}


//comands to cdas
string FarProccess::startCDAS() const {
    string cmd = "./cdas_su_emu " + std::string(EKITPORT) + " " + std::string(EKITPORT) + " 3000";
    bashCmd(cmd);
    return (cmd);
}

string FarProccess::startDataCollection() const {
    string cmd = "./cl s " + string(EKITPORT);
    bashCmd(cmd);
    return (cmd);
}

string FarProccess::rebootStation() const {
    string cmd = "./cl r " + string(EKITPORT);
    bashCmd(cmd);
    return (cmd);
}

string FarProccess::rebootBrodcast() const {
    string cmd = "./cl R " + string(EKITPORT);
    bashCmd(cmd);
    return (cmd);
}

string FarProccess::t3Random() const {
    string cmd = "./cl t " + string(EKITPORT);
    bashCmd(cmd);
    return (cmd);
}

string FarProccess::t3Time(const basic_string<char>& time) const {
    string cmd = "./cl T " + time + " " + string(EKITPORT);
    bashCmd(cmd);
    return (cmd);
}

string FarProccess::os9cmd(const string& input) const {
    string cmd = "./cl c " + input + " " + string(EKITPORT);
    bashCmd(cmd);
    return (cmd);
}

string FarProccess::stopCDAS() const {
    string cmd = "./cl S " + string(EKITPORT);
    bashCmd(cmd);
    return (cmd);
}


//piCommand
int FarProccess::bashCmd(const string& cmd) {
    return system(cmd.c_str());
}


string encrypt(const Generalmsg& generalmsg) {
    return string(generalmsg.gedID() + "[" + generalmsg.getRev() + "]:" + to_string(generalmsg.getSize()) +
                  generalmsg.getPayload());
}

Generalmsg decrypt(const string& input) {
    Generalmsg msg;
    string type = input.substr(0, 3);
    unsigned long headerend = input.find(':');
    string payload = input.substr(headerend);
    if (type == "T3LI") {
        msg = T3msg(payload);
    } else if (type.substr(0, 2) == "CMD") {
        msg = Cmdmsg(payload, type.at(3));
    } else if (type == "HIST") {
        msg = MsgHistory(payload);
    } else if (type == "T2LI") {
        msg = T2msg(payload);
    } else if (type == "LOGA") {
        msg = Logmsg(payload);
    } else {
        msg = Generalmsg(type, "REV0", payload, 12);
        //add a report to log
    }


    return msg;
}
