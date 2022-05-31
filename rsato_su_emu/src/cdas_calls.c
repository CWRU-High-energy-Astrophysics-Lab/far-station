//
// Created by robin on 5/31/22.
//
#include "cdas_calls.h"

char* getT2Fromcdas(){
    return dequeue(&t2q);
}