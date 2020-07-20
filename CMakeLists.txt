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
	games/pong/PongPackets.hpp
	games/pong/Utils.cpp
	games/pong/Utils.hpp
)

add_executable(
	client
	client/main.cpp
	${NETWORK_SRC}
	client/Player.hpp
	client/Player.cpp
	client/Lobby.cpp
	client/Lobby.hpp
	client/NetworkClient.cpp
	client/NetworkClient.hpp
	client/Client.cpp
	client/Client.hpp
	client/Exceptions.hpp
	client/Utils.hpp
	client/Utils.cpp
	client/Controller/MobileController.cpp
	client/Controller/MobileController.hpp
	client/Controller/Exceptions.hpp
	client/Controller/Packet.hpp
	games/pong/CGame.cpp
	games/pong/CGame.hpp
	games/ICGame.hpp
	client/Controller/IController.hpp client/Controller/KeyboardController.cpp client/Controller/KeyboardController.hpp)

if (WIN32)
	target_link_libraries(client tgui sfml-audio sfml-graphics sfml-window sfml-network sfml-system pthread)
else (WIN32)
	target_link_libraries(client tgui sfml-audio sfml-graphics sfml-window sfml-network sfml-system pthread X11)
endif (WIN32)


add_executable(
	server
	server/main.cpp
	${NETWORK_SRC}
	server/ServerMain.cpp
	server/ServerMain.hpp
	server/Client.cpp
	server/Client.hpp
	server/exc.hpp
	server/Lobby.cpp
	server/Lobby.hpp
	server/ClientList.cpp
	server/ClientList.hpp
	server/LobbyList.cpp
	server/LobbyList.hpp
	games/pong/SGame.cpp
	games/pong/SGame.hpp
	games/pong/GameObject.cpp
	games/pong/GameObject.hpp
	games/pong/Player.cpp
	games/pong/Player.hpp
	games/ISGame.hpp
)

target_link_libraries(server sfml-network sfml-system pthread)