//
// Created by robin on 10/21/21.
//
#include <iostream>
#include <string>
#ifndef FAR_STATION_ROBIN_GENERALMSG_H
#define FAR_STATION_ROBIN_GENERALMSG_H
using namespace std;

class Generalmsg {
public:
    Generalmsg();
    [[nodiscard]] int getweight() const;
    [[nodiscard]] string gedID() const;
    [[nodiscard]] string getRev() const;
    [[nodiscard]] unsigned long getSize() const; // in bytes
    [[nodiscard]] virtual string getPayload() const;



    Generalmsg(string id, string rev , string payload, int weight);
    bool operator==(const Generalmsg &that)const
    { return weight == that.weight; }
    bool operator<(const Generalmsg &that)const
    { return weight < that.weight; }
    bool operator>(const Generalmsg &that)const
    { return weight > that.weight; }


    bool operator!=(const Generalmsg &that)const
    { return weight != that.weight; }
    bool operator<=(const Generalmsg &that)const
    { return weight <= that.weight; }
    bool operator>=(const Generalmsg &that)const
    { return weight >= that.weight; }






private:
    string id;
    string rev;
    string payload;
    int weight{};




};


#endif //FAR_STATION_ROBIN_GENERALMSG_H
