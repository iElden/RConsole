cmake_minimum_required(VERSION 3.14)
project(RConsole)

set(CMAKE_CXX_STANDARD 20)

set(
	NETWORK_SRC
	network/RPackets.hpp
	network/Utils.hpp
	network/Utils.cpp
	network/Connection.cpp
	network/Connection.hpp
	network/Exceptions.hpp
	network/ClientConnection.cpp
	network/ClientConnection.hpp
	network/ServerConnection.cpp
	network/ServerConnection.hpp
)

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

target_link_libraries(client sfml-network sfml-system)


add_executable(
		server
		server/main.cpp
		${NETWORK_SRC}
)

target_link_libraries(server sfml-network sfml-system)