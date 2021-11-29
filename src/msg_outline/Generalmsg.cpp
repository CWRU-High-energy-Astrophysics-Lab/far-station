//
// Created by robin on 10/21/21.
//

#include "Generalmsg.h"

#include <utility>

unsigned long Generalmsg::getSize() const {
    return payload.length(); //one character is one byte
}

string Generalmsg::gedID() const {
    return id;
}

string Generalmsg::getRev() const {
    return rev;
}

Generalmsg::Generalmsg(string id, string rev, string payload, int weight) {
    this->id = std::move(id);
    this->rev = std::move(rev);
    this->payload = std::move(payload);
    this->weight =weight;

}

string Generalmsg::getPayload() const {
    return payload;
}

Generalmsg::Generalmsg() = default;

int Generalmsg::getweight() const{
    return weight;
}
