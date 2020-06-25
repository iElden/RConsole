cmake_minimum_required(VERSION 3.14)
project(RConsole)

set(CMAKE_CXX_STANDARD 20)

set(NETWORK_SRC
    network/RPackets.hpp
    network/Utils.hpp network/Utils.cpp network/Connection.cpp network/Connection.hpp)

add_executable(
		client
		client/main.cpp
		${NETWORK_SRC}
		client/RPlayer.hpp
		client/RPlayer.cpp
		client/RClientLobby.cpp
		client/RClientLobby.hpp
		client/RClient.cpp
		client/RClient.hpp
)


add_executable(
		server
		server/main.cpp
		${NETWORK_SRC}
		server/ClientConnection.cpp server/ClientConnection.hpp network/Utils.hpp network/Utils.cpp network/Connection.cpp network/Connection.hpp
)