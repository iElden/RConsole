//
// Created by Gegel85 on 25/06/2020.
//

#ifndef RCONSOLE_RPACKETS_HPP
#define RCONSOLE_RPACKETS_HPP


#include <cstdint>

#define PACKED __attribute((packed))
#define MAX_USERNAME 64
#define MAX_PASSWORD 64
#define MAX_MESSAGE 64

namespace RC::Network
{
	enum PACKED Opcode {
		HELLO,
		OLLEH,
		PING,
		PONG,
		GOODBYE,
		KICKED,
		JOIN_LOBBY,
		CREATE_LOBBY,
		DELETE_LOBBY,
		LEAVE_LOBBY,
		LOBBY_JOINED,
		LOBBY_CREATED,
		LOBBY_DELETED,
		PLAYER_JOINED,
		OK,
		ERROR,
		LOBBY_LIST_REQUEST,
		LOBBY_LIST,
		LOBBY_STATE_REQUEST,
		LOBBY_STATE,
		PLAYER_READY,
		SET_READY,
		CHOOSE_GAME,
		GAME_START,
		GAME_EVENT
	};

	struct PACKED PacketHello {
		uint64_t dataSize;
		Opcode code;
		char username[MAX_USERNAME];
		char password[MAX_PASSWORD];
	};

	struct PACKED PacketOlleh {
		uint64_t dataSize;
		Opcode code;
		uint32_t id;
	};

	struct PACKED PacketKicked {
		uint64_t dataSize;
		Opcode code;
		char reason[MAX_MESSAGE];
	};

	struct PACKED PacketPing {
		uint64_t dataSize;
		Opcode code;
		uint32_t lastPing;
	};

	struct PACKED NPlayer {
		uint32_t id;
		uint32_t ping;
		char username[MAX_USERNAME];
	};

	struct PACKED PacketLobbyState {
		uint64_t dataSize;
		Opcode code;
		uint32_t playerCount;
		NPlayer players[0];
	};

	struct PACKED PacketLobbyJoined {
		uint64_t dataSize;
		Opcode code;
		uint32_t playerCount;
		NPlayer players[0];
	};

	struct PACKED NLobby {
		uint32_t id;
	};

	struct PACKED PacketLobbyCreated {
		uint64_t dataSize;
		Opcode code;
		NLobby lobby;
	};

	struct PACKED PacketLobbyDeleted {
		uint64_t dataSize;
		Opcode code;
		NLobby lobby;
	};

	struct PACKED PacketLobbyList {
		uint64_t dataSize;
		Opcode code;
		uint32_t nbLobby;
		NLobby lobbies[0];
	};

	struct PACKED PacketError {
		uint64_t dataSize;
		Opcode code;
		char error[MAX_MESSAGE];
	};

	struct PACKED PacketPlayerJoined {
		uint64_t dataSize;
		Opcode code;
		NPlayer player;
	};

	struct PACKED PacketJoinLobby {
		uint64_t dataSize;
		Opcode code;
		uint32_t id;
	};

	struct PACKED PacketCreateLobby {
		uint64_t dataSize;
		Opcode code;
		NLobby lobby;
	};

	struct PACKED PacketChooseGame {
		uint64_t dataSize;
		Opcode code;
		uint32_t id;
	};

	struct PACKED PacketGameStart {
		uint64_t dataSize;
		Opcode code;
		uint32_t gameSelected;
	};

	struct PacketGameEvent {
		uint64_t dataSize;
		Opcode code;
		char gameData[0];
	};

	struct PacketPlayerReady {
		uint64_t dataSize;
		Opcode code;
		uint32_t id;
	};

	struct PacketHeader {
		uint64_t dataSize;
		Opcode code;
	};

	union PACKED Packet {
		PacketHeader header;
		PacketHello hello;
		PacketOlleh olleh;
		PacketKicked kicked;
		PacketPing ping;
		PacketLobbyJoined lobbyJoined;
		PacketLobbyState lobbyState;
		PacketLobbyCreated lobbyCreated;
		PacketLobbyDeleted lobbyDeleted;
		PacketLobbyList lobbyList;
		PacketError error;
		PacketPlayerJoined playerJoined;
		PacketPlayerReady playerReady;
		PacketJoinLobby joinLobby;
		PacketCreateLobby createLobby;
		PacketChooseGame chooseGame;
		PacketGameStart gameStart;
		PacketGameEvent gameEvent;
	};
}


#endif //RCONSOLE_RPACKETS_HPP
