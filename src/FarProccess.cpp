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




std::mutex mu;
std::mutex mu2;
std::mutex mu3;
std::mutex mu4;
std::mutex mu5;
bool restartingpi = false;
bool t2ing = true;
std::string user;

bool getRestart(){
    return restartingpi;
}
bool gettingt2(){
    return t2ing;
}
bool init() { //set baud rates and check file system layout
    msgToProccess = std::priority_queue<Generalmsg>();
    msgToPack = std::priority_queue<Generalmsg>();
    msgToUnPack = std::priority_queue<std::string>();
    msgToSend = std::priority_queue<std::string>();
     user= "/home/"+ std::string(getenv("USER"));
    try{
        std::filesystem::current_path(user);
        std::filesystem::current_path(user+"/data");
        std::filesystem::current_path(user+"/data/t2");
        std::filesystem::current_path(user+"/data/t3");
        std::filesystem::current_path(user+"/data/log");

        /*std::filesystem::current_path(user+"/rsato_su_emu");
        std::filesystem::current_path(user+"/rsato_su_emu/bin");
        std::filesystem::current_path(user+"/rsato_su_emu/src");
        system("make");*/

        std::filesystem::current_path(user);
    }
    catch (std::filesystem::filesystem_error const& ex) {
        std::cout<< "File system not as expected, please check\n";
        return false;
    }

    return true;
}

int main() {//this is called on pi boot
    if (init()) {

        std::thread processThread(fpt);
        std::thread cdasThread(msggenmain);
        //Xbee thread
        std::thread xbeeThread(xbeeLoop);

        xbeeThread.join();
        processThread.join();
        cdasThread.join();






    }
    return 1;
}

bool fpt() {


    return farProccess::start();


}

int farProccess::start() {

    while (!restartingpi) {

        std::string msg = getmsgToUnpack();




        if (!msg.empty()) {

            addmsgtoProccess(msg);


        }
        if (!msgtoProccessEmpty()) {
            Generalmsg msg = getmsgToProccess();
            std::string type = msg.getID();

            if (type == "T3LI") {
                /*T3msg *msg = dynamic_cast<T3msg *>(msg);

                t3Time(msg->getPayload()); // calls cdas to send t3 for time


                //execute t3 collection
                send_t3();
                delete msg;*/
                std::cout<< type<< std::endl;
                std::ofstream myfile;
                myfile.open("t3.txt", std::ios::app);
                myfile << encrypt(msg)<<'\n' ;
                myfile.flush();
                myfile.close();
            } else if (type.substr(0, 2) == "CMD") {
                Cmdmsg *msg = dynamic_cast<Cmdmsg *>(msg);
                if (msg->getID() == "CMDB") {// is os9 command
                    os9cmd(msg->getPayload());
                } else if (msg->getID() == "CMDR") {
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
        auto out = getmsgToPack();
        if(!out.empty()){

            addmsgtoSend(out);}
        std::filesystem::current_path(user);


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
void addmsgtoProccess(std::string incoming) {

    Generalmsg msg = decrypt(incoming);

    mu.lock();
    msgToProccess.push(msg);
    mu.unlock();
}

Generalmsg farProccess::getmsgToProccess() {
    Generalmsg msg;
    mu.lock();

    if (!msgToProccess.empty()) {
        msg = msgToProccess.top();
        msgToProccess.pop();
    }

    mu.unlock();
    return msg;
}

// functions to msgToPack
void addmsgtoPack(Generalmsg outgoing) {
    mu2.lock();


    msgToPack.push(outgoing);
    mu2.unlock();

};

std::string getmsgToPack() {
    mu2.lock();

    std::string pack;
    if(!msgToPack.empty()){

        pack = encrypt(msgToPack.top());
        msgToPack.pop();
    }

    mu2.unlock();
    return pack;

}

//function to msgToSend
void addmsgtoSend(std::string outgoing) {
    mu3.lock();
    //std::cout<< msgToSend.size()<<std::endl;
    msgToSend.push(outgoing);
    mu3.unlock();

};

std::string getmsgToSend() {
    mu3.lock();
    std::string pack;
    if (!msgToSend.empty()) {

        pack = msgToSend.top();
        msgToSend.pop();
    }
    mu3.unlock();

    return pack;

}

//Functions to msgToUnpack
void addmsgtoUnpack(std::string incoming) {

    mu4.lock();

    msgToUnPack.push(incoming);
    mu4.unlock();

};

std::string getmsgToUnpack() {
    mu4.lock();
    std::string pack;
    if (!msgToUnPack.empty()) {

        pack = msgToUnPack.top();
        msgToUnPack.pop();
    }
    mu4.unlock();


    return pack;


}


//comands to cdas
std::string farProccess::startCDAS()  {
    //std::string cmd = "./cdas_su_emu " + std::string(EKITPORT) + " " + std::string(EKITPORT) + " 3000";
    std::string cmd = "ls";
    bashCmd(cmd);
    return (cmd);
}

std::string farProccess::startDataCollection()  {
    std::string cmd = "./cl s " + std::string(EKITPORT);
    bashCmd(cmd);
    return (cmd);
}

std::string farProccess::rebootStation()  {
    std::string cmd = "./cl r " + std::string(EKITPORT);
    bashCmd(cmd);
    return (cmd);
}

std::string farProccess::rebootBrodcast()  {
    std::string cmd = "./cl R " + std::string(EKITPORT);
    bashCmd(cmd);
    return (cmd);
}

std::string farProccess::t3Random() {
    std::string cmd = "./cl t " + std::string(EKITPORT);
    bashCmd(cmd);
    return (cmd);
}

std::string farProccess::t3Time(const std::basic_string<char>& time)  {
    std::string cmd = "./cl T " + time + " " + std::string(EKITPORT);
    bashCmd(cmd);
    return (cmd);
}

std::string farProccess::os9cmd(const std::string& input)  {
    std::string cmd = "./cl c " + input + " " + std::string(EKITPORT);
    bashCmd(cmd);
    return (cmd);
}

std::string farProccess::stopCDAS()  {
    std::string cmd = "./cl S " + std::string(EKITPORT);
    bashCmd(cmd);
    return (cmd);
}


//piCommand
int farProccess::bashCmd(const std::string& cmd) {
    return system(cmd.c_str());
}




std::string encrypt(const Generalmsg& generalmsg) {
    return std::string(generalmsg.getID() + "[" + generalmsg.getRev() + "]:" + std::to_string(generalmsg.getSize()) +
                  generalmsg.getPayload()+'\0');
}

Generalmsg decrypt(const std::string& input) {
    Generalmsg msg;

    std::string type = input.substr(0, 4);
    unsigned long headerend = input.find(':');
    try {
        std::string payload = input.substr(headerend + 1);
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


        }
    }
    catch (const std::out_of_range) {
        msg = Generalmsg("ERRO", "REV0", input, 12);
    }
    return msg;
}
