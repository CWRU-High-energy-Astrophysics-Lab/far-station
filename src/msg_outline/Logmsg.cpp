//
// Created by robin on 10/21/21.
//

#include "Logmsg.h"

#include <utility>

using namespace std;
Logmsg::Logmsg( string payload) : Generalmsg("LOGA", "rev1", std::move(payload),1) {


}
