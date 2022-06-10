#! /usr/bin/bash

cd ~/far-station || cd ~/CLionProjects/far-station || exit;#
cmake CMakeLists.txt;#
make;#
./~/far-station/far_station || ./~/CLionProjects/far-station/far_station || exit;#