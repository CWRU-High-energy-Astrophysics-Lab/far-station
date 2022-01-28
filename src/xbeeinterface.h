//
// Created by robin on 11/24/21.
//

#ifndef FAR_STATION_ROBIN_XBEEINTERFACE_H
#define FAR_STATION_ROBIN_XBEEINTERFACE_H
#include <string>
#endif //FAR_STATION_ROBIN_XBEEINTERFACE_H
using namespace std;
bool restartingpi = false;
//returns next msg needed to be sent
string getmsgToSend();

// add incoming msg to queue to be unpacked and process
void addmsgtoUnpack(string incoming);
bool startXbee();