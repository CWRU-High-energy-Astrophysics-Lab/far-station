//
// Created by robin on 10/21/21.
//

#include "MsgHistory.h"

#include <utility>

MsgHistory::MsgHistory() : Generalmsg("HIST", "Rev1", "",7) {

}
MsgHistory::MsgHistory(string payload) : Generalmsg("HIST", "Rev1", std::move(payload),7) {

}
void MsgHistory::addMSG(const string& id, int size) {
    string temp = '\n'+id +':'+to_string(size);
    msglist.append(temp);
}





