cmake_minimum_required(VERSION 3.30)
project(Server)

set(CMAKE_CXX_STANDARD 20)

add_executable(Server main.cpp
        GameServer.cpp
        GameServer.h
        Player.cpp
        Player.h)
