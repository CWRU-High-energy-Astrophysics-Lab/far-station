#! /usr/bin/bash

cd ~/far-station || cd ~/CLionProjects/far-station || exit;#
cmake CMakeLists.txt;#
make;#
./far_station || exit;#