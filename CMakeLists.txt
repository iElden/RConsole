cmake_minimum_required(VERSION 3.14)
project(RConsole)

set(CMAKE_CXX_STANDARD 14)

add_executable(client
               client/main.cpp client/RPlayer.hpp client/RPlayer.cpp client/RClientLobby.cpp client/RClientLobby.hpp)

add_executable(server
               server/main.cpp)