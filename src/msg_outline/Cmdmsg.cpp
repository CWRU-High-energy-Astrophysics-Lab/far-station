//
// Created by robin on 10/22/21.
//

#include "Cmdmsg.h"

// CMDB = bash command
//CMDA = os9

Cmdmsg::Cmdmsg(string cmd, char type) : Generalmsg("CMD"+type, "Rev1", cmd,5) {

}
