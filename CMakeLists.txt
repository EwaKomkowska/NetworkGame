cmake_minimum_required(VERSION 3.12)
project(SK_Pseudo_Game)

set(CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_FLAGS "-pthread")

add_executable(server After.cpp Client.cpp Client.h Server.cpp Server.h Handler.h)
