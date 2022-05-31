cmake_minimum_required(VERSION 3.20)
project(far_station)

set(CMAKE_CXX_STANDARD 20)
find_package(Threads)
SET(CMAKE_CXX_FLAGS -pthread)

add_executable(cl rsato_su_emu/src/cl.c rsato_su_emu/src/sockets.c)
add_executable(cdas_su_emu rsato_su_emu/src/cdas_su_emu.c rsato_su_emu/src/msg_interpreter.c rsato_su_emu/src/ub_side.c rsato_su_emu/src/protocol.c rsato_su_emu/src/t2.c rsato_su_emu/src/t3req.c rsato_su_emu/src/sockets.c rsato_su_emu/src/cdas_calls.c)
add_executable(su_emu rsato_su_emu/src/su_emu.c rsato_su_emu/src/ub_side.c rsato_su_emu/src/protocol.c)
add_executable(su_bsu_emu rsato_su_emu/src/su_bsu_emu.c rsato_su_emu/src/ub_side.c rsato_su_emu/src/pm_side.c rsato_su_emu/src/protocol.c)
add_executable(far_station src/msg_outline/T2msg.cpp
        src/msg_outline/T2msg.h
        src/msg_outline/Generalmsg.cpp
        src/msg_outline/Generalmsg.h
        src/msg_outline/Logmsg.cpp
        src/msg_outline/Logmsg.h
        src/msg_outline/T3msg.cpp
        src/msg_outline/T3msg.h
        src/msg_outline/MsgHistory.cpp
        src/msg_outline/MsgHistory.h
        src/msg_outline/Cmdmsg.h
        src/msg_outline/Cmdmsg.cpp
        src/msg_outline/augermsg.h
        src/FarProccess.cpp
        src/FarProccess.hpp
        src/t2li.h
        src/xbeeinterface.h
        src/xbeeproccess.cpp
        rsato_su_emu/src/cdas_calls.c)



