//
// Created by robin on 10/21/21.
//

#ifndef FAR_STATION_ROBIN_MSGHISTORY_H
#define FAR_STATION_ROBIN_MSGHISTORY_H
#include "Generalmsg.h"

class MsgHistory: public Generalmsg{
public:
    MsgHistory();
    explicit MsgHistory(string payload);
    void addMSG(const string& id, int size );




private:
    string msglist; // format: id:size \n id:size \n


};


#endif //FAR_STATION_ROBIN_MSGHISTORY_H
