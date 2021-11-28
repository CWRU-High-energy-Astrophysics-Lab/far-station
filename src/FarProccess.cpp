//
// Created by robin on 10/21/21.
//
#include "t2li.h"
#include "FarProccess.h"
#include <mutex>
#include <thread>
#include <iostream>
#include <fstream>
#include <filesystem>
#include "processmsg.h"

mutex mu;

bool init() { //set baud rates and check file system layout
    msgToProccess = priority_queue<Generalmsg>();
    msgToPack = priority_queue<Generalmsg>();
    msgToUnPack = priority_queue<Generalmsg>();
    msgToSend = priority_queue<Generalmsg>();

    try{
        filesystem::current_path("/home/pi");
        filesystem::current_path("/home/pi/data");
        filesystem::current_path("/home/pi/data/t2");
        filesystem::current_path("/home/pi/data/t3");
        filesystem::current_path("/home/pi/data/log");

        filesystem::current_path("/home/pi/rsato_su_emu");
        filesystem::current_path("/home/pi/rsato_su_emu/bin");
        filesystem::current_path("/home/pi/rsato_su_emu/src");
    }
    catch (std::filesystem::filesystem_error const& ex) {
        cout<< "File system not as expected, please check\n";
        return false;
    }

    return 1;
}

int main() {//this is called on pi boot
    if (init()) {

        thread processThread(fpt);

        //Xbee thread

        processThread.join();



//start theads



    }
    return 1;
}

bool fpt() {
    FarProccess node = FarProccess();

    return node.start();


}

int FarProccess::start() {
    bool restartingpi = false;
    while (!restartingpi) {
        if (!msgtoProccessEmpty()) {
            Generalmsg msg = getmsgToProccess();
            string type = msg.gedID();
            if (type == "T3LI") {
                T3msg *msg = dynamic_cast<T3msg *>(msg);

                t3Time(msg->getPayload()); // calls cdas to send t3 for time


                //execute t3 collection
                send_t3();

            } else if (type.substr(0, 2) == "CMD") {
                Cmdmsg *msg = dynamic_cast<Cmdmsg *>(msg);
                if (msg->gedID() == "CMDB") {// is os9 command
                    os9cmd(msg->getPayload());
                } else if (msg->gedID() == "CMDR") {
                    restartingpi = true;
                } else {
                    bashCmd(msg->getPayload());
                }
            } else if (type == "HIST") {
                MsgHistory *msg = dynamic_cast<MsgHistory *>(msg);
                //history request, send history
            } else if (type == "T2LI") {
                T2msg *msg = dynamic_cast<T2msg *>(msg);
                //should not be here, report to log
            } else if (type == "LOGA") {
                Logmsg *msg = dynamic_cast<Logmsg *>(msg);
                // log request, send log file.
            } else {
                Generalmsg *msg = msg;
                //add a report to log
            }
        }
        // get t2 file
        bashCmd("cp T2_list.out T2_toSend.out");
        bashCmd("rm T2_list.out");

        string line;
        ifstream t2File("T2_toSend.out");
        string payload;
        if (t2File.is_open()) {
            while (getline(t2File, line)) {
                if (line.starts_with("---")) {
                    T2msg msg(payload);
                    addmsgtoPack(msg);
                    string payload;
                } else {
                    payload.append(line);
                }
            }
            t2File.close();
        }


    }
    return restartingpi;
}

bool send_t3(){
    int size =0;
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

    Generalmsg msg = decrypt(incoming);
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
void FarProccess::addmsgtoPack(Generalmsg outgoing) {
    mu.lock();
    msgToPack.push(outgoing);
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
void addmsgtoSend(Generalmsg outgoing) {
    mu.lock();
    msgToSend.push(outgoing);
    mu.unlock();

};

string getmsgToSend() {
    mu.lock();
    string pack = encrypt(msgToSend.top());
    msgToSend.pop();
    mu.unlock();
    return pack;

}

//Functions to msgToUnpack
void addmsgtoUnpack(Generalmsg incoming) {
    mu.lock();
    msgToUnPack.push(incoming);
    mu.unlock();

};

string getmsgToUnpack() {
    mu.lock();
    string pack = encrypt(msgToUnPack.top());
    msgToUnPack.pop();
    mu.unlock();
    return pack;

}


//comands to cdas
string FarProccess::startCDAS() {
    string cmd = "./cdas_su_emu " + std::string(EKITPORT) + " " + std::string(EKITPORT) + " 3000";
    bashCmd(cmd.c_str());
    return (cmd.c_str());
}

string FarProccess::startDataCollection() {
    string cmd = "./cl s " + string(EKITPORT);
    bashCmd(cmd.c_str());
    return (cmd);
}

string FarProccess::rebootStation() {
    string cmd = "./cl r " + string(EKITPORT);
    bashCmd(cmd.c_str());
    return (cmd);
}

string FarProccess::rebootBrodcast() {
    string cmd = "./cl R " + string(EKITPORT);
    bashCmd(cmd.c_str());
    return (cmd);
}

string FarProccess::t3Random() {
    string cmd = "./cl t " + string(EKITPORT);
    bashCmd(cmd.c_str());
    return (cmd);
}

string FarProccess::t3Time(basic_string<char> time) {
    string cmd = "./cl T " + time + " " + string(EKITPORT);
    bashCmd(cmd.c_str());
    return (cmd);
}

string FarProccess::os9cmd(string input) {
    string cmd = "./cl c " + input + " " + string(EKITPORT);
    bashCmd(cmd.c_str());
    return (cmd);
}

string FarProccess::stopCDAS() {
    string cmd = "./cl S " + string(EKITPORT);
    bashCmd(cmd.c_str());
    return (cmd);
}


//piCommand
int FarProccess::bashCmd(string cmd) {
    return system(cmd.c_str());
}


string encrypt(Generalmsg generalmsg) {
    return string(generalmsg.gedID() + "[" + generalmsg.getRev() + "]:" + to_string(generalmsg.getSize()) +
                  generalmsg.getPayload());
}

Generalmsg decrypt(string input) {
    Generalmsg msg;
    string type = input.substr(0, 3);
    int headerend = input.find(':');
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
